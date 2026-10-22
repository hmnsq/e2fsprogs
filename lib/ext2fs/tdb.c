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
#अण्णहा
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
स्थिर वर्ण *rep_strdup(ध्रुव वर्ण *s)
{
	वर्ण *ret;
	पूर्णाङ्क length;

	जइ (!s)
		प्रत्यागम रिक्त;
	length = strlen(s);
	ret = malloc(length + 1);
	जइ (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	प्रत्यागम ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#जइ (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#अण्णहा
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#जइ defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
प्रकारनिर्देश पूर्णाङ्क bool;
#endif

#अन्तर्भाव "tdb.h"

स्थिर TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

प्रकारनिर्देश u32 tdb_len_t;
प्रकारनिर्देश u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned पूर्णाङ्क)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*परिमाण(संरचना list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_शिखरम् (परिमाण(संरचना tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_शिखरम्(hash) (FREELIST_शिखरम् + (BUCKET(hash)+1)*परिमाण(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*परिमाण(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_शिखरम्(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(संरचना tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(संरचना tdb_header, sequence_सङ्ख्या)
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
#define SAFE_FREE(x) करेहि { जइ ((x) != रिक्त) {मुक्त(x); (x)=रिक्त;} } जाव(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, परिमाण(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
संरचना list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t कुञ्चिका_len; /* byte length of key */
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
	वर्ण magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_सङ्ख्या; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

संरचना tdb_lock_type {
	पूर्णाङ्क list;
	u32 गणनम्;
	u32 ltype;
};

संरचना tdb_traverse_lock {
	संरचना tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	पूर्णाङ्क lock_rw;
};


संरचना tdb_methods {
	पूर्णाङ्क (*tdb_read)(संरचना tdb_context *, tdb_off_t , शून्य *, tdb_len_t , पूर्णाङ्क );
	पूर्णाङ्क (*tdb_write)(संरचना tdb_context *, tdb_off_t, ध्रुव शून्य *, tdb_len_t);
	शून्य (*next_hash_chain)(संरचना tdb_context *, u32 *);
	पूर्णाङ्क (*tdb_oob)(संरचना tdb_context *, tdb_off_t , पूर्णाङ्क );
	पूर्णाङ्क (*tdb_expand_file)(संरचना tdb_context *, tdb_off_t , tdb_off_t );
	पूर्णाङ्क (*tdb_brlock)(संरचना tdb_context *, tdb_off_t , पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क, size_t);
};

संरचना tdb_context {
	वर्ण *name; /* the name of the database */
	शून्य *map_ptr; /* where it is currently mapped */
	पूर्णाङ्क fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	पूर्णाङ्क read_only; /* opened read-only */
	पूर्णाङ्क traverse_read; /* read-only traversal */
	संरचना tdb_lock_type global_lock;
	पूर्णाङ्क अङ्कः_lockrecs;
	संरचना tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	गणना TDB_ERROR ecode; /* error code for last tdb error */
	संरचना tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	संरचना tdb_traverse_lock travlocks; /* current traversal locks */
	संरचना tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	संरचना tdb_logging_context log;
	unsigned पूर्णाङ्क (*hash_fn)(TDB_DATA *कुञ्चिका);
	पूर्णाङ्क open_flags; /* flags used in the open - needed by reopen */
	unsigned पूर्णाङ्क अङ्कः_locks; /* number of chain locks held */
	ध्रुव संरचना tdb_methods *methods;
	संरचना tdb_transaction *transaction;
	पूर्णाङ्क page_size;
	पूर्णाङ्क max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
स्थिर पूर्णाङ्क tdb_munmap(संरचना tdb_context *tdb);
स्थिर शून्य tdb_mmap(संरचना tdb_context *tdb);
स्थिर पूर्णाङ्क tdb_lock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype);
पूर्णाङ्क tdb_lock_nonblock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype);
स्थिर पूर्णाङ्क tdb_unlock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype);
स्थिर पूर्णाङ्क tdb_brlock(संरचना tdb_context *tdb, tdb_off_t offset, पूर्णाङ्क rw_type, पूर्णाङ्क lck_type, पूर्णाङ्क probe, size_t len);
स्थिर पूर्णाङ्क tdb_transaction_lock(संरचना tdb_context *tdb, पूर्णाङ्क ltype);
स्थिर पूर्णाङ्क tdb_transaction_unlock(संरचना tdb_context *tdb);
स्थिर पूर्णाङ्क tdb_brlock_upgrade(संरचना tdb_context *tdb, tdb_off_t offset, size_t len);
स्थिर पूर्णाङ्क tdb_write_lock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_write_unlock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर पूर्णाङ्क tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर शून्य *tdb_convert(शून्य *buf, u32 size);
स्थिर पूर्णाङ्क tdb_free(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec);
स्थिर tdb_off_t tdb_allocate(संरचना tdb_context *tdb, tdb_len_t length, संरचना list_struct *rec);
स्थिर पूर्णाङ्क tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर पूर्णाङ्क tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
स्थिर पूर्णाङ्क tdb_lock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_unlock_record(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_rec_read(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec);
स्थिर पूर्णाङ्क tdb_rec_write(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec);
स्थिर पूर्णाङ्क tdb_do_delete(संरचना tdb_context *tdb, tdb_off_t rec_ptr, संरचना list_struct *rec);
स्थिर unsigned वर्ण *tdb_alloc_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
स्थिर पूर्णाङ्क tdb_parse_data(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका,
		   tdb_off_t offset, tdb_len_t len,
		   पूर्णाङ्क (*parser)(TDB_DATA कुञ्चिका, TDB_DATA data,
				 शून्य *private_data),
		   शून्य *private_data);
स्थिर tdb_off_t tdb_find_lock_hash(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, u32 hash, पूर्णाङ्क locktype,
			   संरचना list_struct *rec);
स्थिर शून्य tdb_io_init(संरचना tdb_context *tdb);
स्थिर पूर्णाङ्क tdb_expand(संरचना tdb_context *tdb, tdb_off_t size);
स्थिर पूर्णाङ्क tdb_rec_free_read(संरचना tdb_context *tdb, tdb_off_t off,
		      संरचना list_struct *rec);


/* file: error.c */

गणना TDB_ERROR tdb_error(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->ecode;
}

स्थिर संरचना tdb_errname {
	गणना TDB_ERROR ecode; ध्रुव वर्ण *estring;
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
ध्रुव वर्ण *tdb_errorstr(संरचना tdb_context *tdb)
{
	u32 i;
	प्रति (i = 0; i < परिमाण(emap) / परिमाण(संरचना tdb_errname); i++)
		जइ (tdb->ecode == emap[i].ecode)
			प्रत्यागम emap[i].estring;
	प्रत्यागम "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
पूर्णाङ्क tdb_brlock(संरचना tdb_context *tdb, tdb_off_t offset,
	       पूर्णाङ्क rw_type, पूर्णाङ्क lck_type, पूर्णाङ्क probe, size_t len)
{
	संरचना flock fl;
	पूर्णाङ्क ret;

	जइ (tdb->flags & TDB_NOLOCK) {
		प्रत्यागम 0;
	}

	जइ ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	करेहि {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} जाव (ret == -1 && errno == EINTR);

	जइ (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		जइ (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (पूर्णाङ्क)len));
		}
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	प्रत्यागम 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
पूर्णाङ्क tdb_brlock_upgrade(संरचना tdb_context *tdb, tdb_off_t offset, size_t len)
{
	पूर्णाङ्क गणनम् = 1000;
	जाव (गणनम्--) {
		संरचना timeval tv;
		जइ (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			प्रत्यागम 0;
		}
		जइ (errno != EDEADLK) {
			भङ्ग;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, रिक्त, रिक्त, रिक्त, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	प्रत्यागम -1;
}


/* lock a list in the database. list -1 is the alloc list */
स्थिर पूर्णाङ्क _tdb_lock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype, पूर्णाङ्क op)
{
	संरचना tdb_lock_type *new_lck;
	पूर्णाङ्क i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	जइ (tdb->global_lock.गणनम् &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		प्रत्यागम 0;
	}

	जइ (tdb->global_lock.गणनम्) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जइ (list < -1 || list >= (पूर्णाङ्क)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		प्रत्यागम -1;
	}
	जइ (tdb->flags & TDB_NOLOCK)
		प्रत्यागम 0;

	प्रति (i=0; i<tdb->अङ्कः_lockrecs; i++) {
		जइ (tdb->lockrecs[i].list == list) {
			जइ (tdb->lockrecs[i].गणनम् == 0) {
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
			tdb->lockrecs[i].गणनम्++;
			प्रत्यागम 0;
		}
	}

	new_lck = (संरचना tdb_lock_type *)realloc(
		tdb->lockrecs,
		परिमाण(*tdb->lockrecs) * (tdb->अङ्कः_lockrecs+1));
	जइ (new_lck == रिक्त) {
		errno = ENOMEM;
		प्रत्यागम -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	जइ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_शिखरम्+4*list, ltype, op,
				     0, 1)) {
		प्रत्यागम -1;
	}

	tdb->अङ्कः_locks++;

	tdb->lockrecs[tdb->अङ्कः_lockrecs].list = list;
	tdb->lockrecs[tdb->अङ्कः_lockrecs].गणनम् = 1;
	tdb->lockrecs[tdb->अङ्कः_lockrecs].ltype = ltype;
	tdb->अङ्कः_lockrecs += 1;

	प्रत्यागम 0;
}

/* lock a list in the database. list -1 is the alloc list */
पूर्णाङ्क tdb_lock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype)
{
	पूर्णाङ्क ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	जइ (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	प्रत्यागम ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
पूर्णाङ्क tdb_lock_nonblock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype)
{
	प्रत्यागम _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
पूर्णाङ्क tdb_unlock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क i;
	संरचना tdb_lock_type *lck = रिक्त;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	जइ (tdb->global_lock.गणनम् &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		प्रत्यागम 0;
	}

	जइ (tdb->global_lock.गणनम्) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जइ (tdb->flags & TDB_NOLOCK)
		प्रत्यागम 0;

	/* Sanity checks */
	जइ (list < -1 || list >= (पूर्णाङ्क)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		प्रत्यागम ret;
	}

	प्रति (i=0; i<tdb->अङ्कः_lockrecs; i++) {
		जइ (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			भङ्ग;
		}
	}

	जइ ((lck == रिक्त) || (lck->गणनम् == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		प्रत्यागम -1;
	}

	जइ (lck->गणनम् > 1) {
		lck->गणनम्--;
		प्रत्यागम 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	जइ (mark_lock) {
		ret = 0;
	} अण्णहा {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_शिखरम्+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->अङ्कः_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	जइ (tdb->अङ्कः_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->अङ्कः_lockrecs-1];
	}
	tdb->अङ्कः_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	जइ (tdb->अङ्कः_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	जइ (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	प्रत्यागम ret;
}

/*
  get the transaction lock
 */
पूर्णाङ्क tdb_transaction_lock(संरचना tdb_context *tdb, पूर्णाङ्क ltype)
{
	जइ (tdb->have_transaction_lock || tdb->global_lock.गणनम्) {
		प्रत्यागम 0;
	}
	जइ (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		प्रत्यागम -1;
	}
	tdb->have_transaction_lock = 1;
	प्रत्यागम 0;
}

/*
  release the transaction lock
 */
पूर्णाङ्क tdb_transaction_unlock(संरचना tdb_context *tdb)
{
	पूर्णाङ्क ret;
	जइ (!tdb->have_transaction_lock) {
		प्रत्यागम 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	जइ (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	प्रत्यागम ret;
}




/* lock/unlock entire database */
स्थिर पूर्णाङ्क _tdb_lockall(संरचना tdb_context *tdb, पूर्णाङ्क ltype, पूर्णाङ्क op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	जइ (tdb->read_only || tdb->traverse_read)
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);

	जइ (tdb->global_lock.गणनम् && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.गणनम्++;
		प्रत्यागम 0;
	}

	जइ (tdb->global_lock.गणनम्) {
		/* a global lock of a different type exists */
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जइ (tdb->अङ्कः_locks != 0) {
		/* can't combine global and chain locks */
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जइ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शिखरम्, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		जइ (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		प्रत्यागम -1;
	}

	tdb->global_lock.गणनम् = 1;
	tdb->global_lock.ltype = ltype;

	प्रत्यागम 0;
}



/* unlock entire db */
स्थिर पूर्णाङ्क _tdb_unlockall(संरचना tdb_context *tdb, पूर्णाङ्क ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	जइ (tdb->read_only || tdb->traverse_read) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जइ (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.गणनम् == 0) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	जइ (tdb->global_lock.गणनम् > 1) {
		tdb->global_lock.गणनम्--;
		प्रत्यागम 0;
	}

	जइ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शिखरम्, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		प्रत्यागम -1;
	}

	tdb->global_lock.गणनम् = 0;
	tdb->global_lock.ltype = 0;

	प्रत्यागम 0;
}

/* lock entire database with write lock */
पूर्णाङ्क tdb_lockall(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
पूर्णाङ्क tdb_lockall_mark(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
पूर्णाङ्क tdb_lockall_unmark(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
पूर्णाङ्क tdb_lockall_nonblock(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
पूर्णाङ्क tdb_unlockall(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
पूर्णाङ्क tdb_lockall_read(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
पूर्णाङ्क tdb_lockall_read_nonblock(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
पूर्णाङ्क tdb_unlockall_read(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
पूर्णाङ्क tdb_chainlock(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_lock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
पूर्णाङ्क tdb_chainlock_nonblock(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णाङ्क tdb_chainlock_mark(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_lock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णाङ्क tdb_chainlock_unmark(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK | TDB_MARK_LOCK);
}

पूर्णाङ्क tdb_chainunlock(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK);
}

पूर्णाङ्क tdb_chainlock_read(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_lock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_RDLCK);
}

पूर्णाङ्क tdb_chainunlock_read(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	प्रत्यागम tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_RDLCK);
}



/* record lock stops delete underneath */
पूर्णाङ्क tdb_lock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	प्रत्यागम off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णाङ्क tdb_write_lock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	संरचना tdb_traverse_lock *i;
	प्रति (i = &tdb->travlocks; i; i = i->next)
		जइ (i->off == off)
			प्रत्यागम -1;
	प्रत्यागम tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णाङ्क tdb_write_unlock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	प्रत्यागम tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
पूर्णाङ्क tdb_unlock_record(संरचना tdb_context *tdb, tdb_off_t off)
{
	संरचना tdb_traverse_lock *i;
	u32 गणनम् = 0;

	जइ (off == 0)
		प्रत्यागम 0;
	प्रति (i = &tdb->travlocks; i; i = i->next)
		जइ (i->off == off)
			गणनम्++;
	प्रत्यागम (गणनम् == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
स्थिर पूर्णाङ्क tdb_oob(संरचना tdb_context *tdb, tdb_off_t len, पूर्णाङ्क probe)
{
	संरचना stat st;
	जइ (len <= tdb->map_size)
		प्रत्यागम 0;
	जइ (tdb->flags & TDB_INTERNAL) {
		जइ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (पूर्णाङ्क)len, (पूर्णाङ्क)tdb->map_size));
		}
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	जइ (fstat(tdb->fd, &st) == -1) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	जइ (st.st_size < (off_t)len) {
		जइ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (पूर्णाङ्क)len, (पूर्णाङ्क)st.st_size));
		}
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	जइ (tdb_munmap(tdb) == -1)
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	प्रत्यागम 0;
}

/* write a lump of data at a specified offset */
स्थिर पूर्णाङ्क tdb_write(संरचना tdb_context *tdb, tdb_off_t off,
		     ध्रुव शून्य *buf, tdb_len_t len)
{
	जइ (len == 0) {
		प्रत्यागम 0;
	}

	जइ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	जइ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		प्रत्यागम -1;

	जइ (tdb->map_ptr) {
		memcpy(off + (वर्ण *)tdb->map_ptr, buf, len);
	} अण्णहा जइ (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	प्रत्यागम 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
शून्य *tdb_convert(शून्य *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	प्रति (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	प्रत्यागम buf;
}


/* read a lump of data at a specified offset, maybe convert */
स्थिर पूर्णाङ्क tdb_read(संरचना tdb_context *tdb, tdb_off_t off, शून्य *buf,
		    tdb_len_t len, पूर्णाङ्क cv)
{
	जइ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		प्रत्यागम -1;
	}

	जइ (tdb->map_ptr) {
		memcpy(buf, off + (वर्ण *)tdb->map_ptr, len);
	} अण्णहा {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		जइ (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (पूर्णाङ्क)off, (पूर्णाङ्क)len, (पूर्णाङ्क)ret, strerror(errno),
				 (पूर्णाङ्क)tdb->map_size));
			प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	जइ (cv) {
		tdb_convert(buf, len);
	}
	प्रत्यागम 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
स्थिर शून्य tdb_next_hash_chain(संरचना tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	जइ (tdb->map_ptr) {
		प्रति (;h < tdb->header.hash_size;h++) {
			जइ (0 != *(u32 *)(TDB_HASH_शिखरम्(h) + (unsigned वर्ण *)tdb->map_ptr)) {
				भङ्ग;
			}
		}
	} अण्णहा {
		u32 off=0;
		प्रति (;h < tdb->header.hash_size;h++) {
			जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(h), &off) != 0 || off != 0) {
				भङ्ग;
			}
		}
	}
	(*chain) = h;
}


पूर्णाङ्क tdb_munmap(संरचना tdb_context *tdb)
{
	जइ (tdb->flags & TDB_INTERNAL)
		प्रत्यागम 0;

#ifdef HAVE_MMAP
	जइ (tdb->map_ptr) {
		पूर्णाङ्क ret = munmap(tdb->map_ptr, tdb->real_map_size);
		जइ (ret != 0)
			प्रत्यागम ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = रिक्त;
	प्रत्यागम 0;
}

शून्य tdb_mmap(संरचना tdb_context *tdb)
{
	जइ (tdb->flags & TDB_INTERNAL)
		प्रत्यागम;

#ifdef HAVE_MMAP
	जइ (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(रिक्त, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		जइ (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = रिक्त;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} अण्णहा {
		tdb->map_ptr = रिक्त;
	}
#अण्णहा
	tdb->map_ptr = रिक्त;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
स्थिर पूर्णाङ्क tdb_expand_file(संरचना tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	वर्ण buf[1024];

	जइ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	जइ (ftruncate(tdb->fd, size+addition) == -1) {
		वर्ण b = 0;
		जइ (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			प्रत्यागम -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, परिमाण(buf));
	जाव (addition) {
		पूर्णाङ्क n = addition>परिमाण(buf)?परिमाण(buf):addition;
		पूर्णाङ्क ret = pwrite(tdb->fd, buf, n, size);
		जइ (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			प्रत्यागम -1;
		}
		addition -= n;
		size += n;
	}
	प्रत्यागम 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
पूर्णाङ्क tdb_expand(संरचना tdb_context *tdb, tdb_off_t size)
{
	संरचना list_struct rec;
	tdb_off_t offset;

	जइ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		प्रत्यागम -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	जइ (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	जइ (!(tdb->flags & TDB_INTERNAL)) {
		जइ (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			गच्छ fail;
	}

	tdb->map_size += size;

	जइ (tdb->flags & TDB_INTERNAL) {
		वर्ण *new_map_ptr = (वर्ण *)realloc(tdb->map_ptr,
						    tdb->map_size);
		जइ (!new_map_ptr) {
			tdb->map_size -= size;
			गच्छ fail;
		}
		tdb->map_ptr = new_map_ptr;
	} अण्णहा {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',परिमाण(rec));
	rec.rec_len = size - परिमाण(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	जइ (tdb_free(tdb, offset, &rec) == -1)
		गच्छ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम -1;
}

/* read/write a tdb_off_t */
पूर्णाङ्क tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	प्रत्यागम tdb->methods->tdb_read(tdb, offset, (वर्ण*)d, परिमाण(*d), DOCONV());
}

पूर्णाङ्क tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	प्रत्यागम tdb->methods->tdb_write(tdb, offset, CONVERT(off), परिमाण(*d));
}


/* read a lump of data, allocating the space for it */
unsigned वर्ण *tdb_alloc_read(संरचना tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned वर्ण *buf;

	/* some systems don't like zero length malloc */
	जइ (len == 0) {
		len = 1;
	}

	जइ (!(buf = (unsigned वर्ण *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	जइ (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		प्रत्यागम रिक्त;
	}
	प्रत्यागम buf;
}

/* Give a piece of tdb data to a parser */

पूर्णाङ्क tdb_parse_data(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका,
		   tdb_off_t offset, tdb_len_t len,
		   पूर्णाङ्क (*parser)(TDB_DATA कुञ्चिका, TDB_DATA data,
				 शून्य *private_data),
		   शून्य *private_data)
{
	TDB_DATA data;
	पूर्णाङ्क result;

	data.dsize = len;

	जइ ((tdb->transaction == रिक्त) && (tdb->map_ptr != रिक्त)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		जइ (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			प्रत्यागम -1;
		}
		data.dptr = offset + (unsigned वर्ण *)tdb->map_ptr;
		प्रत्यागम parser(कुञ्चिका, data, private_data);
	}

	जइ (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		प्रत्यागम -1;
	}

	result = parser(कुञ्चिका, data, private_data);
	मुक्त(data.dptr);
	प्रत्यागम result;
}

/* read/write a record */
पूर्णाङ्क tdb_rec_read(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec)
{
	जइ (tdb->methods->tdb_read(tdb, offset, rec, परिमाण(*rec),DOCONV()) == -1)
		प्रत्यागम -1;
	जइ (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	प्रत्यागम tdb->methods->tdb_oob(tdb, rec->next+परिमाण(*rec), 0);
}

पूर्णाङ्क tdb_rec_write(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec)
{
	संरचना list_struct r = *rec;
	प्रत्यागम tdb->methods->tdb_write(tdb, offset, CONVERT(r), परिमाण(r));
}

स्थिर ध्रुव संरचना tdb_methods io_methods = {
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
	unsigned वर्ण *data;
};

/*
  hold the context of any current transaction
*/
संरचना tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ध्रुव संरचना tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	संरचना tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	पूर्णाङ्क transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	पूर्णाङ्क nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
स्थिर पूर्णाङ्क transaction_read(संरचना tdb_context *tdb, tdb_off_t off, शून्य *buf,
			    tdb_len_t len, पूर्णाङ्क cv)
{
	संरचना tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	प्रति (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		जइ (off+len <= el->offset) {
			अनुवर्त;
		}
		जइ (off >= el->offset + el->length) {
			अनुवर्त;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		जइ (off < el->offset) {
			partial = el->offset - off;
			जइ (transaction_read(tdb, off, buf, partial, cv) != 0) {
				गच्छ fail;
			}
			len -= partial;
			off += partial;
			buf = (शून्य *)(partial + (वर्ण *)buf);
		}
		जइ (off + len <= el->offset + el->length) {
			partial = len;
		} अण्णहा {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		जइ (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (शून्य *)(partial + (वर्ण *)buf);

		जइ (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			गच्छ fail;
		}

		प्रत्यागम 0;
	}

	/* its not in the transaction elements - do a real read */
	प्रत्यागम tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	प्रत्यागम -1;
}


/*
  write while in a transaction
*/
स्थिर पूर्णाङ्क transaction_write(संरचना tdb_context *tdb, tdb_off_t off,
			     ध्रुव शून्य *buf, tdb_len_t len)
{
	संरचना tdb_transaction_el *el, *best_el=रिक्त;

	जइ (len == 0) {
		प्रत्यागम 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	जइ (len == परिमाण(tdb_off_t) && off >= FREELIST_शिखरम् &&
	    off < FREELIST_शिखरम्+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_शिखरम्) / परिमाण(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	प्रति (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		जइ (best_el == रिक्त && off == el->offset+el->length) {
			best_el = el;
		}

		जइ (off+len <= el->offset) {
			अनुवर्त;
		}
		जइ (off >= el->offset + el->length) {
			अनुवर्त;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		जइ (off < el->offset) {
			partial = el->offset - off;
			जइ (transaction_write(tdb, off, buf, partial) != 0) {
				गच्छ fail;
			}
			len -= partial;
			off += partial;
			buf = (ध्रुव शून्य *)(partial + (ध्रुव वर्ण *)buf);
		}
		जइ (off + len <= el->offset + el->length) {
			partial = len;
		} अण्णहा {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (ध्रुव शून्य *)(partial + (ध्रुव वर्ण *)buf);

		जइ (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			गच्छ fail;
		}

		प्रत्यागम 0;
	}

	/* see if we can append the new entry to an existing entry */
	जइ (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned वर्ण *data = best_el->data;
		el = best_el;
		el->data = (unsigned वर्ण *)realloc(el->data,
						    el->length + len);
		जइ (el->data == रिक्त) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			प्रत्यागम -1;
		}
		जइ (buf) {
			memcpy(el->data + el->length, buf, len);
		} अण्णहा {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		प्रत्यागम 0;
	}

	/* add a new entry at the end of the list */
	el = (संरचना tdb_transaction_el *)malloc(परिमाण(*el));
	जइ (el == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		प्रत्यागम -1;
	}
	el->next = रिक्त;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned वर्ण *)malloc(len);
	जइ (el->data == रिक्त) {
		मुक्त(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		प्रत्यागम -1;
	}
	जइ (buf) {
		memcpy(el->data, buf, len);
	} अण्णहा {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	जइ (el->prev) {
		el->prev->next = el;
	} अण्णहा {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	प्रत्यागम 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	प्रत्यागम -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
स्थिर शून्य transaction_next_hash_chain(संरचना tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	प्रति (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		जइ (0 != tdb->transaction->hash_heads[h+1]) {
			भङ्ग;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
स्थिर पूर्णाङ्क transaction_oob(संरचना tdb_context *tdb, tdb_off_t len,
			   पूर्णाङ्क probe EXT2FS_ATTR((unused)))
{
	जइ (len <= tdb->map_size) {
		प्रत्यागम 0;
	}
	प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
स्थिर पूर्णाङ्क transaction_expand_file(संरचना tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	जइ (transaction_write(tdb, size, रिक्त, addition) != 0) {
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

/*
  brlock during a transaction - ignore them
*/
स्थिर पूर्णाङ्क transaction_brlock(संरचना tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      पूर्णाङ्क rw_type EXT2FS_ATTR((unused)),
			      पूर्णाङ्क lck_type EXT2FS_ATTR((unused)),
			      पूर्णाङ्क probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	प्रत्यागम 0;
}

स्थिर ध्रुव संरचना tdb_methods transaction_methods = {
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
पूर्णाङ्क tdb_transaction_start(संरचना tdb_context *tdb)
{
	/* some sanity checks */
	जइ (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		प्रत्यागम -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	जइ (tdb->transaction != रिक्त) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		प्रत्यागम 0;
	}

	जइ (tdb->अङ्कः_locks != 0 || tdb->global_lock.गणनम्) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		प्रत्यागम -1;
	}

	जइ (tdb->travlocks.next != रिक्त) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		प्रत्यागम -1;
	}

	tdb->transaction = (संरचना tdb_transaction *)
		calloc(परिमाण(संरचना tdb_transaction), 1);
	जइ (tdb->transaction == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		प्रत्यागम -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	जइ (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		प्रत्यागम -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	जइ (tdb_brlock(tdb, FREELIST_शिखरम्, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		गच्छ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, परिमाण(u32));
	जइ (tdb->transaction->hash_heads == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		गच्छ fail;
	}
	जइ (tdb->methods->tdb_read(tdb, FREELIST_शिखरम्, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		गच्छ fail;
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
	जइ (transaction_write(tdb, FREELIST_शिखरम्, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		गच्छ fail;
	}

	प्रत्यागम 0;

fail:
	tdb_brlock(tdb, FREELIST_शिखरम्, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	प्रत्यागम -1;
}


/*
  cancel the current transaction
*/
पूर्णाङ्क tdb_transaction_cancel(संरचना tdb_context *tdb)
{
	जइ (tdb->transaction == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		प्रत्यागम -1;
	}

	जइ (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		प्रत्यागम 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	जाव (tdb->transaction->elements) {
		संरचना tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		मुक्त(el->data);
		मुक्त(el);
	}

	/* remove any global lock created during the transaction */
	जइ (tdb->global_lock.गणनम् != 0) {
		tdb_brlock(tdb, FREELIST_शिखरम्, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.गणनम् = 0;
	}

	/* remove any locks created during the transaction */
	जइ (tdb->अङ्कः_locks != 0) {
		पूर्णाङ्क i;
		प्रति (i=0;i<tdb->अङ्कः_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_शिखरम्+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->अङ्कः_locks = 0;
		tdb->अङ्कः_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_शिखरम्, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	प्रत्यागम 0;
}

/*
  sync to disk
*/
स्थिर पूर्णाङ्क transaction_sync(संरचना tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	जइ (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		प्रत्यागम -1;
	}
#जइ defined(HAVE_MSYNC) && defined(MS_SYNC)
	जइ (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		जइ (msync(moffset + (वर्ण *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			प्रत्यागम -1;
		}
	}
#endif
	प्रत्यागम 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
स्थिर tdb_len_t tdb_recovery_size(संरचना tdb_context *tdb)
{
	संरचना tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = परिमाण(u32);
	प्रति (el=tdb->transaction->elements;el;el=el->next) {
		जइ (el->offset >= tdb->transaction->old_map_size) {
			अनुवर्त;
		}
		recovery_size += 2*परिमाण(tdb_off_t) + el->length;
	}

	प्रत्यागम recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
स्थिर पूर्णाङ्क tdb_recovery_allocate(संरचना tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	संरचना list_struct rec;
	ध्रुव संरचना tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	जइ (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		प्रत्यागम -1;
	}

	rec.rec_len = 0;

	जइ (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, परिमाण(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		प्रत्यागम -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	जइ (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		प्रत्यागम 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	जइ (recovery_head != 0) {
		जइ (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			प्रत्यागम -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(परिमाण(rec) + *recovery_size, tdb->page_size) - परिमाण(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	जइ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     परिमाण(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		प्रत्यागम -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	जइ (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, परिमाण(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
स्थिर पूर्णाङ्क transaction_setup_recovery(संरचना tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	संरचना tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned वर्ण *data, *p;
	ध्रुव संरचना tdb_methods *methods = tdb->transaction->io_methods;
	संरचना list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	जइ (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		प्रत्यागम -1;
	}

	data = (unsigned वर्ण *)malloc(recovery_size + परिमाण(*rec));
	जइ (data == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		प्रत्यागम -1;
	}

	rec = (संरचना list_struct *)data;
	memset(rec, 0, परिमाण(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->कुञ्चिका_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + परिमाण(*rec);
	प्रति (el=tdb->transaction->elements;el;el=el->next) {
		जइ (el->offset >= old_map_size) {
			अनुवर्त;
		}
		जइ (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			मुक्त(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			प्रत्यागम -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		जइ (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		जइ (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			मुक्त(data);
			tdb->ecode = TDB_ERR_IO;
			प्रत्यागम -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = परिमाण(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	जइ (methods->tdb_write(tdb, recovery_offset, data, परिमाण(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		मुक्त(data);
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	जइ (transaction_sync(tdb, recovery_offset, परिमाण(*rec) + recovery_size) == -1) {
		मुक्त(data);
		प्रत्यागम -1;
	}

	मुक्त(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(संरचना list_struct, magic);

	जइ (methods->tdb_write(tdb, *magic_offset, &magic, परिमाण(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* ensure the recovery magic marker is on disk */
	जइ (transaction_sync(tdb, *magic_offset, परिमाण(magic)) == -1) {
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

/*
  commit the current transaction
*/
पूर्णाङ्क tdb_transaction_commit(संरचना tdb_context *tdb)
{
	ध्रुव संरचना tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	जइ (tdb->transaction == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		प्रत्यागम -1;
	}

	जइ (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		प्रत्यागम -1;
	}

	जइ (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		प्रत्यागम 0;
	}

	/* check for a null transaction */
	जइ (tdb->transaction->elements == रिक्त) {
		tdb_transaction_cancel(tdb);
		प्रत्यागम 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	जइ (tdb->अङ्कः_locks || tdb->global_lock.गणनम्) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		प्रत्यागम -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	जइ (tdb_brlock_upgrade(tdb, FREELIST_शिखरम्, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		प्रत्यागम -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	जइ (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		प्रत्यागम -1;
	}

	जइ (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		जइ (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			प्रत्यागम -1;
		}
	}

	/* expand the file to the new size if needed */
	जइ (tdb->map_size != tdb->transaction->old_map_size) {
		जइ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			प्रत्यागम -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	जाव (tdb->transaction->elements) {
		संरचना tdb_transaction_el *el = tdb->transaction->elements;

		जइ (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			प्रत्यागम -1;
		}
		tdb->transaction->elements = el->next;
		मुक्त(el->data);
		मुक्त(el);
	}

	जइ (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		जइ (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			प्रत्यागम -1;
		}

		/* remove the recovery marker */
		जइ (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			प्रत्यागम -1;
		}

		/* ensure the recovery marker has been removed on disk */
		जइ (transaction_sync(tdb, magic_offset, 4) == -1) {
			प्रत्यागम -1;
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
	प्रत्यागम 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
पूर्णाङ्क tdb_transaction_recover(संरचना tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned वर्ण *data, *p;
	u32 zero = 0;
	संरचना list_struct rec;

	/* find the recovery area */
	जइ (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	जइ (recovery_head == 0) {
		/* we have never allocated a recovery record */
		प्रत्यागम 0;
	}

	/* read the recovery record */
	जइ (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   परिमाण(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	जइ (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		प्रत्यागम 0;
	}

	जइ (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		प्रत्यागम -1;
	}

	recovery_eof = rec.कुञ्चिका_len;

	data = (unsigned वर्ण *)malloc(rec.data_len);
	जइ (data == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		प्रत्यागम -1;
	}

	/* read the full recovery data */
	जइ (tdb->methods->tdb_read(tdb, recovery_head + परिमाण(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		मुक्त(data);
		प्रत्यागम -1;
	}

	/* recover the file data */
	p = data;
	जाव (p+8 < data + rec.data_len) {
		u32 ofs, len;
		जइ (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		जइ (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			मुक्त(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			प्रत्यागम -1;
		}
		p += 8 + len;
	}

	मुक्त(data);

	जइ (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	जइ (recovery_eof <= recovery_head) {
		जइ (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			प्रत्यागम -1;
		}
	}

	/* remove the recovery magic */
	जइ (tdb_ofs_write(tdb, recovery_head + offsetof(संरचना list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	जइ (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	जइ (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	प्रत्यागम 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
स्थिर पूर्णाङ्क tdb_rec_free_read(संरचना tdb_context *tdb, tdb_off_t off, संरचना list_struct *rec)
{
	जइ (tdb->methods->tdb_read(tdb, off, rec, परिमाण(*rec),DOCONV()) == -1)
		प्रत्यागम -1;

	जइ (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		जइ (tdb->methods->tdb_write(tdb, off, rec, परिमाण(*rec)) == -1)
			प्रत्यागम -1;
	}

	जइ (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	जइ (tdb->methods->tdb_oob(tdb, rec->next+परिमाण(*rec), 0) != 0)
		प्रत्यागम -1;
	प्रत्यागम 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
स्थिर पूर्णाङ्क remove_from_freelist(संरचना tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_शिखरम्;
	जाव (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		जइ (i == off) {
			/* We've found it! */
			प्रत्यागम tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	प्रत्यागम TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
स्थिर पूर्णाङ्क update_tailer(संरचना tdb_context *tdb, tdb_off_t offset,
			 ध्रुव संरचना list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = परिमाण(*rec) + rec->rec_len;
	प्रत्यागम tdb_ofs_write(tdb, offset + totalsize - परिमाण(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
पूर्णाङ्क tdb_free(संरचना tdb_context *tdb, tdb_off_t offset, संरचना list_struct *rec)
{
	tdb_off_t दक्षिणः, वामः;

	/* Allocation and tailer lock */
	जइ (tdb_lock(tdb, -1, F_WRLCK) != 0)
		प्रत्यागम -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	जइ (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		गच्छ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	दक्षिणः = offset + परिमाण(*rec) + rec->rec_len;
	जइ (दक्षिणः + परिमाण(*rec) <= tdb->map_size) {
		संरचना list_struct r;

		जइ (tdb->methods->tdb_read(tdb, दक्षिणः, &r, परिमाण(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", दक्षिणः));
			गच्छ वामः;
		}

		/* If it's free, expand to include it. */
		जइ (r.magic == TDB_FREE_MAGIC) {
			जइ (remove_from_freelist(tdb, दक्षिणः, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", दक्षिणः));
				गच्छ वामः;
			}
			rec->rec_len += परिमाण(r) + r.rec_len;
		}
	}

वामः:
	/* Look left */
	वामः = offset - परिमाण(tdb_off_t);
	जइ (वामः > TDB_DATA_START(tdb->header.hash_size)) {
		संरचना list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		जइ (tdb_ofs_read(tdb, वामः, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", वामः));
			गच्छ update;
		}

		/* it could be uninitialised data */
		जइ (leftsize == 0 || leftsize == TDB_PAD_U32) {
			गच्छ update;
		}

		वामः = offset - leftsize;

		/* Now read in record */
		जइ (tdb->methods->tdb_read(tdb, वामः, &l, परिमाण(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", वामः, leftsize));
			गच्छ update;
		}

		/* If it's free, expand to include it. */
		जइ (l.magic == TDB_FREE_MAGIC) {
			जइ (remove_from_freelist(tdb, वामः, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", वामः));
				गच्छ update;
			} अण्णहा {
				offset = वामः;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	जइ (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		गच्छ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	जइ (tdb_ofs_read(tdb, FREELIST_शिखरम्, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_शिखरम्, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		गच्छ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम -1;
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

	memset(&newrec, '\0', परिमाण(newrec));

	/* found it - now possibly split it up  */
	जइ (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (परिमाण(*rec) + length);
		newrec_ptr = rec_ptr + परिमाण(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} अण्णहा {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	जइ (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		प्रत्यागम 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	जइ (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		प्रत्यागम 0;
	}

	/* Did we create new block? */
	जइ (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		जइ (update_tailer(tdb, rec_ptr, rec) == -1) {
			प्रत्यागम 0;
		}

		/* Free new record */
		जइ (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			प्रत्यागम 0;
		}
	}

	/* all done - return the new record offset */
	प्रत्यागम rec_ptr;
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

	जइ (tdb_lock(tdb, -1, F_WRLCK) == -1)
		प्रत्यागम 0;

	/* Extra bytes required for tailer */
	length += परिमाण(tdb_off_t);

 again:
	last_ptr = FREELIST_शिखरम्;

	/* read in the freelist top */
	जइ (tdb_ofs_read(tdb, FREELIST_शिखरम्, &rec_ptr) == -1)
		गच्छ fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	जाव (rec_ptr) {
		जइ (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			गच्छ fail;
		}

		जइ (rec->rec_len >= length) {
			जइ (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				जइ (bestfit.rec_len < 2*length) {
					भङ्ग;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	जइ (bestfit.rec_ptr != 0) {
		जइ (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			गच्छ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		प्रत्यागम newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	जइ (tdb_expand(tdb, length + परिमाण(*rec)) == 0)
		गच्छ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

स्थिर पूर्णाङ्क seen_निवेशय(संरचना tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA कुञ्चिका, data;

	memset(&data, '\0', परिमाण(data));
	कुञ्चिका.dptr = (unsigned वर्ण *)&rec_ptr;
	कुञ्चिका.dsize = परिमाण(rec_ptr);
	प्रत्यागम tdb_store(mem_tdb, कुञ्चिका, data, TDB_निवेशय);
}

पूर्णाङ्क tdb_validate_freelist(संरचना tdb_context *tdb, पूर्णाङ्क *pnum_entries)
{
	संरचना tdb_context *mem_tdb = रिक्त;
	संरचना list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	पूर्णाङ्क ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	जइ (!mem_tdb) {
		प्रत्यागम -1;
	}

	जइ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		प्रत्यागम 0;
	}

	last_ptr = FREELIST_शिखरम्;

	/* Store the FREELIST_TOP record. */
	जइ (seen_निवेशय(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		गच्छ fail;
	}

	/* read in the freelist top */
	जइ (tdb_ofs_read(tdb, FREELIST_शिखरम्, &rec_ptr) == -1) {
		गच्छ fail;
	}

	जाव (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		जइ (seen_निवेशय(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			गच्छ fail;
		}

		जइ (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			गच्छ fail;
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
	प्रत्यागम ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
स्थिर पूर्णाङ्क tdb_next_lock(संरचना tdb_context *tdb, संरचना tdb_traverse_lock *tlock,
			 संरचना list_struct *rec)
{
	पूर्णाङ्क want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	प्रति (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		जइ (!tlock->off && tlock->hash != 0) {
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
			जइ (tlock->hash == tdb->header.hash_size) {
				अनुवर्त;
			}
		}

		जइ (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			प्रत्यागम -1;

		/* No previous record?  Start at top of chain. */
		जइ (!tlock->off) {
			जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(tlock->hash),
				     &tlock->off) == -1)
				गच्छ fail;
		} अण्णहा {
			/* Otherwise unlock the previous record. */
			जइ (tdb_unlock_record(tdb, tlock->off) != 0)
				गच्छ fail;
		}

		जइ (want_next) {
			/* We have offset of old record: grab next */
			जइ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				गच्छ fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		जाव( tlock->off) {
			tdb_off_t current;
			जइ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				गच्छ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			जइ (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				गच्छ fail;
			}

			जइ (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				जइ (tdb_lock_record(tdb, tlock->off) != 0)
					गच्छ fail;
				प्रत्यागम tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			जइ (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				गच्छ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	प्रत्यागम TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	जइ (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	प्रत्यागम -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
स्थिर पूर्णाङ्क tdb_traverse_internal(संरचना tdb_context *tdb,
				 tdb_traverse_func fn, शून्य *private_data,
				 संरचना tdb_traverse_lock *tl)
{
	TDB_DATA कुञ्चिका, dbuf;
	संरचना list_struct rec;
	पूर्णाङ्क ret, गणनम् = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	जाव ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		गणनम्++;
		/* now read the full record */
		कुञ्चिका.dptr = tdb_alloc_read(tdb, tl->off + परिमाण(rec),
					  rec.कुञ्चिका_len + rec.data_len);
		जइ (!कुञ्चिका.dptr) {
			ret = -1;
			जइ (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				गच्छ out;
			जइ (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			गच्छ out;
		}
		कुञ्चिका.dsize = rec.कुञ्चिका_len;
		dbuf.dptr = कुञ्चिका.dptr + rec.कुञ्चिका_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		जइ (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(कुञ्चिका.dptr);
			गच्छ out;
		}
		जइ (fn && fn(tdb, कुञ्चिका, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = गणनम्;
			जइ (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(कुञ्चिका.dptr);
			गच्छ out;
		}
		SAFE_FREE(कुञ्चिका.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	जइ (ret < 0)
		प्रत्यागम -1;
	अण्णहा
		प्रत्यागम गणनम्;
}


/*
  a write style traverse - temporarily marks the db read only
*/
पूर्णाङ्क tdb_traverse_read(संरचना tdb_context *tdb,
		      tdb_traverse_func fn, शून्य *private_data)
{
	संरचना tdb_traverse_lock tl = { रिक्त, 0, 0, F_RDLCK };
	पूर्णाङ्क ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	जइ (tdb_transaction_lock(tdb, F_RDLCK)) {
		प्रत्यागम -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	प्रत्यागम ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
पूर्णाङ्क tdb_traverse(संरचना tdb_context *tdb,
		 tdb_traverse_func fn, शून्य *private_data)
{
	संरचना tdb_traverse_lock tl = { रिक्त, 0, 0, F_WRLCK };
	पूर्णाङ्क ret;

	जइ (tdb->read_only || tdb->traverse_read) {
		प्रत्यागम tdb_traverse_read(tdb, fn, private_data);
	}

	जइ (tdb_transaction_lock(tdb, F_WRLCK)) {
		प्रत्यागम -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	प्रत्यागम ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(संरचना tdb_context *tdb)
{
	TDB_DATA कुञ्चिका;
	संरचना list_struct rec;

	/* release any old lock */
	जइ (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		प्रत्यागम tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	जइ (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		प्रत्यागम tdb_null;
	/* now read the key */
	कुञ्चिका.dsize = rec.कुञ्चिका_len;
	कुञ्चिका.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+परिमाण(rec),कुञ्चिका.dsize);

	/* Unlock the hash chain of the record we just read. */
	जइ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	प्रत्यागम कुञ्चिका;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(संरचना tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA कुञ्चिका = tdb_null;
	संरचना list_struct rec;
	unsigned वर्ण *k = रिक्त;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	जइ (tdb->travlocks.off) {
		जइ (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			प्रत्यागम tdb_null;
		जइ (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+परिमाण(rec),
					    rec.कुञ्चिका_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			जइ (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				प्रत्यागम tdb_null;
			}
			जइ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				प्रत्यागम tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	जइ (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		जइ (!tdb->travlocks.off)
			प्रत्यागम tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		जइ (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			प्रत्यागम tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	जइ (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		कुञ्चिका.dsize = rec.कुञ्चिका_len;
		कुञ्चिका.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+परिमाण(rec),
					  कुञ्चिका.dsize);
		/* Unlock the chain of this new record */
		जइ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	जइ (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	प्रत्यागम कुञ्चिका;
}

/* file: dump.c */

स्थिर tdb_off_t tdb_dump_record(संरचना tdb_context *tdb, पूर्णाङ्क hash,
				 tdb_off_t offset)
{
	संरचना list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	जइ (tdb->methods->tdb_read(tdb, offset, (वर्ण *)&rec,
				   परिमाण(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		प्रत्यागम 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.कुञ्चिका_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + परिमाण(rec) + rec.rec_len - परिमाण(tdb_off_t);

	जइ (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		प्रत्यागम rec.next;
	}

	जइ (tailer != rec.rec_len + परिमाण(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned पूर्णाङ्क)tailer, (unsigned पूर्णाङ्क)(rec.rec_len + परिमाण(rec)));
	}
	प्रत्यागम rec.next;
}

स्थिर पूर्णाङ्क tdb_dump_chain(संरचना tdb_context *tdb, पूर्णाङ्क i)
{
	tdb_off_t rec_ptr, शिखरम्;

	शिखरम् = TDB_HASH_शिखरम्(i);

	जइ (tdb_lock(tdb, i, F_WRLCK) != 0)
		प्रत्यागम -1;

	जइ (tdb_ofs_read(tdb, शिखरम्, &rec_ptr) == -1)
		प्रत्यागम tdb_unlock(tdb, i, F_WRLCK);

	जइ (rec_ptr)
		printf("hash=%d\n", i);

	जाव (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	प्रत्यागम tdb_unlock(tdb, i, F_WRLCK);
}

शून्य tdb_dump_all(संरचना tdb_context *tdb)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < (पूर्णाङ्क)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

पूर्णाङ्क tdb_printfreelist(संरचना tdb_context *tdb)
{
	पूर्णाङ्क ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	संरचना list_struct rec;

	जइ ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		प्रत्यागम ret;

	offset = FREELIST_शिखरम्;

	/* read in the freelist top */
	जइ (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		प्रत्यागम 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	जाव (rec_ptr) {
		जइ (tdb->methods->tdb_read(tdb, rec_ptr, (वर्ण *)&rec,
					   परिमाण(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			प्रत्यागम -1;
		}

		जइ (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			प्रत्यागम -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (पूर्णाङ्क)total_free,
               (पूर्णाङ्क)total_free);

	प्रत्यागम tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
शून्य tdb_increment_seqnum_nonblock(संरचना tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	जइ (!(tdb->flags & TDB_SEQNUM)) {
		प्रत्यागम;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	जइ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		प्रत्यागम;
	seqnum++;
	(शून्य) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
स्थिर शून्य tdb_increment_seqnum(संरचना tdb_context *tdb)
{
	जइ (!(tdb->flags & TDB_SEQNUM)) {
		प्रत्यागम;
	}

	जइ (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		प्रत्यागम;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

स्थिर पूर्णाङ्क tdb_कुञ्चिका_compare(TDB_DATA कुञ्चिका, TDB_DATA data,
			   शून्य *private_data EXT2FS_ATTR((unused)))
{
	प्रत्यागम memcmp(data.dptr, कुञ्चिका.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
स्थिर tdb_off_t tdb_find(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, u32 hash,
			संरचना list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		प्रत्यागम 0;

	/* keep looking until we find the right record */
	जाव (rec_ptr) {
		जइ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			प्रत्यागम 0;

		जइ (!TDB_DEAD(r) && hash==r->full_hash
		    && कुञ्चिका.dsize==r->कुञ्चिका_len
		    && tdb_parse_data(tdb, कुञ्चिका, rec_ptr + परिमाण(*r),
				      r->कुञ्चिका_len, tdb_कुञ्चिका_compare,
				      रिक्त) == 0) {
			प्रत्यागम rec_ptr;
		}
		rec_ptr = r->next;
	}
	प्रत्यागम TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, u32 hash, पूर्णाङ्क locktype,
			   संरचना list_struct *rec)
{
	u32 rec_ptr;

	जइ (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		प्रत्यागम 0;
	जइ (!(rec_ptr = tdb_find(tdb, कुञ्चिका, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	प्रत्यागम rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
स्थिर पूर्णाङ्क tdb_update_hash(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, u32 hash, TDB_DATA dbuf)
{
	संरचना list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	जइ (!(rec_ptr = tdb_find(tdb, कुञ्चिका, hash, &rec)))
		प्रत्यागम -1;

	/* must be long enough key, data and tailer */
	जइ (rec.rec_len < कुञ्चिका.dsize + dbuf.dsize + परिमाण(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		प्रत्यागम -1;
	}

	जइ (tdb->methods->tdb_write(tdb, rec_ptr + परिमाण(rec) + rec.कुञ्चिका_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		प्रत्यागम -1;

	जइ (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		प्रत्यागम tdb_rec_write(tdb, rec_ptr, &rec);
	}

	प्रत्यागम 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);
	जइ (!(rec_ptr = tdb_find_lock_hash(tdb,कुञ्चिका,hash,F_RDLCK,&rec)))
		प्रत्यागम tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + परिमाण(rec) + rec.कुञ्चिका_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	प्रत्यागम ret;
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

पूर्णाङ्क tdb_parse_record(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका,
		     पूर्णाङ्क (*parser)(TDB_DATA कुञ्चिका, TDB_DATA data,
				   शून्य *private_data),
		     शून्य *private_data)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	पूर्णाङ्क ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);

	जइ (!(rec_ptr = tdb_find_lock_hash(tdb,कुञ्चिका,hash,F_RDLCK,&rec))) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, कुञ्चिका, rec_ptr + परिमाण(rec) + rec.कुञ्चिका_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	प्रत्यागम ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
स्थिर पूर्णाङ्क tdb_exists_hash(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, u32 hash)
{
	संरचना list_struct rec;

	जइ (tdb_find_lock_hash(tdb, कुञ्चिका, hash, F_RDLCK, &rec) == 0)
		प्रत्यागम 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	प्रत्यागम 1;
}

पूर्णाङ्क tdb_exists(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	u32 hash = tdb->hash_fn(&कुञ्चिका);
	प्रत्यागम tdb_exists_hash(tdb, कुञ्चिका, hash);
}

/* actually delete an entry in the database given the offset */
पूर्णाङ्क tdb_do_delete(संरचना tdb_context *tdb, tdb_off_t rec_ptr, संरचना list_struct*rec)
{
	tdb_off_t last_ptr, i;
	संरचना list_struct lastrec;

	जइ (tdb->read_only || tdb->traverse_read) प्रत्यागम -1;

	जइ (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		प्रत्यागम tdb_rec_write(tdb, rec_ptr, rec);
	}
	जइ (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		प्रत्यागम -1;

	/* find previous record in hash chain */
	जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(rec->full_hash), &i) == -1)
		प्रत्यागम -1;
	प्रति (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		जइ (tdb_rec_read(tdb, i, &lastrec) == -1)
			प्रत्यागम -1;

	/* unlink it: next ptr is at start of record. */
	जइ (last_ptr == 0)
		last_ptr = TDB_HASH_शिखरम्(rec->full_hash);
	जइ (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		प्रत्यागम -1;

	/* recover the space */
	जइ (tdb_free(tdb, rec_ptr, rec) == -1)
		प्रत्यागम -1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क tdb_गणनम्_dead(संरचना tdb_context *tdb, u32 hash)
{
	पूर्णाङ्क res = 0;
	tdb_off_t rec_ptr;
	संरचना list_struct rec;

	/* read in the hash top */
	जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		प्रत्यागम 0;

	जाव (rec_ptr) {
		जइ (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			प्रत्यागम 0;

		जइ (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	प्रत्यागम res;
}

/*
 * Purge all DEAD records from a hash chain
 */
स्थिर पूर्णाङ्क tdb_purge_dead(संरचना tdb_context *tdb, u32 hash)
{
	पूर्णाङ्क res = -1;
	संरचना list_struct rec;
	tdb_off_t rec_ptr;

	जइ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		प्रत्यागम -1;
	}

	/* read in the hash top */
	जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		गच्छ fail;

	जाव (rec_ptr) {
		tdb_off_t next;

		जइ (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			गच्छ fail;
		}

		next = rec.next;

		जइ (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			गच्छ fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम res;
}

/* delete an entry in the database given a key */
स्थिर पूर्णाङ्क tdb_delete_hash(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, u32 hash)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	पूर्णाङ्क ret;

	जइ (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		जइ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			प्रत्यागम -1;

		जइ (tdb_गणनम्_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		जइ (!(rec_ptr = tdb_find(tdb, कुञ्चिका, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			प्रत्यागम -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	अण्णहा {
		जइ (!(rec_ptr = tdb_find_lock_hash(tdb, कुञ्चिका, hash, F_WRLCK,
						   &rec)))
			प्रत्यागम -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	जइ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	जइ (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	प्रत्यागम ret;
}

पूर्णाङ्क tdb_delete(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका)
{
	u32 hash = tdb->hash_fn(&कुञ्चिका);
	प्रत्यागम tdb_delete_hash(tdb, कुञ्चिका, hash);
}

/*
 * See if we have a dead record around with enough space
 */
स्थिर tdb_off_t tdb_find_dead(संरचना tdb_context *tdb, u32 hash,
			       संरचना list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		प्रत्यागम 0;

	/* keep looking until we find the right record */
	जाव (rec_ptr) {
		जइ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			प्रत्यागम 0;

		जइ (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			प्रत्यागम rec_ptr;
		}
		rec_ptr = r->next;
	}
	प्रत्यागम 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
पूर्णाङ्क tdb_store(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, TDB_DATA dbuf, पूर्णाङ्क flag)
{
	संरचना list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	वर्ण *p = रिक्त;
	पूर्णाङ्क ret = -1;

	जइ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);
	जइ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		प्रत्यागम -1;

	/* check for it existing, on insert. */
	जइ (flag == TDB_निवेशय) {
		जइ (tdb_exists_hash(tdb, कुञ्चिका, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			गच्छ fail;
		}
	} अण्णहा {
		/* first try in-place update, on modify or replace. */
		जइ (tdb_update_hash(tdb, कुञ्चिका, hash, dbuf) == 0) {
			गच्छ done;
		}
		जइ (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			गच्छ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	जइ (flag != TDB_निवेशय)
		tdb_delete_hash(tdb, कुञ्चिका, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	जइ (!(p = (वर्ण *)malloc(कुञ्चिका.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		गच्छ fail;
	}

	memcpy(p, कुञ्चिका.dptr, कुञ्चिका.dsize);
	जइ (dbuf.dsize)
		memcpy(p+कुञ्चिका.dsize, dbuf.dptr, dbuf.dsize);

	जइ (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			कुञ्चिका.dsize + dbuf.dsize + परिमाण(tdb_off_t));

		जइ (rec_ptr != 0) {
			rec.कुञ्चिका_len = कुञ्चिका.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			जइ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + परिमाण(rec),
				    p, कुञ्चिका.dsize + dbuf.dsize) == -1) {
				गच्छ fail;
			}
			गच्छ done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	जइ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		गच्छ fail;
	}

	जइ ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		गच्छ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, कुञ्चिका.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	जइ (rec_ptr == 0) {
		गच्छ fail;
	}

	/* Read hash top into next ptr */
	जइ (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec.next) == -1)
		गच्छ fail;

	rec.कुञ्चिका_len = कुञ्चिका.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	जइ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+परिमाण(rec), p, कुञ्चिका.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		गच्छ fail;
	}

 done:
	ret = 0;
 fail:
	जइ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	प्रत्यागम ret;
}


/* Append to an entry. Create if not exist. */
पूर्णाङ्क tdb_append(संरचना tdb_context *tdb, TDB_DATA कुञ्चिका, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	पूर्णाङ्क ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);
	जइ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		प्रत्यागम -1;

	dbuf = tdb_fetch(tdb, कुञ्चिका);

	जइ (dbuf.dptr == रिक्त) {
		dbuf.dptr = (unsigned वर्ण *)malloc(new_dbuf.dsize);
	} अण्णहा {
		unsigned वर्ण *new_dptr = (unsigned वर्ण *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		जइ (new_dptr == रिक्त) {
			मुक्त(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	जइ (dbuf.dptr == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		गच्छ failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, कुञ्चिका, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	प्रत्यागम ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ध्रुव वर्ण *tdb_name(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
पूर्णाङ्क tdb_fd(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->log.log_fn;
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
पूर्णाङ्क tdb_get_seqnum(संरचना tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	जइ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		प्रत्यागम 0;
	प्रत्यागम seqnum;
}

पूर्णाङ्क tdb_hash_size(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->header.hash_size;
}

size_t tdb_map_size(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->map_size;
}

पूर्णाङ्क tdb_get_flags(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->flags;
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
स्थिर unsigned पूर्णाङ्क default_tdb_hash(TDB_DATA *कुञ्चिका)
{
	u32 मानम्;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	प्रति (मानम् = 0, i=0; i < कुञ्चिका->dsize; i++)
		मानम् = मानम् * 256 + कुञ्चिका->dptr[i] + (मानम् >> 24) * 241;

	प्रत्यागम मानम्;
}


/* initialise a new database with a specified hash size */
स्थिर पूर्णाङ्क tdb_new_database(संरचना tdb_context *tdb, पूर्णाङ्क hash_size)
{
	संरचना tdb_header *newdb;
	पूर्णाङ्क size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = परिमाण(संरचना tdb_header) + (hash_size+1)*परिमाण(tdb_off_t);
	जइ (!(newdb = (संरचना tdb_header *)calloc(size, 1)))
		प्रत्यागम TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	जइ (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (वर्ण *)newdb;
		memcpy(&tdb->header, newdb, परिमाण(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		प्रत्यागम 0;
	}
	जइ (lseek(tdb->fd, 0, SEEK_SET) == -1)
		गच्छ fail;

	जइ (ftruncate(tdb->fd, 0) == -1)
		गच्छ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, परिमाण(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	जइ (लिख(tdb->fd, newdb, size) != size) {
		ret = -1;
	} अण्णहा {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	प्रत्यागम ret;
}



स्थिर पूर्णाङ्क tdb_already_open(dev_t device,
			    ino_t ino)
{
	संरचना tdb_context *i;

	प्रति (i = tdbs; i; i = i->next) {
		जइ (i->device == device && i->inode == ino) {
			प्रत्यागम 1;
		}
	}

	प्रत्यागम 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
संरचना tdb_context *tdb_open(ध्रुव वर्ण *name, पूर्णाङ्क hash_size, पूर्णाङ्क tdb_flags,
		      पूर्णाङ्क open_flags, mode_t mode)
{
	प्रत्यागम tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, रिक्त, रिक्त);
}

/* a default logging function */
स्थिर शून्य null_log_fn(संरचना tdb_context *tdb, गणना tdb_debug_level level, ध्रुव वर्ण *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
स्थिर शून्य null_log_fn(संरचना tdb_context *tdb EXT2FS_ATTR((unused)),
			गणना tdb_debug_level level EXT2FS_ATTR((unused)),
			ध्रुव वर्ण *fmt EXT2FS_ATTR((unused)), ...)
{
}


संरचना tdb_context *tdb_open_ex(ध्रुव वर्ण *name, पूर्णाङ्क hash_size, पूर्णाङ्क tdb_flags,
				पूर्णाङ्क open_flags, mode_t mode,
				ध्रुव संरचना tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	संरचना tdb_context *tdb;
	संरचना stat st;
	पूर्णाङ्क rev = 0, locked = 0;
	unsigned वर्ण *vp;
	u32 vertest;

	जइ (!(tdb = (संरचना tdb_context *)calloc(1, परिमाण *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		गच्छ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = रिक्त;
	tdb->map_ptr = रिक्त;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	जइ (log_ctx) {
		tdb->log = *log_ctx;
	} अण्णहा {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = रिक्त;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	जइ (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	जइ ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		गच्छ fail;
	}

	जइ (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	जइ ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	जइ (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		जइ (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			गच्छ fail;
		}
		गच्छ internal;
	}

	जइ ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		गच्छ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	जइ (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		गच्छ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	जइ ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		जइ (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			गच्छ fail; /* errno set by ftruncate */
		}
	}

	जइ (पठ(tdb->fd, &tdb->header, परिमाण(tdb->header)) != परिमाण(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      परिमाण(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		जइ (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			गच्छ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned वर्ण *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	जइ (!rev)
		tdb->flags &= ~TDB_CONVERT;
	अण्णहा {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, परिमाण(tdb->header));
	}
	जइ (fstat(tdb->fd, &st) == -1)
		गच्छ fail;

	जइ (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		गच्छ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	जइ (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (पूर्णाङ्क)st.st_dev, (पूर्णाङ्क)st.st_ino));
		errno = EBUSY;
		गच्छ fail;
	}

	जइ (!(tdb->name = (वर्ण *)strdup(name))) {
		errno = ENOMEM;
		गच्छ fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	जइ (locked) {
		जइ (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			गच्छ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	जइ (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		जइ (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			गच्छ fail;
	}

	/* if needed, run recovery */
	जइ (tdb_transaction_recover(tdb) == -1) {
		गच्छ fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	जइ (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		गच्छ fail;
	tdb->next = tdbs;
	tdbs = tdb;
	प्रत्यागम tdb;

 fail:
	{ पूर्णाङ्क save_errno = errno;

	जइ (!tdb)
		प्रत्यागम रिक्त;

	जइ (tdb->map_ptr) {
		जइ (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		अण्णहा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	जइ (tdb->fd != -1)
		जइ (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	प्रत्यागम रिक्त;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

शून्य tdb_set_max_dead(संरचना tdb_context *tdb, पूर्णाङ्क max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
पूर्णाङ्क tdb_close(संरचना tdb_context *tdb)
{
	संरचना tdb_context **i;
	पूर्णाङ्क ret = 0;

	जइ (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	जइ (tdb->map_ptr) {
		जइ (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		अण्णहा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	जइ (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	प्रति (i = &tdbs; *i; i = &(*i)->next) {
		जइ (*i == tdb) {
			*i = tdb->next;
			भङ्ग;
		}
	}

	memset(tdb, 0, परिमाण(*tdb));
	SAFE_FREE(tdb);

	प्रत्यागम ret;
}

/* register a logging function */
शून्य tdb_set_logging_function(संरचना tdb_context *tdb,
                              ध्रुव संरचना tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

शून्य *tdb_get_logging_private(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
पूर्णाङ्क tdb_reopen(संरचना tdb_context *tdb)
{
	संरचना stat st;

	जइ (tdb->flags & TDB_INTERNAL) {
		प्रत्यागम 0; /* Nothing to do. */
	}

	जइ (tdb->अङ्कः_locks != 0 || tdb->global_lock.गणनम्) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		गच्छ fail;
	}

	जइ (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		गच्छ fail;
	}

	जइ (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		गच्छ fail;
	}
	जइ (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	जइ (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		गच्छ fail;
	}
	जइ ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		गच्छ fail;
	}
	जइ (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		गच्छ fail;
	}
	जइ (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		गच्छ fail;
	}
	tdb_mmap(tdb);

	प्रत्यागम 0;

fail:
	tdb_close(tdb);
	प्रत्यागम -1;
}

/* reopen all tdb's */
पूर्णाङ्क tdb_reopen_all(पूर्णाङ्क parent_longlived)
{
	संरचना tdb_context *tdb;

	प्रति (tdb=tdbs; tdb; tdb = tdb->next) {
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
		जइ (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		जइ (tdb_reopen(tdb) != 0)
			प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

/**
 * Flush a database file from the page cache.
 **/
पूर्णाङ्क tdb_flush(संरचना tdb_context *tdb)
{
	जइ (tdb->fd != -1)
		प्रत्यागम fsync(tdb->fd);
	प्रत्यागम 0;
}
