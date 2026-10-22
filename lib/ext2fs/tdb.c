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

#अन्तर्भाव "config.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <string.h>
#ifdef HAVE_SYS_SELECT_H
#अन्तर्भाव <sys/select.h>
#endif
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <time.h>
#ifdef HAVE_UTIME_H
#अन्तर्भाव <utime.h>
#endif
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/file.h>
#अन्तर्भाव <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#अन्तर्भाव <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#अन्यथा
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((शून्य *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
स्थिर अक्षर *rep_strdup(स्थिरांक अक्षर *s)
{
	अक्षर *ret;
	पूर्णांक length;

	जर (!s)
		परत रिक्त;
	length = म_माप(s);
	ret = दो_स्मृति(length + 1);
	जर (ret) {
		म_नकलन(ret, s, length);
		ret[length] = '\0';
	}
	परत ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#जर (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#अन्यथा
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#जर defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
प्रकारव्याख्या पूर्णांक bool;
#endif

#अन्तर्भाव "tdb.h"

स्थिर TDB_DATA tdb_null;

#ifndef u32
#define u32 अचिन्हित
#endif

प्रकारव्याख्या u32 tdb_len_t;
प्रकारव्याख्या u32 tdb_off_t;

#ifndef दुरत्व
#define दुरत्व(t,f) ((अचिन्हित पूर्णांक)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*आकार(संरचना list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_शीर्ष (आकार(संरचना tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_शीर्ष(hash) (FREELIST_शीर्ष + (BUCKET(hash)+1)*आकार(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*आकार(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_शीर्ष(hash_size-1)
#define TDB_RECOVERY_HEAD दुरत्व(संरचना tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    दुरत्व(संरचना tdb_header, sequence_संख्या)
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
#define SAFE_FREE(x) करा { जर ((x) != रिक्त) {मुक्त(x); (x)=रिक्त;} } पर्यंत(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, आकार(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
संरचना list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t किल्ली_len; /* byte length of key */
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
संरचना tdb_header {
	अक्षर magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_संख्या; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

संरचना tdb_lock_type {
	पूर्णांक list;
	u32 मोजणी;
	u32 ltype;
};

संरचना tdb_traverse_lock {
	संरचना tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	पूर्णांक lock_rw;
};


संरचना tdb_methods {
	पूर्णांक (*tdb_read)(संरचना tdb_context *, tdb_off_t , शून्य *, tdb_len_t , पूर्णांक );
	पूर्णांक (*tdb_write)(संरचना tdb_context *, tdb_off_t, स्थिरांक शून्य *, tdb_len_t);
	शून्य (*next_hash_chain)(संरचना tdb_context *, u32 *);
	पूर्णांक (*tdb_oob)(संरचना tdb_context *, tdb_off_t , पूर्णांक );
	पूर्णांक (*tdb_expand_file)(संरचना tdb_context *, tdb_off_t , tdb_off_t );
	पूर्णांक (*tdb_brlock)(संरचना tdb_context *, tdb_off_t , पूर्णांक, पूर्णांक, पूर्णांक, माप_प्रकार);
};

संरचना tdb_context {
	अक्षर *name; /* the name of the database */
	शून्य *map_ptr; /* where it is currently mapped */
	पूर्णांक fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	पूर्णांक read_only; /* opened read-only */
	पूर्णांक traverse_read; /* read-only traversal */
	संरचना tdb_lock_type global_lock;
	पूर्णांक अंक_lockrecs;
	संरचना tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	गणना TDB_ERROR ecode; /* error code for last tdb error */
	संरचना tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	संरचना tdb_traverse_lock travlocks; /* current traversal locks */
	संरचना tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	संरचना tdb_logging_context log;
	अचिन्हित पूर्णांक (*hash_fn)(TDB_DATA *किल्ली);
	पूर्णांक open_flags; /* flags used in the open - needed by reopen */
	अचिन्हित पूर्णांक अंक_locks; /* number of chain locks held */
	स्थिरांक संरचना tdb_methods *methods;
	संरचना tdb_transaction *transaction;
	पूर्णांक page_size;
	पूर्णांक max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
स्थिर पूर्णांक tdb_munmap(संरचना tdb_context *tdb);
स्थिर शून्य tdb_mmap(संरचना tdb_context *tdb);
स्थिर पूर्णांक tdb_lock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype);
पूर्णांक tdb_lock_nonblock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype);
स्थिर पूर्णांक tdb_unlock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype);
स्थिर पूर्णांक tdb_brlock(संरचना tdb_context *tdb, tdb_off_t offset, पूर्णांक rw_type, पूर्णांक lck_type, पूर्णांक probe, माप_प्रकार len);
स्थिर पूर्णांक tdb_transaction_lock(संरचना tdb_context *tdb, पूर्णांक ltype);
स्थिर पूर्णांक tdb_transaction_unlock(संरचना tdb_context *tdb);
स्थिर पूर्णांक tdb_brlock_upgrade(संरचना tdb_context *tdb, tdb_off_t offset, माप_प्रकार len);
स्थिर पूर्णांक tdb_write_lock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णांक tdb_write_unlock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णांक tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर पूर्णांक tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर शून्य *tdb_convert(शून्य *buf, u32 size);
स्थिर पूर्णांक tdb_free(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec);
स्थिर tdb_off_t tdb_allocate(संरचना tdb_context *tdb, tdb_len_t length, संरचना list_struct *rec);
स्थिर पूर्णांक tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर पूर्णांक tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर पूर्णांक tdb_lock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णांक tdb_unlock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णांक tdb_rec_read(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec);
स्थिर पूर्णांक tdb_rec_write(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec);
स्थिर पूर्णांक tdb_do_delete(संरचना tdb_context *tdb, tdb_off_t rec_ptr, संरचना list_struct *rec);
स्थिर अचिन्हित अक्षर *tdb_alloc_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
स्थिर पूर्णांक tdb_parse_data(संरचना tdb_context *tdb, TDB_DATA किल्ली,
		   tdb_off_t offset, tdb_len_t len,
		   पूर्णांक (*parser)(TDB_DATA किल्ली, TDB_DATA data,
				 शून्य *private_data),
		   शून्य *private_data);
स्थिर tdb_off_t tdb_find_lock_hash(संरचना tdb_context *tdb, TDB_DATA किल्ली, u32 hash, पूर्णांक locktype,
			   संरचना list_struct *rec);
स्थिर शून्य tdb_io_init(संरचना tdb_context *tdb);
स्थिर पूर्णांक tdb_expand(संरचना tdb_context *tdb, tdb_off_t size);
स्थिर पूर्णांक tdb_rec_free_read(संरचना tdb_context *tdb, tdb_off_t off,
		      संरचना list_struct *rec);


/* file: error.c */

गणना TDB_ERROR tdb_error(संरचना tdb_context *tdb)
{
	परत tdb->ecode;
}

स्थिर संरचना tdb_errname {
	गणना TDB_ERROR ecode; स्थिरांक अक्षर *estring;
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
स्थिरांक अक्षर *tdb_errorstr(संरचना tdb_context *tdb)
{
	u32 i;
	साठी (i = 0; i < आकार(emap) / आकार(संरचना tdb_errname); i++)
		जर (tdb->ecode == emap[i].ecode)
			परत emap[i].estring;
	परत "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
पूर्णांक tdb_brlock(संरचना tdb_context *tdb, tdb_off_t offset,
	       पूर्णांक rw_type, पूर्णांक lck_type, पूर्णांक probe, माप_प्रकार len)
{
	संरचना flock fl;
	पूर्णांक ret;

	जर (tdb->flags & TDB_NOLOCK) {
		परत 0;
	}

	जर ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		परत -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = शुरू_से;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	करा {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} पर्यंत (ret == -1 && त्रुटि_सं == EINTR);

	जर (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		जर (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (पूर्णांक)len));
		}
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	परत 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
पूर्णांक tdb_brlock_upgrade(संरचना tdb_context *tdb, tdb_off_t offset, माप_प्रकार len)
{
	पूर्णांक मोजणी = 1000;
	पर्यंत (मोजणी--) {
		संरचना timeval tv;
		जर (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			परत 0;
		}
		जर (त्रुटि_सं != EDEADLK) {
			थांबा;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, रिक्त, रिक्त, रिक्त, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	परत -1;
}


/* lock a list in the database. list -1 is the alloc list */
स्थिर पूर्णांक _tdb_lock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype, पूर्णांक op)
{
	संरचना tdb_lock_type *new_lck;
	पूर्णांक i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	जर (tdb->global_lock.मोजणी &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		परत 0;
	}

	जर (tdb->global_lock.मोजणी) {
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जर (list < -1 || list >= (पूर्णांक)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		परत -1;
	}
	जर (tdb->flags & TDB_NOLOCK)
		परत 0;

	साठी (i=0; i<tdb->अंक_lockrecs; i++) {
		जर (tdb->lockrecs[i].list == list) {
			जर (tdb->lockrecs[i].मोजणी == 0) {
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
			tdb->lockrecs[i].मोजणी++;
			परत 0;
		}
	}

	new_lck = (संरचना tdb_lock_type *)पुनः_स्मृति(
		tdb->lockrecs,
		आकार(*tdb->lockrecs) * (tdb->अंक_lockrecs+1));
	जर (new_lck == रिक्त) {
		त्रुटि_सं = ENOMEM;
		परत -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	जर (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_शीर्ष+4*list, ltype, op,
				     0, 1)) {
		परत -1;
	}

	tdb->अंक_locks++;

	tdb->lockrecs[tdb->अंक_lockrecs].list = list;
	tdb->lockrecs[tdb->अंक_lockrecs].मोजणी = 1;
	tdb->lockrecs[tdb->अंक_lockrecs].ltype = ltype;
	tdb->अंक_lockrecs += 1;

	परत 0;
}

/* lock a list in the database. list -1 is the alloc list */
पूर्णांक tdb_lock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype)
{
	पूर्णांक ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	जर (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, म_त्रुटि(त्रुटि_सं)));
	}
	परत ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
पूर्णांक tdb_lock_nonblock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype)
{
	परत _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
पूर्णांक tdb_unlock(संरचना tdb_context *tdb, पूर्णांक list, पूर्णांक ltype)
{
	पूर्णांक ret = -1;
	पूर्णांक i;
	संरचना tdb_lock_type *lck = रिक्त;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	जर (tdb->global_lock.मोजणी &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		परत 0;
	}

	जर (tdb->global_lock.मोजणी) {
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जर (tdb->flags & TDB_NOLOCK)
		परत 0;

	/* Sanity checks */
	जर (list < -1 || list >= (पूर्णांक)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		परत ret;
	}

	साठी (i=0; i<tdb->अंक_lockrecs; i++) {
		जर (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			थांबा;
		}
	}

	जर ((lck == रिक्त) || (lck->मोजणी == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		परत -1;
	}

	जर (lck->मोजणी > 1) {
		lck->मोजणी--;
		परत 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	जर (mark_lock) {
		ret = 0;
	} अन्यथा {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_शीर्ष+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->अंक_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	जर (tdb->अंक_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->अंक_lockrecs-1];
	}
	tdb->अंक_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	जर (tdb->अंक_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	जर (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	परत ret;
}

/*
  get the transaction lock
 */
पूर्णांक tdb_transaction_lock(संरचना tdb_context *tdb, पूर्णांक ltype)
{
	जर (tdb->have_transaction_lock || tdb->global_lock.मोजणी) {
		परत 0;
	}
	जर (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		परत -1;
	}
	tdb->have_transaction_lock = 1;
	परत 0;
}

/*
  release the transaction lock
 */
पूर्णांक tdb_transaction_unlock(संरचना tdb_context *tdb)
{
	पूर्णांक ret;
	जर (!tdb->have_transaction_lock) {
		परत 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	जर (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	परत ret;
}




/* lock/unlock entire database */
स्थिर पूर्णांक _tdb_lockall(संरचना tdb_context *tdb, पूर्णांक ltype, पूर्णांक op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	जर (tdb->read_only || tdb->traverse_read)
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);

	जर (tdb->global_lock.मोजणी && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.मोजणी++;
		परत 0;
	}

	जर (tdb->global_lock.मोजणी) {
		/* a global lock of a different type exists */
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जर (tdb->अंक_locks != 0) {
		/* can't combine global and chain locks */
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जर (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शीर्ष, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		जर (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		}
		परत -1;
	}

	tdb->global_lock.मोजणी = 1;
	tdb->global_lock.ltype = ltype;

	परत 0;
}



/* unlock entire db */
स्थिर पूर्णांक _tdb_unlockall(संरचना tdb_context *tdb, पूर्णांक ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	जर (tdb->read_only || tdb->traverse_read) {
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जर (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.मोजणी == 0) {
		परत TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जर (tdb->global_lock.मोजणी > 1) {
		tdb->global_lock.मोजणी--;
		परत 0;
	}

	जर (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		परत -1;
	}

	tdb->global_lock.मोजणी = 0;
	tdb->global_lock.ltype = 0;

	परत 0;
}

/* lock entire database with write lock */
पूर्णांक tdb_lockall(संरचना tdb_context *tdb)
{
	परत _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
पूर्णांक tdb_lockall_mark(संरचना tdb_context *tdb)
{
	परत _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
पूर्णांक tdb_lockall_unmark(संरचना tdb_context *tdb)
{
	परत _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
पूर्णांक tdb_lockall_nonblock(संरचना tdb_context *tdb)
{
	परत _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
पूर्णांक tdb_unlockall(संरचना tdb_context *tdb)
{
	परत _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
पूर्णांक tdb_lockall_read(संरचना tdb_context *tdb)
{
	परत _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
पूर्णांक tdb_lockall_read_nonblock(संरचना tdb_context *tdb)
{
	परत _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
पूर्णांक tdb_unlockall_read(संरचना tdb_context *tdb)
{
	परत _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
पूर्णांक tdb_chainlock(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_lock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
पूर्णांक tdb_chainlock_nonblock(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णांक tdb_chainlock_mark(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_lock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णांक tdb_chainlock_unmark(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_unlock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_WRLCK | TDB_MARK_LOCK);
}

पूर्णांक tdb_chainunlock(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_unlock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_WRLCK);
}

पूर्णांक tdb_chainlock_read(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_lock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_RDLCK);
}

पूर्णांक tdb_chainunlock_read(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	परत tdb_unlock(tdb, BUCKET(tdb->hash_fn(&किल्ली)), F_RDLCK);
}



/* record lock stops delete underneath */
पूर्णांक tdb_lock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	परत off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णांक tdb_write_lock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	संरचना tdb_traverse_lock *i;
	साठी (i = &tdb->travlocks; i; i = i->next)
		जर (i->off == off)
			परत -1;
	परत tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णांक tdb_write_unlock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	परत tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
पूर्णांक tdb_unlock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	संरचना tdb_traverse_lock *i;
	u32 मोजणी = 0;

	जर (off == 0)
		परत 0;
	साठी (i = &tdb->travlocks; i; i = i->next)
		जर (i->off == off)
			मोजणी++;
	परत (मोजणी == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
स्थिर पूर्णांक tdb_oob(संरचना tdb_context *tdb, tdb_off_t len, पूर्णांक probe)
{
	संरचना stat st;
	जर (len <= tdb->map_size)
		परत 0;
	जर (tdb->flags & TDB_INTERNAL) {
		जर (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (पूर्णांक)len, (पूर्णांक)tdb->map_size));
		}
		परत TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	जर (ख_स्थिति(tdb->fd, &st) == -1) {
		परत TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	जर (st.st_size < (off_t)len) {
		जर (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (पूर्णांक)len, (पूर्णांक)st.st_size));
		}
		परत TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	जर (tdb_munmap(tdb) == -1)
		परत TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	परत 0;
}

/* write a lump of data at a specified offset */
स्थिर पूर्णांक tdb_write(संरचना tdb_context *tdb, tdb_off_t off,
		     स्थिरांक शून्य *buf, tdb_len_t len)
{
	जर (len == 0) {
		परत 0;
	}

	जर (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		परत -1;
	}

	जर (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		परत -1;

	जर (tdb->map_ptr) {
		स_नकल(off + (अक्षर *)tdb->map_ptr, buf, len);
	} अन्यथा जर (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, म_त्रुटि(त्रुटि_सं)));
		परत TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	परत 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
शून्य *tdb_convert(शून्य *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	साठी (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	परत buf;
}


/* read a lump of data at a specified offset, maybe convert */
स्थिर पूर्णांक tdb_read(संरचना tdb_context *tdb, tdb_off_t off, शून्य *buf,
		    tdb_len_t len, पूर्णांक cv)
{
	जर (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		परत -1;
	}

	जर (tdb->map_ptr) {
		स_नकल(buf, off + (अक्षर *)tdb->map_ptr, len);
	} अन्यथा {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		जर (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (पूर्णांक)off, (पूर्णांक)len, (पूर्णांक)ret, म_त्रुटि(त्रुटि_सं),
				 (पूर्णांक)tdb->map_size));
			परत TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	जर (cv) {
		tdb_convert(buf, len);
	}
	परत 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
स्थिर शून्य tdb_next_hash_chain(संरचना tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	जर (tdb->map_ptr) {
		साठी (;h < tdb->header.hash_size;h++) {
			जर (0 != *(u32 *)(TDB_HASH_शीर्ष(h) + (अचिन्हित अक्षर *)tdb->map_ptr)) {
				थांबा;
			}
		}
	} अन्यथा {
		u32 off=0;
		साठी (;h < tdb->header.hash_size;h++) {
			जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(h), &off) != 0 || off != 0) {
				थांबा;
			}
		}
	}
	(*chain) = h;
}


पूर्णांक tdb_munmap(संरचना tdb_context *tdb)
{
	जर (tdb->flags & TDB_INTERNAL)
		परत 0;

#ifdef HAVE_MMAP
	जर (tdb->map_ptr) {
		पूर्णांक ret = munmap(tdb->map_ptr, tdb->real_map_size);
		जर (ret != 0)
			परत ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = रिक्त;
	परत 0;
}

शून्य tdb_mmap(संरचना tdb_context *tdb)
{
	जर (tdb->flags & TDB_INTERNAL)
		परत;

#ifdef HAVE_MMAP
	जर (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(रिक्त, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		जर (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = रिक्त;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, म_त्रुटि(त्रुटि_सं)));
		}
		tdb->real_map_size = tdb->map_size;
	} अन्यथा {
		tdb->map_ptr = रिक्त;
	}
#अन्यथा
	tdb->map_ptr = रिक्त;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
स्थिर पूर्णांक tdb_expand_file(संरचना tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	अक्षर buf[1024];

	जर (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		परत -1;
	}

	जर (ftruncate(tdb->fd, size+addition) == -1) {
		अक्षर b = 0;
		जर (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, म_त्रुटि(त्रुटि_सं)));
			परत -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	स_रखो(buf, TDB_PAD_BYTE, आकार(buf));
	पर्यंत (addition) {
		पूर्णांक n = addition>आकार(buf)?आकार(buf):addition;
		पूर्णांक ret = pwrite(tdb->fd, buf, n, size);
		जर (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, म_त्रुटि(त्रुटि_सं)));
			परत -1;
		}
		addition -= n;
		size += n;
	}
	परत 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
पूर्णांक tdb_expand(संरचना tdb_context *tdb, tdb_off_t size)
{
	संरचना list_struct rec;
	tdb_off_t offset;

	जर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		परत -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	जर (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	जर (!(tdb->flags & TDB_INTERNAL)) {
		जर (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			जा fail;
	}

	tdb->map_size += size;

	जर (tdb->flags & TDB_INTERNAL) {
		अक्षर *new_map_ptr = (अक्षर *)पुनः_स्मृति(tdb->map_ptr,
						    tdb->map_size);
		जर (!new_map_ptr) {
			tdb->map_size -= size;
			जा fail;
		}
		tdb->map_ptr = new_map_ptr;
	} अन्यथा {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	स_रखो(&rec,'\0',आकार(rec));
	rec.rec_len = size - आकार(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	जर (tdb_free(tdb, offset, &rec) == -1)
		जा fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	परत 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	परत -1;
}

/* read/write a tdb_off_t */
पूर्णांक tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	परत tdb->methods->tdb_read(tdb, offset, (अक्षर*)d, आकार(*d), DOCONV());
}

पूर्णांक tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	परत tdb->methods->tdb_write(tdb, offset, CONVERT(off), आकार(*d));
}


/* read a lump of data, allocating the space for it */
अचिन्हित अक्षर *tdb_alloc_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	अचिन्हित अक्षर *buf;

	/* some systems don't like zero length malloc */
	जर (len == 0) {
		len = 1;
	}

	जर (!(buf = (अचिन्हित अक्षर *)दो_स्मृति(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, म_त्रुटि(त्रुटि_सं)));
		परत TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	जर (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		परत रिक्त;
	}
	परत buf;
}

/* Give a piece of tdb data to a parser */

पूर्णांक tdb_parse_data(संरचना tdb_context *tdb, TDB_DATA किल्ली,
		   tdb_off_t offset, tdb_len_t len,
		   पूर्णांक (*parser)(TDB_DATA किल्ली, TDB_DATA data,
				 शून्य *private_data),
		   शून्य *private_data)
{
	TDB_DATA data;
	पूर्णांक result;

	data.dsize = len;

	जर ((tdb->transaction == रिक्त) && (tdb->map_ptr != रिक्त)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		जर (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			परत -1;
		}
		data.dptr = offset + (अचिन्हित अक्षर *)tdb->map_ptr;
		परत parser(किल्ली, data, private_data);
	}

	जर (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		परत -1;
	}

	result = parser(किल्ली, data, private_data);
	मुक्त(data.dptr);
	परत result;
}

/* read/write a record */
पूर्णांक tdb_rec_read(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec)
{
	जर (tdb->methods->tdb_read(tdb, offset, rec, आकार(*rec),DOCONV()) == -1)
		परत -1;
	जर (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		परत TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	परत tdb->methods->tdb_oob(tdb, rec->next+आकार(*rec), 0);
}

पूर्णांक tdb_rec_write(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec)
{
	संरचना list_struct r = *rec;
	परत tdb->methods->tdb_write(tdb, offset, CONVERT(r), आकार(r));
}

स्थिर स्थिरांक संरचना tdb_methods io_methods = {
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
शून्य tdb_io_init(संरचना tdb_context *tdb)
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

संरचना tdb_transaction_el {
	संरचना tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	अचिन्हित अक्षर *data;
};

/*
  hold the context of any current transaction
*/
संरचना tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	स्थिरांक संरचना tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	संरचना tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	पूर्णांक transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	पूर्णांक nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
स्थिर पूर्णांक transaction_read(संरचना tdb_context *tdb, tdb_off_t off, शून्य *buf,
			    tdb_len_t len, पूर्णांक cv)
{
	संरचना tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	साठी (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		जर (off+len <= el->offset) {
			सुरूठेवा;
		}
		जर (off >= el->offset + el->length) {
			सुरूठेवा;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		जर (off < el->offset) {
			partial = el->offset - off;
			जर (transaction_read(tdb, off, buf, partial, cv) != 0) {
				जा fail;
			}
			len -= partial;
			off += partial;
			buf = (शून्य *)(partial + (अक्षर *)buf);
		}
		जर (off + len <= el->offset + el->length) {
			partial = len;
		} अन्यथा {
			partial = el->offset + el->length - off;
		}
		स_नकल(buf, el->data + (off - el->offset), partial);
		जर (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (शून्य *)(partial + (अक्षर *)buf);

		जर (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			जा fail;
		}

		परत 0;
	}

	/* its not in the transaction elements - do a real read */
	परत tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	परत -1;
}


/*
  write while in a transaction
*/
स्थिर पूर्णांक transaction_write(संरचना tdb_context *tdb, tdb_off_t off,
			     स्थिरांक शून्य *buf, tdb_len_t len)
{
	संरचना tdb_transaction_el *el, *best_el=रिक्त;

	जर (len == 0) {
		परत 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	जर (len == आकार(tdb_off_t) && off >= FREELIST_शीर्ष &&
	    off < FREELIST_शीर्ष+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_शीर्ष) / आकार(tdb_off_t);
		स_नकल(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	साठी (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		जर (best_el == रिक्त && off == el->offset+el->length) {
			best_el = el;
		}

		जर (off+len <= el->offset) {
			सुरूठेवा;
		}
		जर (off >= el->offset + el->length) {
			सुरूठेवा;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		जर (off < el->offset) {
			partial = el->offset - off;
			जर (transaction_write(tdb, off, buf, partial) != 0) {
				जा fail;
			}
			len -= partial;
			off += partial;
			buf = (स्थिरांक शून्य *)(partial + (स्थिरांक अक्षर *)buf);
		}
		जर (off + len <= el->offset + el->length) {
			partial = len;
		} अन्यथा {
			partial = el->offset + el->length - off;
		}
		स_नकल(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (स्थिरांक शून्य *)(partial + (स्थिरांक अक्षर *)buf);

		जर (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			जा fail;
		}

		परत 0;
	}

	/* see if we can append the new entry to an existing entry */
	जर (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		अचिन्हित अक्षर *data = best_el->data;
		el = best_el;
		el->data = (अचिन्हित अक्षर *)पुनः_स्मृति(el->data,
						    el->length + len);
		जर (el->data == रिक्त) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			परत -1;
		}
		जर (buf) {
			स_नकल(el->data + el->length, buf, len);
		} अन्यथा {
			स_रखो(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		परत 0;
	}

	/* add a new entry at the end of the list */
	el = (संरचना tdb_transaction_el *)दो_स्मृति(आकार(*el));
	जर (el == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		परत -1;
	}
	el->next = रिक्त;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (अचिन्हित अक्षर *)दो_स्मृति(len);
	जर (el->data == रिक्त) {
		मुक्त(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		परत -1;
	}
	जर (buf) {
		स_नकल(el->data, buf, len);
	} अन्यथा {
		स_रखो(el->data, TDB_PAD_BYTE, len);
	}
	जर (el->prev) {
		el->prev->next = el;
	} अन्यथा {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	परत 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	परत -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
स्थिर शून्य transaction_next_hash_chain(संरचना tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	साठी (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		जर (0 != tdb->transaction->hash_heads[h+1]) {
			थांबा;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
स्थिर पूर्णांक transaction_oob(संरचना tdb_context *tdb, tdb_off_t len,
			   पूर्णांक probe EXT2FS_ATTR((unused)))
{
	जर (len <= tdb->map_size) {
		परत 0;
	}
	परत TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
स्थिर पूर्णांक transaction_expand_file(संरचना tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	जर (transaction_write(tdb, size, रिक्त, addition) != 0) {
		परत -1;
	}

	परत 0;
}

/*
  brlock during a transaction - ignore them
*/
स्थिर पूर्णांक transaction_brlock(संरचना tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      पूर्णांक rw_type EXT2FS_ATTR((unused)),
			      पूर्णांक lck_type EXT2FS_ATTR((unused)),
			      पूर्णांक probe EXT2FS_ATTR((unused)),
			      माप_प्रकार len EXT2FS_ATTR((unused)))
{
	परत 0;
}

स्थिर स्थिरांक संरचना tdb_methods transaction_methods = {
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
पूर्णांक tdb_transaction_start(संरचना tdb_context *tdb)
{
	/* some sanity checks */
	जर (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		परत -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	जर (tdb->transaction != रिक्त) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		परत 0;
	}

	जर (tdb->अंक_locks != 0 || tdb->global_lock.मोजणी) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		परत -1;
	}

	जर (tdb->travlocks.next != रिक्त) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		परत -1;
	}

	tdb->transaction = (संरचना tdb_transaction *)
		सुस्मृति(आकार(संरचना tdb_transaction), 1);
	जर (tdb->transaction == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		परत -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	जर (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		परत -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	जर (tdb_brlock(tdb, FREELIST_शीर्ष, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		जा fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		सुस्मृति(tdb->header.hash_size+1, आकार(u32));
	जर (tdb->transaction->hash_heads == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		जा fail;
	}
	जर (tdb->methods->tdb_read(tdb, FREELIST_शीर्ष, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		जा fail;
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
	जर (transaction_write(tdb, FREELIST_शीर्ष, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		जा fail;
	}

	परत 0;

fail:
	tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	परत -1;
}


/*
  cancel the current transaction
*/
पूर्णांक tdb_transaction_cancel(संरचना tdb_context *tdb)
{
	जर (tdb->transaction == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		परत -1;
	}

	जर (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		परत 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	पर्यंत (tdb->transaction->elements) {
		संरचना tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		मुक्त(el->data);
		मुक्त(el);
	}

	/* remove any global lock created during the transaction */
	जर (tdb->global_lock.मोजणी != 0) {
		tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.मोजणी = 0;
	}

	/* remove any locks created during the transaction */
	जर (tdb->अंक_locks != 0) {
		पूर्णांक i;
		साठी (i=0;i<tdb->अंक_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_शीर्ष+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->अंक_locks = 0;
		tdb->अंक_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	परत 0;
}

/*
  sync to disk
*/
स्थिर पूर्णांक transaction_sync(संरचना tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	जर (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		परत -1;
	}
#जर defined(HAVE_MSYNC) && defined(MS_SYNC)
	जर (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		जर (msync(moffset + (अक्षर *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 म_त्रुटि(त्रुटि_सं)));
			परत -1;
		}
	}
#endif
	परत 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
स्थिर tdb_len_t tdb_recovery_size(संरचना tdb_context *tdb)
{
	संरचना tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = आकार(u32);
	साठी (el=tdb->transaction->elements;el;el=el->next) {
		जर (el->offset >= tdb->transaction->old_map_size) {
			सुरूठेवा;
		}
		recovery_size += 2*आकार(tdb_off_t) + el->length;
	}

	परत recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
स्थिर पूर्णांक tdb_recovery_allocate(संरचना tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	संरचना list_struct rec;
	स्थिरांक संरचना tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	जर (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		परत -1;
	}

	rec.rec_len = 0;

	जर (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, आकार(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		परत -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	जर (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		परत 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	जर (recovery_head != 0) {
		जर (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			परत -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(आकार(rec) + *recovery_size, tdb->page_size) - आकार(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	जर (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     आकार(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		परत -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	जर (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, आकार(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		परत -1;
	}

	परत 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
स्थिर पूर्णांक transaction_setup_recovery(संरचना tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	संरचना tdb_transaction_el *el;
	tdb_len_t recovery_size;
	अचिन्हित अक्षर *data, *p;
	स्थिरांक संरचना tdb_methods *methods = tdb->transaction->io_methods;
	संरचना list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	जर (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		परत -1;
	}

	data = (अचिन्हित अक्षर *)दो_स्मृति(recovery_size + आकार(*rec));
	जर (data == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		परत -1;
	}

	rec = (संरचना list_struct *)data;
	स_रखो(rec, 0, आकार(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->किल्ली_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + आकार(*rec);
	साठी (el=tdb->transaction->elements;el;el=el->next) {
		जर (el->offset >= old_map_size) {
			सुरूठेवा;
		}
		जर (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			मुक्त(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			परत -1;
		}
		स_नकल(p, &el->offset, 4);
		स_नकल(p+4, &el->length, 4);
		जर (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		जर (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			मुक्त(data);
			tdb->ecode = TDB_ERR_IO;
			परत -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = आकार(*rec) + recovery_max_size;
	स_नकल(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	जर (methods->tdb_write(tdb, recovery_offset, data, आकार(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		मुक्त(data);
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	जर (transaction_sync(tdb, recovery_offset, आकार(*rec) + recovery_size) == -1) {
		मुक्त(data);
		परत -1;
	}

	मुक्त(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + दुरत्व(संरचना list_struct, magic);

	जर (methods->tdb_write(tdb, *magic_offset, &magic, आकार(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	/* ensure the recovery magic marker is on disk */
	जर (transaction_sync(tdb, *magic_offset, आकार(magic)) == -1) {
		परत -1;
	}

	परत 0;
}

/*
  commit the current transaction
*/
पूर्णांक tdb_transaction_commit(संरचना tdb_context *tdb)
{
	स्थिरांक संरचना tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	जर (tdb->transaction == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		परत -1;
	}

	जर (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		परत -1;
	}

	जर (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		परत 0;
	}

	/* check for a null transaction */
	जर (tdb->transaction->elements == रिक्त) {
		tdb_transaction_cancel(tdb);
		परत 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	जर (tdb->अंक_locks || tdb->global_lock.मोजणी) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		परत -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	जर (tdb_brlock_upgrade(tdb, FREELIST_शीर्ष, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		परत -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	जर (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		परत -1;
	}

	जर (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		जर (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			परत -1;
		}
	}

	/* expand the file to the new size if needed */
	जर (tdb->map_size != tdb->transaction->old_map_size) {
		जर (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			परत -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	पर्यंत (tdb->transaction->elements) {
		संरचना tdb_transaction_el *el = tdb->transaction->elements;

		जर (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			परत -1;
		}
		tdb->transaction->elements = el->next;
		मुक्त(el->data);
		मुक्त(el);
	}

	जर (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		जर (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			परत -1;
		}

		/* remove the recovery marker */
		जर (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			परत -1;
		}

		/* ensure the recovery marker has been removed on disk */
		जर (transaction_sync(tdb, magic_offset, 4) == -1) {
			परत -1;
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
	utime(tdb->name, रिक्त);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	परत 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
पूर्णांक tdb_transaction_recover(संरचना tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	अचिन्हित अक्षर *data, *p;
	u32 zero = 0;
	संरचना list_struct rec;

	/* find the recovery area */
	जर (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	जर (recovery_head == 0) {
		/* we have never allocated a recovery record */
		परत 0;
	}

	/* read the recovery record */
	जर (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   आकार(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	जर (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		परत 0;
	}

	जर (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		परत -1;
	}

	recovery_eof = rec.किल्ली_len;

	data = (अचिन्हित अक्षर *)दो_स्मृति(rec.data_len);
	जर (data == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		परत -1;
	}

	/* read the full recovery data */
	जर (tdb->methods->tdb_read(tdb, recovery_head + आकार(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		मुक्त(data);
		परत -1;
	}

	/* recover the file data */
	p = data;
	पर्यंत (p+8 < data + rec.data_len) {
		u32 ofs, len;
		जर (DOCONV()) {
			tdb_convert(p, 8);
		}
		स_नकल(&ofs, p, 4);
		स_नकल(&len, p+4, 4);

		जर (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			मुक्त(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			परत -1;
		}
		p += 8 + len;
	}

	मुक्त(data);

	जर (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	जर (recovery_eof <= recovery_head) {
		जर (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			परत -1;
		}
	}

	/* remove the recovery magic */
	जर (tdb_ofs_write(tdb, recovery_head + दुरत्व(संरचना list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	जर (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	जर (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		परत -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	परत 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
स्थिर पूर्णांक tdb_rec_free_read(संरचना tdb_context *tdb, tdb_off_t off, संरचना list_struct *rec)
{
	जर (tdb->methods->tdb_read(tdb, off, rec, आकार(*rec),DOCONV()) == -1)
		परत -1;

	जर (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		जर (tdb->methods->tdb_write(tdb, off, rec, आकार(*rec)) == -1)
			परत -1;
	}

	जर (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		परत TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	जर (tdb->methods->tdb_oob(tdb, rec->next+आकार(*rec), 0) != 0)
		परत -1;
	परत 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
स्थिर पूर्णांक remove_from_freelist(संरचना tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_शीर्ष;
	पर्यंत (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		जर (i == off) {
			/* We've found it! */
			परत tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	परत TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
स्थिर पूर्णांक update_tailer(संरचना tdb_context *tdb, tdb_off_t offset,
			 स्थिरांक संरचना list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = आकार(*rec) + rec->rec_len;
	परत tdb_ofs_write(tdb, offset + totalsize - आकार(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
पूर्णांक tdb_free(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec)
{
	tdb_off_t उजवा, डावा;

	/* Allocation and tailer lock */
	जर (tdb_lock(tdb, -1, F_WRLCK) != 0)
		परत -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	जर (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		जा fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	उजवा = offset + आकार(*rec) + rec->rec_len;
	जर (उजवा + आकार(*rec) <= tdb->map_size) {
		संरचना list_struct r;

		जर (tdb->methods->tdb_read(tdb, उजवा, &r, आकार(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", उजवा));
			जा डावा;
		}

		/* If it's free, expand to include it. */
		जर (r.magic == TDB_FREE_MAGIC) {
			जर (remove_from_freelist(tdb, उजवा, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", उजवा));
				जा डावा;
			}
			rec->rec_len += आकार(r) + r.rec_len;
		}
	}

डावा:
	/* Look left */
	डावा = offset - आकार(tdb_off_t);
	जर (डावा > TDB_DATA_START(tdb->header.hash_size)) {
		संरचना list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		जर (tdb_ofs_read(tdb, डावा, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", डावा));
			जा update;
		}

		/* it could be uninitialised data */
		जर (leftsize == 0 || leftsize == TDB_PAD_U32) {
			जा update;
		}

		डावा = offset - leftsize;

		/* Now read in record */
		जर (tdb->methods->tdb_read(tdb, डावा, &l, आकार(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", डावा, leftsize));
			जा update;
		}

		/* If it's free, expand to include it. */
		जर (l.magic == TDB_FREE_MAGIC) {
			जर (remove_from_freelist(tdb, डावा, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", डावा));
				जा update;
			} अन्यथा {
				offset = डावा;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	जर (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		जा fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	जर (tdb_ofs_read(tdb, FREELIST_शीर्ष, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_शीर्ष, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		जा fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	परत 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	परत -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
स्थिर tdb_off_t tdb_allocate_ofs(संरचना tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				संरचना list_struct *rec, tdb_off_t last_ptr)
{
	संरचना list_struct newrec;
	tdb_off_t newrec_ptr;

	स_रखो(&newrec, '\0', आकार(newrec));

	/* found it - now possibly split it up  */
	जर (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (आकार(*rec) + length);
		newrec_ptr = rec_ptr + आकार(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} अन्यथा {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	जर (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		परत 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	जर (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		परत 0;
	}

	/* Did we create new block? */
	जर (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		जर (update_tailer(tdb, rec_ptr, rec) == -1) {
			परत 0;
		}

		/* Free new record */
		जर (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			परत 0;
		}
	}

	/* all done - return the new record offset */
	परत rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(संरचना tdb_context *tdb, tdb_len_t length, संरचना list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	संरचना {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	जर (tdb_lock(tdb, -1, F_WRLCK) == -1)
		परत 0;

	/* Extra bytes required for tailer */
	length += आकार(tdb_off_t);

 again:
	last_ptr = FREELIST_शीर्ष;

	/* read in the freelist top */
	जर (tdb_ofs_read(tdb, FREELIST_शीर्ष, &rec_ptr) == -1)
		जा fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	पर्यंत (rec_ptr) {
		जर (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			जा fail;
		}

		जर (rec->rec_len >= length) {
			जर (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				जर (bestfit.rec_len < 2*length) {
					थांबा;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	जर (bestfit.rec_ptr != 0) {
		जर (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			जा fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		परत newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	जर (tdb_expand(tdb, length + आकार(*rec)) == 0)
		जा again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	परत 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

स्थिर पूर्णांक seen_घाला(संरचना tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA किल्ली, data;

	स_रखो(&data, '\0', आकार(data));
	किल्ली.dptr = (अचिन्हित अक्षर *)&rec_ptr;
	किल्ली.dsize = आकार(rec_ptr);
	परत tdb_store(mem_tdb, किल्ली, data, TDB_घाला);
}

पूर्णांक tdb_validate_freelist(संरचना tdb_context *tdb, पूर्णांक *pnum_entries)
{
	संरचना tdb_context *mem_tdb = रिक्त;
	संरचना list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	पूर्णांक ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	जर (!mem_tdb) {
		परत -1;
	}

	जर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		परत 0;
	}

	last_ptr = FREELIST_शीर्ष;

	/* Store the FREELIST_TOP record. */
	जर (seen_घाला(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		जा fail;
	}

	/* read in the freelist top */
	जर (tdb_ofs_read(tdb, FREELIST_शीर्ष, &rec_ptr) == -1) {
		जा fail;
	}

	पर्यंत (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		जर (seen_घाला(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			जा fail;
		}

		जर (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			जा fail;
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
	परत ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
स्थिर पूर्णांक tdb_next_lock(संरचना tdb_context *tdb, संरचना tdb_traverse_lock *tlock,
			 संरचना list_struct *rec)
{
	पूर्णांक want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	साठी (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		जर (!tlock->off && tlock->hash != 0) {
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
			जर (tlock->hash == tdb->header.hash_size) {
				सुरूठेवा;
			}
		}

		जर (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			परत -1;

		/* No previous record?  Start at top of chain. */
		जर (!tlock->off) {
			जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(tlock->hash),
				     &tlock->off) == -1)
				जा fail;
		} अन्यथा {
			/* Otherwise unlock the previous record. */
			जर (tdb_unlock_record(tdb, tlock->off) != 0)
				जा fail;
		}

		जर (want_next) {
			/* We have offset of old record: grab next */
			जर (tdb_rec_read(tdb, tlock->off, rec) == -1)
				जा fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		पर्यंत( tlock->off) {
			tdb_off_t current;
			जर (tdb_rec_read(tdb, tlock->off, rec) == -1)
				जा fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			जर (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				जा fail;
			}

			जर (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				जर (tdb_lock_record(tdb, tlock->off) != 0)
					जा fail;
				परत tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			जर (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				जा fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	परत TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	जर (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	परत -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
स्थिर पूर्णांक tdb_traverse_internal(संरचना tdb_context *tdb,
				 tdb_traverse_func fn, शून्य *private_data,
				 संरचना tdb_traverse_lock *tl)
{
	TDB_DATA किल्ली, dbuf;
	संरचना list_struct rec;
	पूर्णांक ret, मोजणी = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	पर्यंत ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		मोजणी++;
		/* now read the full record */
		किल्ली.dptr = tdb_alloc_read(tdb, tl->off + आकार(rec),
					  rec.किल्ली_len + rec.data_len);
		जर (!किल्ली.dptr) {
			ret = -1;
			जर (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				जा out;
			जर (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			जा out;
		}
		किल्ली.dsize = rec.किल्ली_len;
		dbuf.dptr = किल्ली.dptr + rec.किल्ली_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		जर (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(किल्ली.dptr);
			जा out;
		}
		जर (fn && fn(tdb, किल्ली, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = मोजणी;
			जर (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(किल्ली.dptr);
			जा out;
		}
		SAFE_FREE(किल्ली.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	जर (ret < 0)
		परत -1;
	अन्यथा
		परत मोजणी;
}


/*
  a write style traverse - temporarily marks the db read only
*/
पूर्णांक tdb_traverse_read(संरचना tdb_context *tdb,
		      tdb_traverse_func fn, शून्य *private_data)
{
	संरचना tdb_traverse_lock tl = { रिक्त, 0, 0, F_RDLCK };
	पूर्णांक ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	जर (tdb_transaction_lock(tdb, F_RDLCK)) {
		परत -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	परत ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
पूर्णांक tdb_traverse(संरचना tdb_context *tdb,
		 tdb_traverse_func fn, शून्य *private_data)
{
	संरचना tdb_traverse_lock tl = { रिक्त, 0, 0, F_WRLCK };
	पूर्णांक ret;

	जर (tdb->read_only || tdb->traverse_read) {
		परत tdb_traverse_read(tdb, fn, private_data);
	}

	जर (tdb_transaction_lock(tdb, F_WRLCK)) {
		परत -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	परत ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(संरचना tdb_context *tdb)
{
	TDB_DATA किल्ली;
	संरचना list_struct rec;

	/* release any old lock */
	जर (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		परत tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	जर (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		परत tdb_null;
	/* now read the key */
	किल्ली.dsize = rec.किल्ली_len;
	किल्ली.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+आकार(rec),किल्ली.dsize);

	/* Unlock the hash chain of the record we just read. */
	जर (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	परत किल्ली;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(संरचना tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA किल्ली = tdb_null;
	संरचना list_struct rec;
	अचिन्हित अक्षर *k = रिक्त;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	जर (tdb->travlocks.off) {
		जर (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			परत tdb_null;
		जर (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+आकार(rec),
					    rec.किल्ली_len))
		    || स_भेद(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			जर (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				परत tdb_null;
			}
			जर (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				परत tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	जर (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		जर (!tdb->travlocks.off)
			परत tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		जर (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", म_त्रुटि(त्रुटि_सं)));
			परत tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	जर (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		किल्ली.dsize = rec.किल्ली_len;
		किल्ली.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+आकार(rec),
					  किल्ली.dsize);
		/* Unlock the chain of this new record */
		जर (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	जर (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	परत किल्ली;
}

/* file: dump.c */

स्थिर tdb_off_t tdb_dump_record(संरचना tdb_context *tdb, पूर्णांक hash,
				 tdb_off_t offset)
{
	संरचना list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	जर (tdb->methods->tdb_read(tdb, offset, (अक्षर *)&rec,
				   आकार(rec), DOCONV()) == -1) {
		म_लिखो("ERROR: failed to read record at %u\n", offset);
		परत 0;
	}

	म_लिखो(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.किल्ली_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + आकार(rec) + rec.rec_len - आकार(tdb_off_t);

	जर (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		म_लिखो("ERROR: failed to read tailer at %u\n", tailer_ofs);
		परत rec.next;
	}

	जर (tailer != rec.rec_len + आकार(rec)) {
		म_लिखो("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(अचिन्हित पूर्णांक)tailer, (अचिन्हित पूर्णांक)(rec.rec_len + आकार(rec)));
	}
	परत rec.next;
}

स्थिर पूर्णांक tdb_dump_chain(संरचना tdb_context *tdb, पूर्णांक i)
{
	tdb_off_t rec_ptr, शीर्ष;

	शीर्ष = TDB_HASH_शीर्ष(i);

	जर (tdb_lock(tdb, i, F_WRLCK) != 0)
		परत -1;

	जर (tdb_ofs_read(tdb, शीर्ष, &rec_ptr) == -1)
		परत tdb_unlock(tdb, i, F_WRLCK);

	जर (rec_ptr)
		म_लिखो("hash=%d\n", i);

	पर्यंत (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	परत tdb_unlock(tdb, i, F_WRLCK);
}

शून्य tdb_dump_all(संरचना tdb_context *tdb)
{
	पूर्णांक i;
	साठी (i = 0; i < (पूर्णांक)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	म_लिखो("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

पूर्णांक tdb_printfreelist(संरचना tdb_context *tdb)
{
	पूर्णांक ret;
	दीर्घ total_free = 0;
	tdb_off_t offset, rec_ptr;
	संरचना list_struct rec;

	जर ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		परत ret;

	offset = FREELIST_शीर्ष;

	/* read in the freelist top */
	जर (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		परत 0;
	}

	म_लिखो("freelist top=[0x%08x]\n", rec_ptr );
	पर्यंत (rec_ptr) {
		जर (tdb->methods->tdb_read(tdb, rec_ptr, (अक्षर *)&rec,
					   आकार(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			परत -1;
		}

		जर (rec.magic != TDB_FREE_MAGIC) {
			म_लिखो("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			परत -1;
		}

		म_लिखो("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	म_लिखो("total rec_len = [0x%08x (%d)]\n", (पूर्णांक)total_free,
               (पूर्णांक)total_free);

	परत tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
शून्य tdb_increment_seqnum_nonblock(संरचना tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	जर (!(tdb->flags & TDB_SEQNUM)) {
		परत;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	जर (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		परत;
	seqnum++;
	(शून्य) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
स्थिर शून्य tdb_increment_seqnum(संरचना tdb_context *tdb)
{
	जर (!(tdb->flags & TDB_SEQNUM)) {
		परत;
	}

	जर (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		परत;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

स्थिर पूर्णांक tdb_किल्ली_compare(TDB_DATA किल्ली, TDB_DATA data,
			   शून्य *private_data EXT2FS_ATTR((unused)))
{
	परत स_भेद(data.dptr, किल्ली.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
स्थिर tdb_off_t tdb_find(संरचना tdb_context *tdb, TDB_DATA किल्ली, u32 hash,
			संरचना list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		परत 0;

	/* keep looking until we find the right record */
	पर्यंत (rec_ptr) {
		जर (tdb_rec_read(tdb, rec_ptr, r) == -1)
			परत 0;

		जर (!TDB_DEAD(r) && hash==r->full_hash
		    && किल्ली.dsize==r->किल्ली_len
		    && tdb_parse_data(tdb, किल्ली, rec_ptr + आकार(*r),
				      r->किल्ली_len, tdb_किल्ली_compare,
				      रिक्त) == 0) {
			परत rec_ptr;
		}
		rec_ptr = r->next;
	}
	परत TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(संरचना tdb_context *tdb, TDB_DATA किल्ली, u32 hash, पूर्णांक locktype,
			   संरचना list_struct *rec)
{
	u32 rec_ptr;

	जर (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		परत 0;
	जर (!(rec_ptr = tdb_find(tdb, किल्ली, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	परत rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
स्थिर पूर्णांक tdb_update_hash(संरचना tdb_context *tdb, TDB_DATA किल्ली, u32 hash, TDB_DATA dbuf)
{
	संरचना list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	जर (!(rec_ptr = tdb_find(tdb, किल्ली, hash, &rec)))
		परत -1;

	/* must be long enough key, data and tailer */
	जर (rec.rec_len < किल्ली.dsize + dbuf.dsize + आकार(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		परत -1;
	}

	जर (tdb->methods->tdb_write(tdb, rec_ptr + आकार(rec) + rec.किल्ली_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		परत -1;

	जर (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		परत tdb_rec_write(tdb, rec_ptr, &rec);
	}

	परत 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&किल्ली);
	जर (!(rec_ptr = tdb_find_lock_hash(tdb,किल्ली,hash,F_RDLCK,&rec)))
		परत tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + आकार(rec) + rec.किल्ली_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	परत ret;
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

पूर्णांक tdb_parse_record(संरचना tdb_context *tdb, TDB_DATA किल्ली,
		     पूर्णांक (*parser)(TDB_DATA किल्ली, TDB_DATA data,
				   शून्य *private_data),
		     शून्य *private_data)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	पूर्णांक ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&किल्ली);

	जर (!(rec_ptr = tdb_find_lock_hash(tdb,किल्ली,hash,F_RDLCK,&rec))) {
		परत TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, किल्ली, rec_ptr + आकार(rec) + rec.किल्ली_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	परत ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
स्थिर पूर्णांक tdb_exists_hash(संरचना tdb_context *tdb, TDB_DATA किल्ली, u32 hash)
{
	संरचना list_struct rec;

	जर (tdb_find_lock_hash(tdb, किल्ली, hash, F_RDLCK, &rec) == 0)
		परत 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	परत 1;
}

पूर्णांक tdb_exists(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	u32 hash = tdb->hash_fn(&किल्ली);
	परत tdb_exists_hash(tdb, किल्ली, hash);
}

/* actually delete an entry in the database given the offset */
पूर्णांक tdb_do_delete(संरचना tdb_context *tdb, tdb_off_t rec_ptr, संरचना list_struct*rec)
{
	tdb_off_t last_ptr, i;
	संरचना list_struct lastrec;

	जर (tdb->read_only || tdb->traverse_read) परत -1;

	जर (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		परत tdb_rec_write(tdb, rec_ptr, rec);
	}
	जर (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		परत -1;

	/* find previous record in hash chain */
	जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(rec->full_hash), &i) == -1)
		परत -1;
	साठी (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		जर (tdb_rec_read(tdb, i, &lastrec) == -1)
			परत -1;

	/* unlink it: next ptr is at start of record. */
	जर (last_ptr == 0)
		last_ptr = TDB_HASH_शीर्ष(rec->full_hash);
	जर (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		परत -1;

	/* recover the space */
	जर (tdb_free(tdb, rec_ptr, rec) == -1)
		परत -1;
	परत 0;
}

स्थिर पूर्णांक tdb_मोजणी_dead(संरचना tdb_context *tdb, u32 hash)
{
	पूर्णांक res = 0;
	tdb_off_t rec_ptr;
	संरचना list_struct rec;

	/* read in the hash top */
	जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		परत 0;

	पर्यंत (rec_ptr) {
		जर (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			परत 0;

		जर (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	परत res;
}

/*
 * Purge all DEAD records from a hash chain
 */
स्थिर पूर्णांक tdb_purge_dead(संरचना tdb_context *tdb, u32 hash)
{
	पूर्णांक res = -1;
	संरचना list_struct rec;
	tdb_off_t rec_ptr;

	जर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		परत -1;
	}

	/* read in the hash top */
	जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		जा fail;

	पर्यंत (rec_ptr) {
		tdb_off_t next;

		जर (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			जा fail;
		}

		next = rec.next;

		जर (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			जा fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	परत res;
}

/* delete an entry in the database given a key */
स्थिर पूर्णांक tdb_delete_hash(संरचना tdb_context *tdb, TDB_DATA किल्ली, u32 hash)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	पूर्णांक ret;

	जर (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		जर (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			परत -1;

		जर (tdb_मोजणी_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		जर (!(rec_ptr = tdb_find(tdb, किल्ली, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			परत -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	अन्यथा {
		जर (!(rec_ptr = tdb_find_lock_hash(tdb, किल्ली, hash, F_WRLCK,
						   &rec)))
			परत -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	जर (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	जर (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	परत ret;
}

पूर्णांक tdb_delete(संरचना tdb_context *tdb, TDB_DATA किल्ली)
{
	u32 hash = tdb->hash_fn(&किल्ली);
	परत tdb_delete_hash(tdb, किल्ली, hash);
}

/*
 * See if we have a dead record around with enough space
 */
स्थिर tdb_off_t tdb_find_dead(संरचना tdb_context *tdb, u32 hash,
			       संरचना list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		परत 0;

	/* keep looking until we find the right record */
	पर्यंत (rec_ptr) {
		जर (tdb_rec_read(tdb, rec_ptr, r) == -1)
			परत 0;

		जर (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			परत rec_ptr;
		}
		rec_ptr = r->next;
	}
	परत 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
पूर्णांक tdb_store(संरचना tdb_context *tdb, TDB_DATA किल्ली, TDB_DATA dbuf, पूर्णांक flag)
{
	संरचना list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	अक्षर *p = रिक्त;
	पूर्णांक ret = -1;

	जर (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		परत -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&किल्ली);
	जर (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		परत -1;

	/* check for it existing, on insert. */
	जर (flag == TDB_घाला) {
		जर (tdb_exists_hash(tdb, किल्ली, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			जा fail;
		}
	} अन्यथा {
		/* first try in-place update, on modify or replace. */
		जर (tdb_update_hash(tdb, किल्ली, hash, dbuf) == 0) {
			जा done;
		}
		जर (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			जा fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	जर (flag != TDB_घाला)
		tdb_delete_hash(tdb, किल्ली, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	जर (!(p = (अक्षर *)दो_स्मृति(किल्ली.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		जा fail;
	}

	स_नकल(p, किल्ली.dptr, किल्ली.dsize);
	जर (dbuf.dsize)
		स_नकल(p+किल्ली.dsize, dbuf.dptr, dbuf.dsize);

	जर (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			किल्ली.dsize + dbuf.dsize + आकार(tdb_off_t));

		जर (rec_ptr != 0) {
			rec.किल्ली_len = किल्ली.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			जर (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + आकार(rec),
				    p, किल्ली.dsize + dbuf.dsize) == -1) {
				जा fail;
			}
			जा done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	जर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		जा fail;
	}

	जर ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		जा fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, किल्ली.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	जर (rec_ptr == 0) {
		जा fail;
	}

	/* Read hash top into next ptr */
	जर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec.next) == -1)
		जा fail;

	rec.किल्ली_len = किल्ली.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	जर (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+आकार(rec), p, किल्ली.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		जा fail;
	}

 done:
	ret = 0;
 fail:
	जर (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	परत ret;
}


/* Append to an entry. Create if not exist. */
पूर्णांक tdb_append(संरचना tdb_context *tdb, TDB_DATA किल्ली, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	पूर्णांक ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&किल्ली);
	जर (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		परत -1;

	dbuf = tdb_fetch(tdb, किल्ली);

	जर (dbuf.dptr == रिक्त) {
		dbuf.dptr = (अचिन्हित अक्षर *)दो_स्मृति(new_dbuf.dsize);
	} अन्यथा {
		अचिन्हित अक्षर *new_dptr = (अचिन्हित अक्षर *)पुनः_स्मृति(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		जर (new_dptr == रिक्त) {
			मुक्त(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	जर (dbuf.dptr == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		जा failed;
	}

	स_नकल(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, किल्ली, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	परत ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
स्थिरांक अक्षर *tdb_name(संरचना tdb_context *tdb)
{
	परत tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
पूर्णांक tdb_fd(संरचना tdb_context *tdb)
{
	परत tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(संरचना tdb_context *tdb)
{
	परत tdb->log.log_fn;
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
पूर्णांक tdb_get_seqnum(संरचना tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	जर (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		परत 0;
	परत seqnum;
}

पूर्णांक tdb_hash_size(संरचना tdb_context *tdb)
{
	परत tdb->header.hash_size;
}

माप_प्रकार tdb_map_size(संरचना tdb_context *tdb)
{
	परत tdb->map_size;
}

पूर्णांक tdb_get_flags(संरचना tdb_context *tdb)
{
	परत tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
शून्य tdb_enable_seqnum(संरचना tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
स्थिर संरचना tdb_context *tdbs = रिक्त;


/* This is from a hash algorithm suggested by Rogier Wolff */
स्थिर अचिन्हित पूर्णांक default_tdb_hash(TDB_DATA *किल्ली)
{
	u32 मूल्य;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	साठी (मूल्य = 0, i=0; i < किल्ली->dsize; i++)
		मूल्य = मूल्य * 256 + किल्ली->dptr[i] + (मूल्य >> 24) * 241;

	परत मूल्य;
}


/* initialise a new database with a specified hash size */
स्थिर पूर्णांक tdb_new_database(संरचना tdb_context *tdb, पूर्णांक hash_size)
{
	संरचना tdb_header *newdb;
	पूर्णांक size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = आकार(संरचना tdb_header) + (hash_size+1)*आकार(tdb_off_t);
	जर (!(newdb = (संरचना tdb_header *)सुस्मृति(size, 1)))
		परत TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	जर (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (अक्षर *)newdb;
		स_नकल(&tdb->header, newdb, आकार(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		परत 0;
	}
	जर (lseek(tdb->fd, 0, शुरू_से) == -1)
		जा fail;

	जर (ftruncate(tdb->fd, 0) == -1)
		जा fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	स_नकल(&tdb->header, newdb, आकार(tdb->header));
	/* Don't endian-convert the magic food! */
	स_नकल(newdb->magic_food, TDB_MAGIC_FOOD, म_माप(TDB_MAGIC_FOOD)+1);
	जर (लिख(tdb->fd, newdb, size) != size) {
		ret = -1;
	} अन्यथा {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	परत ret;
}



स्थिर पूर्णांक tdb_already_open(dev_t device,
			    ino_t ino)
{
	संरचना tdb_context *i;

	साठी (i = tdbs; i; i = i->next) {
		जर (i->device == device && i->inode == ino) {
			परत 1;
		}
	}

	परत 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
संरचना tdb_context *tdb_open(स्थिरांक अक्षर *name, पूर्णांक hash_size, पूर्णांक tdb_flags,
		      पूर्णांक open_flags, mode_t mode)
{
	परत tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, रिक्त, रिक्त);
}

/* a default logging function */
स्थिर शून्य null_log_fn(संरचना tdb_context *tdb, गणना tdb_debug_level level, स्थिरांक अक्षर *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
स्थिर शून्य null_log_fn(संरचना tdb_context *tdb EXT2FS_ATTR((unused)),
			गणना tdb_debug_level level EXT2FS_ATTR((unused)),
			स्थिरांक अक्षर *fmt EXT2FS_ATTR((unused)), ...)
{
}


संरचना tdb_context *tdb_open_ex(स्थिरांक अक्षर *name, पूर्णांक hash_size, पूर्णांक tdb_flags,
				पूर्णांक open_flags, mode_t mode,
				स्थिरांक संरचना tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	संरचना tdb_context *tdb;
	संरचना stat st;
	पूर्णांक rev = 0, locked = 0;
	अचिन्हित अक्षर *vp;
	u32 vertest;

	जर (!(tdb = (संरचना tdb_context *)सुस्मृति(1, आकार *tdb))) {
		/* Can't log this */
		त्रुटि_सं = ENOMEM;
		जा fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = रिक्त;
	tdb->map_ptr = रिक्त;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	जर (log_ctx) {
		tdb->log = *log_ctx;
	} अन्यथा {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = रिक्त;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	जर (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	जर ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		त्रुटि_सं = EINVAL;
		जा fail;
	}

	जर (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	जर ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	जर (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		जर (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			जा fail;
		}
		जा internal;
	}

	जर ((tdb->fd = खोलो(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, म_त्रुटि(त्रुटि_सं)));
		जा fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	जर (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, म_त्रुटि(त्रुटि_सं)));
		जा fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	जर ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		जर (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, म_त्रुटि(त्रुटि_सं)));
			जा fail; /* errno set by ftruncate */
		}
	}

	जर (पठ(tdb->fd, &tdb->header, आकार(tdb->header)) != आकार(tdb->header)
	    || स_भेद(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      आकार(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		जर (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			त्रुटि_सं = EIO; /* ie bad format or something */
			जा fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (अचिन्हित अक्षर *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	जर (!rev)
		tdb->flags &= ~TDB_CONVERT;
	अन्यथा {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, आकार(tdb->header));
	}
	जर (ख_स्थिति(tdb->fd, &st) == -1)
		जा fail;

	जर (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		जा fail;
	}

	/* Is it already in the open list?  If so, fail. */
	जर (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (पूर्णांक)st.st_dev, (पूर्णांक)st.st_ino));
		त्रुटि_सं = EBUSY;
		जा fail;
	}

	जर (!(tdb->name = (अक्षर *)strdup(name))) {
		त्रुटि_सं = ENOMEM;
		जा fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	जर (locked) {
		जर (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, म_त्रुटि(त्रुटि_सं)));
			जा fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	जर (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		जर (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			जा fail;
	}

	/* if needed, run recovery */
	जर (tdb_transaction_recover(tdb) == -1) {
		जा fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	जर (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		जा fail;
	tdb->next = tdbs;
	tdbs = tdb;
	परत tdb;

 fail:
	{ पूर्णांक save_errno = त्रुटि_सं;

	जर (!tdb)
		परत रिक्त;

	जर (tdb->map_ptr) {
		जर (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		अन्यथा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	जर (tdb->fd != -1)
		जर (बंद(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	त्रुटि_सं = save_errno;
	परत रिक्त;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

शून्य tdb_set_max_dead(संरचना tdb_context *tdb, पूर्णांक max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
पूर्णांक tdb_close(संरचना tdb_context *tdb)
{
	संरचना tdb_context **i;
	पूर्णांक ret = 0;

	जर (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	जर (tdb->map_ptr) {
		जर (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		अन्यथा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	जर (tdb->fd != -1)
		ret = बंद(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	साठी (i = &tdbs; *i; i = &(*i)->next) {
		जर (*i == tdb) {
			*i = tdb->next;
			थांबा;
		}
	}

	स_रखो(tdb, 0, आकार(*tdb));
	SAFE_FREE(tdb);

	परत ret;
}

/* register a logging function */
शून्य tdb_set_logging_function(संरचना tdb_context *tdb,
                              स्थिरांक संरचना tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

शून्य *tdb_get_logging_private(संरचना tdb_context *tdb)
{
	परत tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
पूर्णांक tdb_reopen(संरचना tdb_context *tdb)
{
	संरचना stat st;

	जर (tdb->flags & TDB_INTERNAL) {
		परत 0; /* Nothing to do. */
	}

	जर (tdb->अंक_locks != 0 || tdb->global_lock.मोजणी) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		जा fail;
	}

	जर (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		जा fail;
	}

	जर (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		जा fail;
	}
	जर (बंद(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = खोलो(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	जर (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		जा fail;
	}
	जर ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		जा fail;
	}
	जर (ख_स्थिति(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		जा fail;
	}
	जर (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		जा fail;
	}
	tdb_mmap(tdb);

	परत 0;

fail:
	tdb_close(tdb);
	परत -1;
}

/* reopen all tdb's */
पूर्णांक tdb_reopen_all(पूर्णांक parent_longlived)
{
	संरचना tdb_context *tdb;

	साठी (tdb=tdbs; tdb; tdb = tdb->next) {
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
		जर (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		जर (tdb_reopen(tdb) != 0)
			परत -1;
	}

	परत 0;
}

/**
 * Flush a database file from the page cache.
 **/
पूर्णांक tdb_flush(संरचना tdb_context *tdb)
{
	जर (tdb->fd != -1)
		परत fsync(tdb->fd);
	परत 0;
}
