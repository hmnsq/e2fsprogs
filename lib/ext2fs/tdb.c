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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <string.h>
#ifdef HAVE_SYS_SELECT_H
#അന്തര്ഭാവ <sys/select.h>
#endif
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <time.h>
#ifdef HAVE_UTIME_H
#അന്തര്ഭാവ <utime.h>
#endif
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/file.h>
#അന്തര്ഭാവ <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#അന്തര്ഭാവ <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#അല്ലെങ്കിൽ
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((ശൂന്യം *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
സ്ഥിതം അക്ഷരം *rep_strdup(സ്ഥിരം അക്ഷരം *s)
{
	അക്ഷരം *ret;
	പൂർണ്ണസംഖ്യ length;

	എങ്കിൽ (!s)
		തിരികെ ശൂന്യ;
	length = strlen(s);
	ret = malloc(length + 1);
	എങ്കിൽ (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	തിരികെ ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#എങ്കിൽ (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#അല്ലെങ്കിൽ
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#എങ്കിൽ defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
തരനിർവചനം പൂർണ്ണസംഖ്യ bool;
#endif

#അന്തര്ഭാവ "tdb.h"

സ്ഥിതം TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

തരനിർവചനം u32 tdb_len_t;
തരനിർവചനം u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned പൂർണ്ണസംഖ്യ)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*വലുപ്പം(ഘടന list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_മുകൾ (വലുപ്പം(ഘടന tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_മുകൾ(hash) (FREELIST_മുകൾ + (BUCKET(hash)+1)*വലുപ്പം(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*വലുപ്പം(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_മുകൾ(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(ഘടന tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(ഘടന tdb_header, sequence_സംഖ്യ)
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
#define SAFE_FREE(x) ചെയ്യുക { എങ്കിൽ ((x) != ശൂന്യ) {സ്വതന്ത്രം(x); (x)=ശൂന്യ;} } ആകുമ്പോൾ(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, വലുപ്പം(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ഘടന list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t താക്കോൽ_len; /* byte length of key */
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
ഘടന tdb_header {
	അക്ഷരം magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_സംഖ്യ; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ഘടന tdb_lock_type {
	പൂർണ്ണസംഖ്യ list;
	u32 എണ്ണം;
	u32 ltype;
};

ഘടന tdb_traverse_lock {
	ഘടന tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	പൂർണ്ണസംഖ്യ lock_rw;
};


ഘടന tdb_methods {
	പൂർണ്ണസംഖ്യ (*tdb_read)(ഘടന tdb_context *, tdb_off_t , ശൂന്യം *, tdb_len_t , പൂർണ്ണസംഖ്യ );
	പൂർണ്ണസംഖ്യ (*tdb_write)(ഘടന tdb_context *, tdb_off_t, സ്ഥിരം ശൂന്യം *, tdb_len_t);
	ശൂന്യം (*next_hash_chain)(ഘടന tdb_context *, u32 *);
	പൂർണ്ണസംഖ്യ (*tdb_oob)(ഘടന tdb_context *, tdb_off_t , പൂർണ്ണസംഖ്യ );
	പൂർണ്ണസംഖ്യ (*tdb_expand_file)(ഘടന tdb_context *, tdb_off_t , tdb_off_t );
	പൂർണ്ണസംഖ്യ (*tdb_brlock)(ഘടന tdb_context *, tdb_off_t , പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, size_t);
};

ഘടന tdb_context {
	അക്ഷരം *name; /* the name of the database */
	ശൂന്യം *map_ptr; /* where it is currently mapped */
	പൂർണ്ണസംഖ്യ fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	പൂർണ്ണസംഖ്യ read_only; /* opened read-only */
	പൂർണ്ണസംഖ്യ traverse_read; /* read-only traversal */
	ഘടന tdb_lock_type global_lock;
	പൂർണ്ണസംഖ്യ അക്കം_lockrecs;
	ഘടന tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	എണ്ണൽ TDB_ERROR ecode; /* error code for last tdb error */
	ഘടന tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ഘടന tdb_traverse_lock travlocks; /* current traversal locks */
	ഘടന tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ഘടന tdb_logging_context log;
	unsigned പൂർണ്ണസംഖ്യ (*hash_fn)(TDB_DATA *താക്കോൽ);
	പൂർണ്ണസംഖ്യ open_flags; /* flags used in the open - needed by reopen */
	unsigned പൂർണ്ണസംഖ്യ അക്കം_locks; /* number of chain locks held */
	സ്ഥിരം ഘടന tdb_methods *methods;
	ഘടന tdb_transaction *transaction;
	പൂർണ്ണസംഖ്യ page_size;
	പൂർണ്ണസംഖ്യ max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_munmap(ഘടന tdb_context *tdb);
സ്ഥിതം ശൂന്യം tdb_mmap(ഘടന tdb_context *tdb);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_lock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype);
പൂർണ്ണസംഖ്യ tdb_lock_nonblock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_unlock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_brlock(ഘടന tdb_context *tdb, tdb_off_t offset, പൂർണ്ണസംഖ്യ rw_type, പൂർണ്ണസംഖ്യ lck_type, പൂർണ്ണസംഖ്യ probe, size_t len);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_transaction_lock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ ltype);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_transaction_unlock(ഘടന tdb_context *tdb);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_brlock_upgrade(ഘടന tdb_context *tdb, tdb_off_t offset, size_t len);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_write_lock_record(ഘടന tdb_context *tdb, tdb_off_t off);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_write_unlock_record(ഘടന tdb_context *tdb, tdb_off_t off);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_ofs_read(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_ofs_write(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
സ്ഥിതം ശൂന്യം *tdb_convert(ശൂന്യം *buf, u32 size);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_free(ഘടന tdb_context *tdb, tdb_off_t offset, ഘടന list_struct *rec);
സ്ഥിതം tdb_off_t tdb_allocate(ഘടന tdb_context *tdb, tdb_len_t length, ഘടന list_struct *rec);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_ofs_read(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_ofs_write(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_lock_record(ഘടന tdb_context *tdb, tdb_off_t off);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_unlock_record(ഘടന tdb_context *tdb, tdb_off_t off);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_rec_read(ഘടന tdb_context *tdb, tdb_off_t offset, ഘടന list_struct *rec);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_rec_write(ഘടന tdb_context *tdb, tdb_off_t offset, ഘടന list_struct *rec);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_do_delete(ഘടന tdb_context *tdb, tdb_off_t rec_ptr, ഘടന list_struct *rec);
സ്ഥിതം unsigned അക്ഷരം *tdb_alloc_read(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_parse_data(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ,
		   tdb_off_t offset, tdb_len_t len,
		   പൂർണ്ണസംഖ്യ (*parser)(TDB_DATA താക്കോൽ, TDB_DATA data,
				 ശൂന്യം *private_data),
		   ശൂന്യം *private_data);
സ്ഥിതം tdb_off_t tdb_find_lock_hash(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, u32 hash, പൂർണ്ണസംഖ്യ locktype,
			   ഘടന list_struct *rec);
സ്ഥിതം ശൂന്യം tdb_io_init(ഘടന tdb_context *tdb);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_expand(ഘടന tdb_context *tdb, tdb_off_t size);
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_rec_free_read(ഘടന tdb_context *tdb, tdb_off_t off,
		      ഘടന list_struct *rec);


/* file: error.c */

എണ്ണൽ TDB_ERROR tdb_error(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->ecode;
}

സ്ഥിതം ഘടന tdb_errname {
	എണ്ണൽ TDB_ERROR ecode; സ്ഥിരം അക്ഷരം *estring;
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
സ്ഥിരം അക്ഷരം *tdb_errorstr(ഘടന tdb_context *tdb)
{
	u32 i;
	വേണ്ടി (i = 0; i < വലുപ്പം(emap) / വലുപ്പം(ഘടന tdb_errname); i++)
		എങ്കിൽ (tdb->ecode == emap[i].ecode)
			തിരികെ emap[i].estring;
	തിരികെ "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
പൂർണ്ണസംഖ്യ tdb_brlock(ഘടന tdb_context *tdb, tdb_off_t offset,
	       പൂർണ്ണസംഖ്യ rw_type, പൂർണ്ണസംഖ്യ lck_type, പൂർണ്ണസംഖ്യ probe, size_t len)
{
	ഘടന flock fl;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (tdb->flags & TDB_NOLOCK) {
		തിരികെ 0;
	}

	എങ്കിൽ ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		തിരികെ -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	ചെയ്യുക {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} ആകുമ്പോൾ (ret == -1 && errno == EINTR);

	എങ്കിൽ (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		എങ്കിൽ (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (പൂർണ്ണസംഖ്യ)len));
		}
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	തിരികെ 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
പൂർണ്ണസംഖ്യ tdb_brlock_upgrade(ഘടന tdb_context *tdb, tdb_off_t offset, size_t len)
{
	പൂർണ്ണസംഖ്യ എണ്ണം = 1000;
	ആകുമ്പോൾ (എണ്ണം--) {
		ഘടന timeval tv;
		എങ്കിൽ (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			തിരികെ 0;
		}
		എങ്കിൽ (errno != EDEADLK) {
			നിർത്തുക;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ശൂന്യ, ശൂന്യ, ശൂന്യ, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	തിരികെ -1;
}


/* lock a list in the database. list -1 is the alloc list */
സ്ഥിതം പൂർണ്ണസംഖ്യ _tdb_lock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype, പൂർണ്ണസംഖ്യ op)
{
	ഘടന tdb_lock_type *new_lck;
	പൂർണ്ണസംഖ്യ i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	എങ്കിൽ (tdb->global_lock.എണ്ണം &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		തിരികെ 0;
	}

	എങ്കിൽ (tdb->global_lock.എണ്ണം) {
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	എങ്കിൽ (list < -1 || list >= (പൂർണ്ണസംഖ്യ)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		തിരികെ -1;
	}
	എങ്കിൽ (tdb->flags & TDB_NOLOCK)
		തിരികെ 0;

	വേണ്ടി (i=0; i<tdb->അക്കം_lockrecs; i++) {
		എങ്കിൽ (tdb->lockrecs[i].list == list) {
			എങ്കിൽ (tdb->lockrecs[i].എണ്ണം == 0) {
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
			tdb->lockrecs[i].എണ്ണം++;
			തിരികെ 0;
		}
	}

	new_lck = (ഘടന tdb_lock_type *)realloc(
		tdb->lockrecs,
		വലുപ്പം(*tdb->lockrecs) * (tdb->അക്കം_lockrecs+1));
	എങ്കിൽ (new_lck == ശൂന്യ) {
		errno = ENOMEM;
		തിരികെ -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	എങ്കിൽ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_മുകൾ+4*list, ltype, op,
				     0, 1)) {
		തിരികെ -1;
	}

	tdb->അക്കം_locks++;

	tdb->lockrecs[tdb->അക്കം_lockrecs].list = list;
	tdb->lockrecs[tdb->അക്കം_lockrecs].എണ്ണം = 1;
	tdb->lockrecs[tdb->അക്കം_lockrecs].ltype = ltype;
	tdb->അക്കം_lockrecs += 1;

	തിരികെ 0;
}

/* lock a list in the database. list -1 is the alloc list */
പൂർണ്ണസംഖ്യ tdb_lock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype)
{
	പൂർണ്ണസംഖ്യ ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	എങ്കിൽ (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	തിരികെ ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
പൂർണ്ണസംഖ്യ tdb_lock_nonblock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype)
{
	തിരികെ _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
പൂർണ്ണസംഖ്യ tdb_unlock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ list, പൂർണ്ണസംഖ്യ ltype)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	പൂർണ്ണസംഖ്യ i;
	ഘടന tdb_lock_type *lck = ശൂന്യ;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	എങ്കിൽ (tdb->global_lock.എണ്ണം &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		തിരികെ 0;
	}

	എങ്കിൽ (tdb->global_lock.എണ്ണം) {
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	എങ്കിൽ (tdb->flags & TDB_NOLOCK)
		തിരികെ 0;

	/* Sanity checks */
	എങ്കിൽ (list < -1 || list >= (പൂർണ്ണസംഖ്യ)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		തിരികെ ret;
	}

	വേണ്ടി (i=0; i<tdb->അക്കം_lockrecs; i++) {
		എങ്കിൽ (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			നിർത്തുക;
		}
	}

	എങ്കിൽ ((lck == ശൂന്യ) || (lck->എണ്ണം == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		തിരികെ -1;
	}

	എങ്കിൽ (lck->എണ്ണം > 1) {
		lck->എണ്ണം--;
		തിരികെ 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	എങ്കിൽ (mark_lock) {
		ret = 0;
	} അല്ലെങ്കിൽ {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_മുകൾ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->അക്കം_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	എങ്കിൽ (tdb->അക്കം_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->അക്കം_lockrecs-1];
	}
	tdb->അക്കം_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	എങ്കിൽ (tdb->അക്കം_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	എങ്കിൽ (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	തിരികെ ret;
}

/*
  get the transaction lock
 */
പൂർണ്ണസംഖ്യ tdb_transaction_lock(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ ltype)
{
	എങ്കിൽ (tdb->have_transaction_lock || tdb->global_lock.എണ്ണം) {
		തിരികെ 0;
	}
	എങ്കിൽ (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		തിരികെ -1;
	}
	tdb->have_transaction_lock = 1;
	തിരികെ 0;
}

/*
  release the transaction lock
 */
പൂർണ്ണസംഖ്യ tdb_transaction_unlock(ഘടന tdb_context *tdb)
{
	പൂർണ്ണസംഖ്യ ret;
	എങ്കിൽ (!tdb->have_transaction_lock) {
		തിരികെ 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	എങ്കിൽ (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	തിരികെ ret;
}




/* lock/unlock entire database */
സ്ഥിതം പൂർണ്ണസംഖ്യ _tdb_lockall(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ ltype, പൂർണ്ണസംഖ്യ op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	എങ്കിൽ (tdb->read_only || tdb->traverse_read)
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);

	എങ്കിൽ (tdb->global_lock.എണ്ണം && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.എണ്ണം++;
		തിരികെ 0;
	}

	എങ്കിൽ (tdb->global_lock.എണ്ണം) {
		/* a global lock of a different type exists */
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	എങ്കിൽ (tdb->അക്കം_locks != 0) {
		/* can't combine global and chain locks */
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	എങ്കിൽ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_മുകൾ, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		എങ്കിൽ (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		തിരികെ -1;
	}

	tdb->global_lock.എണ്ണം = 1;
	tdb->global_lock.ltype = ltype;

	തിരികെ 0;
}



/* unlock entire db */
സ്ഥിതം പൂർണ്ണസംഖ്യ _tdb_unlockall(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	എങ്കിൽ (tdb->read_only || tdb->traverse_read) {
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	എങ്കിൽ (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.എണ്ണം == 0) {
		തിരികെ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	എങ്കിൽ (tdb->global_lock.എണ്ണം > 1) {
		tdb->global_lock.എണ്ണം--;
		തിരികെ 0;
	}

	എങ്കിൽ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_മുകൾ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		തിരികെ -1;
	}

	tdb->global_lock.എണ്ണം = 0;
	tdb->global_lock.ltype = 0;

	തിരികെ 0;
}

/* lock entire database with write lock */
പൂർണ്ണസംഖ്യ tdb_lockall(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
പൂർണ്ണസംഖ്യ tdb_lockall_mark(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
പൂർണ്ണസംഖ്യ tdb_lockall_unmark(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
പൂർണ്ണസംഖ്യ tdb_lockall_nonblock(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
പൂർണ്ണസംഖ്യ tdb_unlockall(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
പൂർണ്ണസംഖ്യ tdb_lockall_read(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
പൂർണ്ണസംഖ്യ tdb_lockall_read_nonblock(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
പൂർണ്ണസംഖ്യ tdb_unlockall_read(ഘടന tdb_context *tdb)
{
	തിരികെ _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
പൂർണ്ണസംഖ്യ tdb_chainlock(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_lock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
പൂർണ്ണസംഖ്യ tdb_chainlock_nonblock(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
പൂർണ്ണസംഖ്യ tdb_chainlock_mark(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_lock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
പൂർണ്ണസംഖ്യ tdb_chainlock_unmark(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_WRLCK | TDB_MARK_LOCK);
}

പൂർണ്ണസംഖ്യ tdb_chainunlock(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_WRLCK);
}

പൂർണ്ണസംഖ്യ tdb_chainlock_read(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_lock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_RDLCK);
}

പൂർണ്ണസംഖ്യ tdb_chainunlock_read(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	തിരികെ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&താക്കോൽ)), F_RDLCK);
}



/* record lock stops delete underneath */
പൂർണ്ണസംഖ്യ tdb_lock_record(ഘടന tdb_context *tdb, tdb_off_t off)
{
	തിരികെ off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
പൂർണ്ണസംഖ്യ tdb_write_lock_record(ഘടന tdb_context *tdb, tdb_off_t off)
{
	ഘടന tdb_traverse_lock *i;
	വേണ്ടി (i = &tdb->travlocks; i; i = i->next)
		എങ്കിൽ (i->off == off)
			തിരികെ -1;
	തിരികെ tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
പൂർണ്ണസംഖ്യ tdb_write_unlock_record(ഘടന tdb_context *tdb, tdb_off_t off)
{
	തിരികെ tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
പൂർണ്ണസംഖ്യ tdb_unlock_record(ഘടന tdb_context *tdb, tdb_off_t off)
{
	ഘടന tdb_traverse_lock *i;
	u32 എണ്ണം = 0;

	എങ്കിൽ (off == 0)
		തിരികെ 0;
	വേണ്ടി (i = &tdb->travlocks; i; i = i->next)
		എങ്കിൽ (i->off == off)
			എണ്ണം++;
	തിരികെ (എണ്ണം == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_oob(ഘടന tdb_context *tdb, tdb_off_t len, പൂർണ്ണസംഖ്യ probe)
{
	ഘടന stat st;
	എങ്കിൽ (len <= tdb->map_size)
		തിരികെ 0;
	എങ്കിൽ (tdb->flags & TDB_INTERNAL) {
		എങ്കിൽ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (പൂർണ്ണസംഖ്യ)len, (പൂർണ്ണസംഖ്യ)tdb->map_size));
		}
		തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	എങ്കിൽ (fstat(tdb->fd, &st) == -1) {
		തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	എങ്കിൽ (st.st_size < (off_t)len) {
		എങ്കിൽ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (പൂർണ്ണസംഖ്യ)len, (പൂർണ്ണസംഖ്യ)st.st_size));
		}
		തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	എങ്കിൽ (tdb_munmap(tdb) == -1)
		തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	തിരികെ 0;
}

/* write a lump of data at a specified offset */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_write(ഘടന tdb_context *tdb, tdb_off_t off,
		     സ്ഥിരം ശൂന്യം *buf, tdb_len_t len)
{
	എങ്കിൽ (len == 0) {
		തിരികെ 0;
	}

	എങ്കിൽ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		തിരികെ -1;

	എങ്കിൽ (tdb->map_ptr) {
		memcpy(off + (അക്ഷരം *)tdb->map_ptr, buf, len);
	} അല്ലെങ്കിൽ എങ്കിൽ (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	തിരികെ 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
ശൂന്യം *tdb_convert(ശൂന്യം *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	വേണ്ടി (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	തിരികെ buf;
}


/* read a lump of data at a specified offset, maybe convert */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_read(ഘടന tdb_context *tdb, tdb_off_t off, ശൂന്യം *buf,
		    tdb_len_t len, പൂർണ്ണസംഖ്യ cv)
{
	എങ്കിൽ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->map_ptr) {
		memcpy(buf, off + (അക്ഷരം *)tdb->map_ptr, len);
	} അല്ലെങ്കിൽ {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		എങ്കിൽ (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (പൂർണ്ണസംഖ്യ)off, (പൂർണ്ണസംഖ്യ)len, (പൂർണ്ണസംഖ്യ)ret, strerror(errno),
				 (പൂർണ്ണസംഖ്യ)tdb->map_size));
			തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	എങ്കിൽ (cv) {
		tdb_convert(buf, len);
	}
	തിരികെ 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
സ്ഥിതം ശൂന്യം tdb_next_hash_chain(ഘടന tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	എങ്കിൽ (tdb->map_ptr) {
		വേണ്ടി (;h < tdb->header.hash_size;h++) {
			എങ്കിൽ (0 != *(u32 *)(TDB_HASH_മുകൾ(h) + (unsigned അക്ഷരം *)tdb->map_ptr)) {
				നിർത്തുക;
			}
		}
	} അല്ലെങ്കിൽ {
		u32 off=0;
		വേണ്ടി (;h < tdb->header.hash_size;h++) {
			എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(h), &off) != 0 || off != 0) {
				നിർത്തുക;
			}
		}
	}
	(*chain) = h;
}


പൂർണ്ണസംഖ്യ tdb_munmap(ഘടന tdb_context *tdb)
{
	എങ്കിൽ (tdb->flags & TDB_INTERNAL)
		തിരികെ 0;

#ifdef HAVE_MMAP
	എങ്കിൽ (tdb->map_ptr) {
		പൂർണ്ണസംഖ്യ ret = munmap(tdb->map_ptr, tdb->real_map_size);
		എങ്കിൽ (ret != 0)
			തിരികെ ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = ശൂന്യ;
	തിരികെ 0;
}

ശൂന്യം tdb_mmap(ഘടന tdb_context *tdb)
{
	എങ്കിൽ (tdb->flags & TDB_INTERNAL)
		തിരികെ;

#ifdef HAVE_MMAP
	എങ്കിൽ (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ശൂന്യ, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		എങ്കിൽ (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = ശൂന്യ;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} അല്ലെങ്കിൽ {
		tdb->map_ptr = ശൂന്യ;
	}
#അല്ലെങ്കിൽ
	tdb->map_ptr = ശൂന്യ;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_expand_file(ഘടന tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	അക്ഷരം buf[1024];

	എങ്കിൽ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		തിരികെ -1;
	}

	എങ്കിൽ (ftruncate(tdb->fd, size+addition) == -1) {
		അക്ഷരം b = 0;
		എങ്കിൽ (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			തിരികെ -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, വലുപ്പം(buf));
	ആകുമ്പോൾ (addition) {
		പൂർണ്ണസംഖ്യ n = addition>വലുപ്പം(buf)?വലുപ്പം(buf):addition;
		പൂർണ്ണസംഖ്യ ret = pwrite(tdb->fd, buf, n, size);
		എങ്കിൽ (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			തിരികെ -1;
		}
		addition -= n;
		size += n;
	}
	തിരികെ 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
പൂർണ്ണസംഖ്യ tdb_expand(ഘടന tdb_context *tdb, tdb_off_t size)
{
	ഘടന list_struct rec;
	tdb_off_t offset;

	എങ്കിൽ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		തിരികെ -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	എങ്കിൽ (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	എങ്കിൽ (!(tdb->flags & TDB_INTERNAL)) {
		എങ്കിൽ (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			പോകുക fail;
	}

	tdb->map_size += size;

	എങ്കിൽ (tdb->flags & TDB_INTERNAL) {
		അക്ഷരം *new_map_ptr = (അക്ഷരം *)realloc(tdb->map_ptr,
						    tdb->map_size);
		എങ്കിൽ (!new_map_ptr) {
			tdb->map_size -= size;
			പോകുക fail;
		}
		tdb->map_ptr = new_map_ptr;
	} അല്ലെങ്കിൽ {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',വലുപ്പം(rec));
	rec.rec_len = size - വലുപ്പം(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	എങ്കിൽ (tdb_free(tdb, offset, &rec) == -1)
		പോകുക fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	തിരികെ 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	തിരികെ -1;
}

/* read/write a tdb_off_t */
പൂർണ്ണസംഖ്യ tdb_ofs_read(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	തിരികെ tdb->methods->tdb_read(tdb, offset, (അക്ഷരം*)d, വലുപ്പം(*d), DOCONV());
}

പൂർണ്ണസംഖ്യ tdb_ofs_write(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	തിരികെ tdb->methods->tdb_write(tdb, offset, CONVERT(off), വലുപ്പം(*d));
}


/* read a lump of data, allocating the space for it */
unsigned അക്ഷരം *tdb_alloc_read(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned അക്ഷരം *buf;

	/* some systems don't like zero length malloc */
	എങ്കിൽ (len == 0) {
		len = 1;
	}

	എങ്കിൽ (!(buf = (unsigned അക്ഷരം *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		തിരികെ TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	എങ്കിൽ (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		തിരികെ ശൂന്യ;
	}
	തിരികെ buf;
}

/* Give a piece of tdb data to a parser */

പൂർണ്ണസംഖ്യ tdb_parse_data(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ,
		   tdb_off_t offset, tdb_len_t len,
		   പൂർണ്ണസംഖ്യ (*parser)(TDB_DATA താക്കോൽ, TDB_DATA data,
				 ശൂന്യം *private_data),
		   ശൂന്യം *private_data)
{
	TDB_DATA data;
	പൂർണ്ണസംഖ്യ result;

	data.dsize = len;

	എങ്കിൽ ((tdb->transaction == ശൂന്യ) && (tdb->map_ptr != ശൂന്യ)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		എങ്കിൽ (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			തിരികെ -1;
		}
		data.dptr = offset + (unsigned അക്ഷരം *)tdb->map_ptr;
		തിരികെ parser(താക്കോൽ, data, private_data);
	}

	എങ്കിൽ (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		തിരികെ -1;
	}

	result = parser(താക്കോൽ, data, private_data);
	സ്വതന്ത്രം(data.dptr);
	തിരികെ result;
}

/* read/write a record */
പൂർണ്ണസംഖ്യ tdb_rec_read(ഘടന tdb_context *tdb, tdb_off_t offset, ഘടന list_struct *rec)
{
	എങ്കിൽ (tdb->methods->tdb_read(tdb, offset, rec, വലുപ്പം(*rec),DOCONV()) == -1)
		തിരികെ -1;
	എങ്കിൽ (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		തിരികെ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	തിരികെ tdb->methods->tdb_oob(tdb, rec->next+വലുപ്പം(*rec), 0);
}

പൂർണ്ണസംഖ്യ tdb_rec_write(ഘടന tdb_context *tdb, tdb_off_t offset, ഘടന list_struct *rec)
{
	ഘടന list_struct r = *rec;
	തിരികെ tdb->methods->tdb_write(tdb, offset, CONVERT(r), വലുപ്പം(r));
}

സ്ഥിതം സ്ഥിരം ഘടന tdb_methods io_methods = {
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
ശൂന്യം tdb_io_init(ഘടന tdb_context *tdb)
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

ഘടന tdb_transaction_el {
	ഘടന tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned അക്ഷരം *data;
};

/*
  hold the context of any current transaction
*/
ഘടന tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	സ്ഥിരം ഘടന tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ഘടന tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	പൂർണ്ണസംഖ്യ transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	പൂർണ്ണസംഖ്യ nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_read(ഘടന tdb_context *tdb, tdb_off_t off, ശൂന്യം *buf,
			    tdb_len_t len, പൂർണ്ണസംഖ്യ cv)
{
	ഘടന tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	വേണ്ടി (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		എങ്കിൽ (off+len <= el->offset) {
			തുടരുക;
		}
		എങ്കിൽ (off >= el->offset + el->length) {
			തുടരുക;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		എങ്കിൽ (off < el->offset) {
			partial = el->offset - off;
			എങ്കിൽ (transaction_read(tdb, off, buf, partial, cv) != 0) {
				പോകുക fail;
			}
			len -= partial;
			off += partial;
			buf = (ശൂന്യം *)(partial + (അക്ഷരം *)buf);
		}
		എങ്കിൽ (off + len <= el->offset + el->length) {
			partial = len;
		} അല്ലെങ്കിൽ {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		എങ്കിൽ (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (ശൂന്യം *)(partial + (അക്ഷരം *)buf);

		എങ്കിൽ (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			പോകുക fail;
		}

		തിരികെ 0;
	}

	/* its not in the transaction elements - do a real read */
	തിരികെ tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	തിരികെ -1;
}


/*
  write while in a transaction
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_write(ഘടന tdb_context *tdb, tdb_off_t off,
			     സ്ഥിരം ശൂന്യം *buf, tdb_len_t len)
{
	ഘടന tdb_transaction_el *el, *best_el=ശൂന്യ;

	എങ്കിൽ (len == 0) {
		തിരികെ 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	എങ്കിൽ (len == വലുപ്പം(tdb_off_t) && off >= FREELIST_മുകൾ &&
	    off < FREELIST_മുകൾ+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_മുകൾ) / വലുപ്പം(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	വേണ്ടി (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		എങ്കിൽ (best_el == ശൂന്യ && off == el->offset+el->length) {
			best_el = el;
		}

		എങ്കിൽ (off+len <= el->offset) {
			തുടരുക;
		}
		എങ്കിൽ (off >= el->offset + el->length) {
			തുടരുക;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		എങ്കിൽ (off < el->offset) {
			partial = el->offset - off;
			എങ്കിൽ (transaction_write(tdb, off, buf, partial) != 0) {
				പോകുക fail;
			}
			len -= partial;
			off += partial;
			buf = (സ്ഥിരം ശൂന്യം *)(partial + (സ്ഥിരം അക്ഷരം *)buf);
		}
		എങ്കിൽ (off + len <= el->offset + el->length) {
			partial = len;
		} അല്ലെങ്കിൽ {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (സ്ഥിരം ശൂന്യം *)(partial + (സ്ഥിരം അക്ഷരം *)buf);

		എങ്കിൽ (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			പോകുക fail;
		}

		തിരികെ 0;
	}

	/* see if we can append the new entry to an existing entry */
	എങ്കിൽ (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned അക്ഷരം *data = best_el->data;
		el = best_el;
		el->data = (unsigned അക്ഷരം *)realloc(el->data,
						    el->length + len);
		എങ്കിൽ (el->data == ശൂന്യ) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			തിരികെ -1;
		}
		എങ്കിൽ (buf) {
			memcpy(el->data + el->length, buf, len);
		} അല്ലെങ്കിൽ {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		തിരികെ 0;
	}

	/* add a new entry at the end of the list */
	el = (ഘടന tdb_transaction_el *)malloc(വലുപ്പം(*el));
	എങ്കിൽ (el == ശൂന്യ) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		തിരികെ -1;
	}
	el->next = ശൂന്യ;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned അക്ഷരം *)malloc(len);
	എങ്കിൽ (el->data == ശൂന്യ) {
		സ്വതന്ത്രം(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		തിരികെ -1;
	}
	എങ്കിൽ (buf) {
		memcpy(el->data, buf, len);
	} അല്ലെങ്കിൽ {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	എങ്കിൽ (el->prev) {
		el->prev->next = el;
	} അല്ലെങ്കിൽ {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	തിരികെ 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	തിരികെ -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
സ്ഥിതം ശൂന്യം transaction_next_hash_chain(ഘടന tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	വേണ്ടി (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		എങ്കിൽ (0 != tdb->transaction->hash_heads[h+1]) {
			നിർത്തുക;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_oob(ഘടന tdb_context *tdb, tdb_off_t len,
			   പൂർണ്ണസംഖ്യ probe EXT2FS_ATTR((unused)))
{
	എങ്കിൽ (len <= tdb->map_size) {
		തിരികെ 0;
	}
	തിരികെ TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_expand_file(ഘടന tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	എങ്കിൽ (transaction_write(tdb, size, ശൂന്യ, addition) != 0) {
		തിരികെ -1;
	}

	തിരികെ 0;
}

/*
  brlock during a transaction - ignore them
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_brlock(ഘടന tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      പൂർണ്ണസംഖ്യ rw_type EXT2FS_ATTR((unused)),
			      പൂർണ്ണസംഖ്യ lck_type EXT2FS_ATTR((unused)),
			      പൂർണ്ണസംഖ്യ probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം ഘടന tdb_methods transaction_methods = {
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
പൂർണ്ണസംഖ്യ tdb_transaction_start(ഘടന tdb_context *tdb)
{
	/* some sanity checks */
	എങ്കിൽ (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		തിരികെ -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	എങ്കിൽ (tdb->transaction != ശൂന്യ) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		തിരികെ 0;
	}

	എങ്കിൽ (tdb->അക്കം_locks != 0 || tdb->global_lock.എണ്ണം) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->travlocks.next != ശൂന്യ) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		തിരികെ -1;
	}

	tdb->transaction = (ഘടന tdb_transaction *)
		calloc(വലുപ്പം(ഘടന tdb_transaction), 1);
	എങ്കിൽ (tdb->transaction == ശൂന്യ) {
		tdb->ecode = TDB_ERR_OOM;
		തിരികെ -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	എങ്കിൽ (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		തിരികെ -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	എങ്കിൽ (tdb_brlock(tdb, FREELIST_മുകൾ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		പോകുക fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, വലുപ്പം(u32));
	എങ്കിൽ (tdb->transaction->hash_heads == ശൂന്യ) {
		tdb->ecode = TDB_ERR_OOM;
		പോകുക fail;
	}
	എങ്കിൽ (tdb->methods->tdb_read(tdb, FREELIST_മുകൾ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		പോകുക fail;
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
	എങ്കിൽ (transaction_write(tdb, FREELIST_മുകൾ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		പോകുക fail;
	}

	തിരികെ 0;

fail:
	tdb_brlock(tdb, FREELIST_മുകൾ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	തിരികെ -1;
}


/*
  cancel the current transaction
*/
പൂർണ്ണസംഖ്യ tdb_transaction_cancel(ഘടന tdb_context *tdb)
{
	എങ്കിൽ (tdb->transaction == ശൂന്യ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		തിരികെ 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	ആകുമ്പോൾ (tdb->transaction->elements) {
		ഘടന tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		സ്വതന്ത്രം(el->data);
		സ്വതന്ത്രം(el);
	}

	/* remove any global lock created during the transaction */
	എങ്കിൽ (tdb->global_lock.എണ്ണം != 0) {
		tdb_brlock(tdb, FREELIST_മുകൾ, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.എണ്ണം = 0;
	}

	/* remove any locks created during the transaction */
	എങ്കിൽ (tdb->അക്കം_locks != 0) {
		പൂർണ്ണസംഖ്യ i;
		വേണ്ടി (i=0;i<tdb->അക്കം_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_മുകൾ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->അക്കം_locks = 0;
		tdb->അക്കം_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_മുകൾ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	തിരികെ 0;
}

/*
  sync to disk
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_sync(ഘടന tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	എങ്കിൽ (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		തിരികെ -1;
	}
#എങ്കിൽ defined(HAVE_MSYNC) && defined(MS_SYNC)
	എങ്കിൽ (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		എങ്കിൽ (msync(moffset + (അക്ഷരം *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			തിരികെ -1;
		}
	}
#endif
	തിരികെ 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
സ്ഥിതം tdb_len_t tdb_recovery_size(ഘടന tdb_context *tdb)
{
	ഘടന tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = വലുപ്പം(u32);
	വേണ്ടി (el=tdb->transaction->elements;el;el=el->next) {
		എങ്കിൽ (el->offset >= tdb->transaction->old_map_size) {
			തുടരുക;
		}
		recovery_size += 2*വലുപ്പം(tdb_off_t) + el->length;
	}

	തിരികെ recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_recovery_allocate(ഘടന tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	ഘടന list_struct rec;
	സ്ഥിരം ഘടന tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	എങ്കിൽ (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		തിരികെ -1;
	}

	rec.rec_len = 0;

	എങ്കിൽ (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, വലുപ്പം(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		തിരികെ -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	എങ്കിൽ (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		തിരികെ 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	എങ്കിൽ (recovery_head != 0) {
		എങ്കിൽ (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			തിരികെ -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(വലുപ്പം(rec) + *recovery_size, tdb->page_size) - വലുപ്പം(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	എങ്കിൽ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     വലുപ്പം(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		തിരികെ -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	എങ്കിൽ (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, വലുപ്പം(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		തിരികെ -1;
	}

	തിരികെ 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ transaction_setup_recovery(ഘടന tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	ഘടന tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned അക്ഷരം *data, *p;
	സ്ഥിരം ഘടന tdb_methods *methods = tdb->transaction->io_methods;
	ഘടന list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	എങ്കിൽ (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		തിരികെ -1;
	}

	data = (unsigned അക്ഷരം *)malloc(recovery_size + വലുപ്പം(*rec));
	എങ്കിൽ (data == ശൂന്യ) {
		tdb->ecode = TDB_ERR_OOM;
		തിരികെ -1;
	}

	rec = (ഘടന list_struct *)data;
	memset(rec, 0, വലുപ്പം(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->താക്കോൽ_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + വലുപ്പം(*rec);
	വേണ്ടി (el=tdb->transaction->elements;el;el=el->next) {
		എങ്കിൽ (el->offset >= old_map_size) {
			തുടരുക;
		}
		എങ്കിൽ (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			സ്വതന്ത്രം(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			തിരികെ -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		എങ്കിൽ (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		എങ്കിൽ (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			സ്വതന്ത്രം(data);
			tdb->ecode = TDB_ERR_IO;
			തിരികെ -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = വലുപ്പം(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	എങ്കിൽ (methods->tdb_write(tdb, recovery_offset, data, വലുപ്പം(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		സ്വതന്ത്രം(data);
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	എങ്കിൽ (transaction_sync(tdb, recovery_offset, വലുപ്പം(*rec) + recovery_size) == -1) {
		സ്വതന്ത്രം(data);
		തിരികെ -1;
	}

	സ്വതന്ത്രം(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(ഘടന list_struct, magic);

	എങ്കിൽ (methods->tdb_write(tdb, *magic_offset, &magic, വലുപ്പം(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	/* ensure the recovery magic marker is on disk */
	എങ്കിൽ (transaction_sync(tdb, *magic_offset, വലുപ്പം(magic)) == -1) {
		തിരികെ -1;
	}

	തിരികെ 0;
}

/*
  commit the current transaction
*/
പൂർണ്ണസംഖ്യ tdb_transaction_commit(ഘടന tdb_context *tdb)
{
	സ്ഥിരം ഘടന tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	എങ്കിൽ (tdb->transaction == ശൂന്യ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		തിരികെ 0;
	}

	/* check for a null transaction */
	എങ്കിൽ (tdb->transaction->elements == ശൂന്യ) {
		tdb_transaction_cancel(tdb);
		തിരികെ 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	എങ്കിൽ (tdb->അക്കം_locks || tdb->global_lock.എണ്ണം) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		തിരികെ -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	എങ്കിൽ (tdb_brlock_upgrade(tdb, FREELIST_മുകൾ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		തിരികെ -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	എങ്കിൽ (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		തിരികെ -1;
	}

	എങ്കിൽ (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		എങ്കിൽ (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			തിരികെ -1;
		}
	}

	/* expand the file to the new size if needed */
	എങ്കിൽ (tdb->map_size != tdb->transaction->old_map_size) {
		എങ്കിൽ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			തിരികെ -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	ആകുമ്പോൾ (tdb->transaction->elements) {
		ഘടന tdb_transaction_el *el = tdb->transaction->elements;

		എങ്കിൽ (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			തിരികെ -1;
		}
		tdb->transaction->elements = el->next;
		സ്വതന്ത്രം(el->data);
		സ്വതന്ത്രം(el);
	}

	എങ്കിൽ (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		എങ്കിൽ (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			തിരികെ -1;
		}

		/* remove the recovery marker */
		എങ്കിൽ (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			തിരികെ -1;
		}

		/* ensure the recovery marker has been removed on disk */
		എങ്കിൽ (transaction_sync(tdb, magic_offset, 4) == -1) {
			തിരികെ -1;
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
	utime(tdb->name, ശൂന്യ);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	തിരികെ 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
പൂർണ്ണസംഖ്യ tdb_transaction_recover(ഘടന tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned അക്ഷരം *data, *p;
	u32 zero = 0;
	ഘടന list_struct rec;

	/* find the recovery area */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	എങ്കിൽ (recovery_head == 0) {
		/* we have never allocated a recovery record */
		തിരികെ 0;
	}

	/* read the recovery record */
	എങ്കിൽ (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   വലുപ്പം(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	എങ്കിൽ (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		തിരികെ 0;
	}

	എങ്കിൽ (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		തിരികെ -1;
	}

	recovery_eof = rec.താക്കോൽ_len;

	data = (unsigned അക്ഷരം *)malloc(rec.data_len);
	എങ്കിൽ (data == ശൂന്യ) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		തിരികെ -1;
	}

	/* read the full recovery data */
	എങ്കിൽ (tdb->methods->tdb_read(tdb, recovery_head + വലുപ്പം(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		സ്വതന്ത്രം(data);
		തിരികെ -1;
	}

	/* recover the file data */
	p = data;
	ആകുമ്പോൾ (p+8 < data + rec.data_len) {
		u32 ofs, len;
		എങ്കിൽ (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		എങ്കിൽ (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			സ്വതന്ത്രം(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			തിരികെ -1;
		}
		p += 8 + len;
	}

	സ്വതന്ത്രം(data);

	എങ്കിൽ (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	എങ്കിൽ (recovery_eof <= recovery_head) {
		എങ്കിൽ (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			തിരികെ -1;
		}
	}

	/* remove the recovery magic */
	എങ്കിൽ (tdb_ofs_write(tdb, recovery_head + offsetof(ഘടന list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	എങ്കിൽ (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	എങ്കിൽ (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		തിരികെ -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	തിരികെ 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_rec_free_read(ഘടന tdb_context *tdb, tdb_off_t off, ഘടന list_struct *rec)
{
	എങ്കിൽ (tdb->methods->tdb_read(tdb, off, rec, വലുപ്പം(*rec),DOCONV()) == -1)
		തിരികെ -1;

	എങ്കിൽ (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		എങ്കിൽ (tdb->methods->tdb_write(tdb, off, rec, വലുപ്പം(*rec)) == -1)
			തിരികെ -1;
	}

	എങ്കിൽ (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		തിരികെ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	എങ്കിൽ (tdb->methods->tdb_oob(tdb, rec->next+വലുപ്പം(*rec), 0) != 0)
		തിരികെ -1;
	തിരികെ 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
സ്ഥിതം പൂർണ്ണസംഖ്യ remove_from_freelist(ഘടന tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_മുകൾ;
	ആകുമ്പോൾ (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		എങ്കിൽ (i == off) {
			/* We've found it! */
			തിരികെ tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	തിരികെ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
സ്ഥിതം പൂർണ്ണസംഖ്യ update_tailer(ഘടന tdb_context *tdb, tdb_off_t offset,
			 സ്ഥിരം ഘടന list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = വലുപ്പം(*rec) + rec->rec_len;
	തിരികെ tdb_ofs_write(tdb, offset + totalsize - വലുപ്പം(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
പൂർണ്ണസംഖ്യ tdb_free(ഘടന tdb_context *tdb, tdb_off_t offset, ഘടന list_struct *rec)
{
	tdb_off_t വലത്, ഇടത്;

	/* Allocation and tailer lock */
	എങ്കിൽ (tdb_lock(tdb, -1, F_WRLCK) != 0)
		തിരികെ -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	എങ്കിൽ (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		പോകുക fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	വലത് = offset + വലുപ്പം(*rec) + rec->rec_len;
	എങ്കിൽ (വലത് + വലുപ്പം(*rec) <= tdb->map_size) {
		ഘടന list_struct r;

		എങ്കിൽ (tdb->methods->tdb_read(tdb, വലത്, &r, വലുപ്പം(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", വലത്));
			പോകുക ഇടത്;
		}

		/* If it's free, expand to include it. */
		എങ്കിൽ (r.magic == TDB_FREE_MAGIC) {
			എങ്കിൽ (remove_from_freelist(tdb, വലത്, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", വലത്));
				പോകുക ഇടത്;
			}
			rec->rec_len += വലുപ്പം(r) + r.rec_len;
		}
	}

ഇടത്:
	/* Look left */
	ഇടത് = offset - വലുപ്പം(tdb_off_t);
	എങ്കിൽ (ഇടത് > TDB_DATA_START(tdb->header.hash_size)) {
		ഘടന list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		എങ്കിൽ (tdb_ofs_read(tdb, ഇടത്, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ഇടത്));
			പോകുക update;
		}

		/* it could be uninitialised data */
		എങ്കിൽ (leftsize == 0 || leftsize == TDB_PAD_U32) {
			പോകുക update;
		}

		ഇടത് = offset - leftsize;

		/* Now read in record */
		എങ്കിൽ (tdb->methods->tdb_read(tdb, ഇടത്, &l, വലുപ്പം(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ഇടത്, leftsize));
			പോകുക update;
		}

		/* If it's free, expand to include it. */
		എങ്കിൽ (l.magic == TDB_FREE_MAGIC) {
			എങ്കിൽ (remove_from_freelist(tdb, ഇടത്, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ഇടത്));
				പോകുക update;
			} അല്ലെങ്കിൽ {
				offset = ഇടത്;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	എങ്കിൽ (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		പോകുക fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	എങ്കിൽ (tdb_ofs_read(tdb, FREELIST_മുകൾ, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_മുകൾ, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		പോകുക fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	തിരികെ 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	തിരികെ -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
സ്ഥിതം tdb_off_t tdb_allocate_ofs(ഘടന tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				ഘടന list_struct *rec, tdb_off_t last_ptr)
{
	ഘടന list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', വലുപ്പം(newrec));

	/* found it - now possibly split it up  */
	എങ്കിൽ (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (വലുപ്പം(*rec) + length);
		newrec_ptr = rec_ptr + വലുപ്പം(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} അല്ലെങ്കിൽ {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	എങ്കിൽ (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		തിരികെ 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	എങ്കിൽ (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		തിരികെ 0;
	}

	/* Did we create new block? */
	എങ്കിൽ (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		എങ്കിൽ (update_tailer(tdb, rec_ptr, rec) == -1) {
			തിരികെ 0;
		}

		/* Free new record */
		എങ്കിൽ (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			തിരികെ 0;
		}
	}

	/* all done - return the new record offset */
	തിരികെ rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ഘടന tdb_context *tdb, tdb_len_t length, ഘടന list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	ഘടന {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	എങ്കിൽ (tdb_lock(tdb, -1, F_WRLCK) == -1)
		തിരികെ 0;

	/* Extra bytes required for tailer */
	length += വലുപ്പം(tdb_off_t);

 again:
	last_ptr = FREELIST_മുകൾ;

	/* read in the freelist top */
	എങ്കിൽ (tdb_ofs_read(tdb, FREELIST_മുകൾ, &rec_ptr) == -1)
		പോകുക fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	ആകുമ്പോൾ (rec_ptr) {
		എങ്കിൽ (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			പോകുക fail;
		}

		എങ്കിൽ (rec->rec_len >= length) {
			എങ്കിൽ (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				എങ്കിൽ (bestfit.rec_len < 2*length) {
					നിർത്തുക;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	എങ്കിൽ (bestfit.rec_ptr != 0) {
		എങ്കിൽ (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			പോകുക fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		തിരികെ newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	എങ്കിൽ (tdb_expand(tdb, length + വലുപ്പം(*rec)) == 0)
		പോകുക again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	തിരികെ 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

സ്ഥിതം പൂർണ്ണസംഖ്യ seen_തിരുകുക(ഘടന tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA താക്കോൽ, data;

	memset(&data, '\0', വലുപ്പം(data));
	താക്കോൽ.dptr = (unsigned അക്ഷരം *)&rec_ptr;
	താക്കോൽ.dsize = വലുപ്പം(rec_ptr);
	തിരികെ tdb_store(mem_tdb, താക്കോൽ, data, TDB_തിരുകുക);
}

പൂർണ്ണസംഖ്യ tdb_validate_freelist(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ *pnum_entries)
{
	ഘടന tdb_context *mem_tdb = ശൂന്യ;
	ഘടന list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	പൂർണ്ണസംഖ്യ ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	എങ്കിൽ (!mem_tdb) {
		തിരികെ -1;
	}

	എങ്കിൽ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		തിരികെ 0;
	}

	last_ptr = FREELIST_മുകൾ;

	/* Store the FREELIST_TOP record. */
	എങ്കിൽ (seen_തിരുകുക(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		പോകുക fail;
	}

	/* read in the freelist top */
	എങ്കിൽ (tdb_ofs_read(tdb, FREELIST_മുകൾ, &rec_ptr) == -1) {
		പോകുക fail;
	}

	ആകുമ്പോൾ (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		എങ്കിൽ (seen_തിരുകുക(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			പോകുക fail;
		}

		എങ്കിൽ (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			പോകുക fail;
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
	തിരികെ ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_next_lock(ഘടന tdb_context *tdb, ഘടന tdb_traverse_lock *tlock,
			 ഘടന list_struct *rec)
{
	പൂർണ്ണസംഖ്യ want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	വേണ്ടി (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		എങ്കിൽ (!tlock->off && tlock->hash != 0) {
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
			എങ്കിൽ (tlock->hash == tdb->header.hash_size) {
				തുടരുക;
			}
		}

		എങ്കിൽ (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			തിരികെ -1;

		/* No previous record?  Start at top of chain. */
		എങ്കിൽ (!tlock->off) {
			എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(tlock->hash),
				     &tlock->off) == -1)
				പോകുക fail;
		} അല്ലെങ്കിൽ {
			/* Otherwise unlock the previous record. */
			എങ്കിൽ (tdb_unlock_record(tdb, tlock->off) != 0)
				പോകുക fail;
		}

		എങ്കിൽ (want_next) {
			/* We have offset of old record: grab next */
			എങ്കിൽ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				പോകുക fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		ആകുമ്പോൾ( tlock->off) {
			tdb_off_t current;
			എങ്കിൽ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				പോകുക fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			എങ്കിൽ (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				പോകുക fail;
			}

			എങ്കിൽ (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				എങ്കിൽ (tdb_lock_record(tdb, tlock->off) != 0)
					പോകുക fail;
				തിരികെ tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			എങ്കിൽ (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				പോകുക fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	തിരികെ TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	എങ്കിൽ (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	തിരികെ -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_traverse_internal(ഘടന tdb_context *tdb,
				 tdb_traverse_func fn, ശൂന്യം *private_data,
				 ഘടന tdb_traverse_lock *tl)
{
	TDB_DATA താക്കോൽ, dbuf;
	ഘടന list_struct rec;
	പൂർണ്ണസംഖ്യ ret, എണ്ണം = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	ആകുമ്പോൾ ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		എണ്ണം++;
		/* now read the full record */
		താക്കോൽ.dptr = tdb_alloc_read(tdb, tl->off + വലുപ്പം(rec),
					  rec.താക്കോൽ_len + rec.data_len);
		എങ്കിൽ (!താക്കോൽ.dptr) {
			ret = -1;
			എങ്കിൽ (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				പോകുക out;
			എങ്കിൽ (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			പോകുക out;
		}
		താക്കോൽ.dsize = rec.താക്കോൽ_len;
		dbuf.dptr = താക്കോൽ.dptr + rec.താക്കോൽ_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		എങ്കിൽ (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(താക്കോൽ.dptr);
			പോകുക out;
		}
		എങ്കിൽ (fn && fn(tdb, താക്കോൽ, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = എണ്ണം;
			എങ്കിൽ (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(താക്കോൽ.dptr);
			പോകുക out;
		}
		SAFE_FREE(താക്കോൽ.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	എങ്കിൽ (ret < 0)
		തിരികെ -1;
	അല്ലെങ്കിൽ
		തിരികെ എണ്ണം;
}


/*
  a write style traverse - temporarily marks the db read only
*/
പൂർണ്ണസംഖ്യ tdb_traverse_read(ഘടന tdb_context *tdb,
		      tdb_traverse_func fn, ശൂന്യം *private_data)
{
	ഘടന tdb_traverse_lock tl = { ശൂന്യ, 0, 0, F_RDLCK };
	പൂർണ്ണസംഖ്യ ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	എങ്കിൽ (tdb_transaction_lock(tdb, F_RDLCK)) {
		തിരികെ -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	തിരികെ ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
പൂർണ്ണസംഖ്യ tdb_traverse(ഘടന tdb_context *tdb,
		 tdb_traverse_func fn, ശൂന്യം *private_data)
{
	ഘടന tdb_traverse_lock tl = { ശൂന്യ, 0, 0, F_WRLCK };
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (tdb->read_only || tdb->traverse_read) {
		തിരികെ tdb_traverse_read(tdb, fn, private_data);
	}

	എങ്കിൽ (tdb_transaction_lock(tdb, F_WRLCK)) {
		തിരികെ -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	തിരികെ ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(ഘടന tdb_context *tdb)
{
	TDB_DATA താക്കോൽ;
	ഘടന list_struct rec;

	/* release any old lock */
	എങ്കിൽ (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		തിരികെ tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	എങ്കിൽ (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		തിരികെ tdb_null;
	/* now read the key */
	താക്കോൽ.dsize = rec.താക്കോൽ_len;
	താക്കോൽ.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+വലുപ്പം(rec),താക്കോൽ.dsize);

	/* Unlock the hash chain of the record we just read. */
	എങ്കിൽ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	തിരികെ താക്കോൽ;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(ഘടന tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA താക്കോൽ = tdb_null;
	ഘടന list_struct rec;
	unsigned അക്ഷരം *k = ശൂന്യ;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	എങ്കിൽ (tdb->travlocks.off) {
		എങ്കിൽ (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			തിരികെ tdb_null;
		എങ്കിൽ (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+വലുപ്പം(rec),
					    rec.താക്കോൽ_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			എങ്കിൽ (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				തിരികെ tdb_null;
			}
			എങ്കിൽ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				തിരികെ tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	എങ്കിൽ (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		എങ്കിൽ (!tdb->travlocks.off)
			തിരികെ tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		എങ്കിൽ (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			തിരികെ tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	എങ്കിൽ (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		താക്കോൽ.dsize = rec.താക്കോൽ_len;
		താക്കോൽ.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+വലുപ്പം(rec),
					  താക്കോൽ.dsize);
		/* Unlock the chain of this new record */
		എങ്കിൽ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	എങ്കിൽ (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	തിരികെ താക്കോൽ;
}

/* file: dump.c */

സ്ഥിതം tdb_off_t tdb_dump_record(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ hash,
				 tdb_off_t offset)
{
	ഘടന list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	എങ്കിൽ (tdb->methods->tdb_read(tdb, offset, (അക്ഷരം *)&rec,
				   വലുപ്പം(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		തിരികെ 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.താക്കോൽ_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + വലുപ്പം(rec) + rec.rec_len - വലുപ്പം(tdb_off_t);

	എങ്കിൽ (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		തിരികെ rec.next;
	}

	എങ്കിൽ (tailer != rec.rec_len + വലുപ്പം(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned പൂർണ്ണസംഖ്യ)tailer, (unsigned പൂർണ്ണസംഖ്യ)(rec.rec_len + വലുപ്പം(rec)));
	}
	തിരികെ rec.next;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_dump_chain(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ i)
{
	tdb_off_t rec_ptr, മുകൾ;

	മുകൾ = TDB_HASH_മുകൾ(i);

	എങ്കിൽ (tdb_lock(tdb, i, F_WRLCK) != 0)
		തിരികെ -1;

	എങ്കിൽ (tdb_ofs_read(tdb, മുകൾ, &rec_ptr) == -1)
		തിരികെ tdb_unlock(tdb, i, F_WRLCK);

	എങ്കിൽ (rec_ptr)
		printf("hash=%d\n", i);

	ആകുമ്പോൾ (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	തിരികെ tdb_unlock(tdb, i, F_WRLCK);
}

ശൂന്യം tdb_dump_all(ഘടന tdb_context *tdb)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < (പൂർണ്ണസംഖ്യ)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

പൂർണ്ണസംഖ്യ tdb_printfreelist(ഘടന tdb_context *tdb)
{
	പൂർണ്ണസംഖ്യ ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	ഘടന list_struct rec;

	എങ്കിൽ ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		തിരികെ ret;

	offset = FREELIST_മുകൾ;

	/* read in the freelist top */
	എങ്കിൽ (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		തിരികെ 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	ആകുമ്പോൾ (rec_ptr) {
		എങ്കിൽ (tdb->methods->tdb_read(tdb, rec_ptr, (അക്ഷരം *)&rec,
					   വലുപ്പം(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			തിരികെ -1;
		}

		എങ്കിൽ (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			തിരികെ -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (പൂർണ്ണസംഖ്യ)total_free,
               (പൂർണ്ണസംഖ്യ)total_free);

	തിരികെ tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ശൂന്യം tdb_increment_seqnum_nonblock(ഘടന tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	എങ്കിൽ (!(tdb->flags & TDB_SEQNUM)) {
		തിരികെ;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		തിരികെ;
	seqnum++;
	(ശൂന്യം) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
സ്ഥിതം ശൂന്യം tdb_increment_seqnum(ഘടന tdb_context *tdb)
{
	എങ്കിൽ (!(tdb->flags & TDB_SEQNUM)) {
		തിരികെ;
	}

	എങ്കിൽ (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		തിരികെ;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_താക്കോൽ_compare(TDB_DATA താക്കോൽ, TDB_DATA data,
			   ശൂന്യം *private_data EXT2FS_ATTR((unused)))
{
	തിരികെ memcmp(data.dptr, താക്കോൽ.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
സ്ഥിതം tdb_off_t tdb_find(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, u32 hash,
			ഘടന list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(hash), &rec_ptr) == -1)
		തിരികെ 0;

	/* keep looking until we find the right record */
	ആകുമ്പോൾ (rec_ptr) {
		എങ്കിൽ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			തിരികെ 0;

		എങ്കിൽ (!TDB_DEAD(r) && hash==r->full_hash
		    && താക്കോൽ.dsize==r->താക്കോൽ_len
		    && tdb_parse_data(tdb, താക്കോൽ, rec_ptr + വലുപ്പം(*r),
				      r->താക്കോൽ_len, tdb_താക്കോൽ_compare,
				      ശൂന്യ) == 0) {
			തിരികെ rec_ptr;
		}
		rec_ptr = r->next;
	}
	തിരികെ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, u32 hash, പൂർണ്ണസംഖ്യ locktype,
			   ഘടന list_struct *rec)
{
	u32 rec_ptr;

	എങ്കിൽ (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		തിരികെ 0;
	എങ്കിൽ (!(rec_ptr = tdb_find(tdb, താക്കോൽ, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	തിരികെ rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_update_hash(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, u32 hash, TDB_DATA dbuf)
{
	ഘടന list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	എങ്കിൽ (!(rec_ptr = tdb_find(tdb, താക്കോൽ, hash, &rec)))
		തിരികെ -1;

	/* must be long enough key, data and tailer */
	എങ്കിൽ (rec.rec_len < താക്കോൽ.dsize + dbuf.dsize + വലുപ്പം(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		തിരികെ -1;
	}

	എങ്കിൽ (tdb->methods->tdb_write(tdb, rec_ptr + വലുപ്പം(rec) + rec.താക്കോൽ_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		തിരികെ -1;

	എങ്കിൽ (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		തിരികെ tdb_rec_write(tdb, rec_ptr, &rec);
	}

	തിരികെ 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	tdb_off_t rec_ptr;
	ഘടന list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&താക്കോൽ);
	എങ്കിൽ (!(rec_ptr = tdb_find_lock_hash(tdb,താക്കോൽ,hash,F_RDLCK,&rec)))
		തിരികെ tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + വലുപ്പം(rec) + rec.താക്കോൽ_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	തിരികെ ret;
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

പൂർണ്ണസംഖ്യ tdb_parse_record(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ,
		     പൂർണ്ണസംഖ്യ (*parser)(TDB_DATA താക്കോൽ, TDB_DATA data,
				   ശൂന്യം *private_data),
		     ശൂന്യം *private_data)
{
	tdb_off_t rec_ptr;
	ഘടന list_struct rec;
	പൂർണ്ണസംഖ്യ ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&താക്കോൽ);

	എങ്കിൽ (!(rec_ptr = tdb_find_lock_hash(tdb,താക്കോൽ,hash,F_RDLCK,&rec))) {
		തിരികെ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, താക്കോൽ, rec_ptr + വലുപ്പം(rec) + rec.താക്കോൽ_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	തിരികെ ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_exists_hash(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, u32 hash)
{
	ഘടന list_struct rec;

	എങ്കിൽ (tdb_find_lock_hash(tdb, താക്കോൽ, hash, F_RDLCK, &rec) == 0)
		തിരികെ 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	തിരികെ 1;
}

പൂർണ്ണസംഖ്യ tdb_exists(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	u32 hash = tdb->hash_fn(&താക്കോൽ);
	തിരികെ tdb_exists_hash(tdb, താക്കോൽ, hash);
}

/* actually delete an entry in the database given the offset */
പൂർണ്ണസംഖ്യ tdb_do_delete(ഘടന tdb_context *tdb, tdb_off_t rec_ptr, ഘടന list_struct*rec)
{
	tdb_off_t last_ptr, i;
	ഘടന list_struct lastrec;

	എങ്കിൽ (tdb->read_only || tdb->traverse_read) തിരികെ -1;

	എങ്കിൽ (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		തിരികെ tdb_rec_write(tdb, rec_ptr, rec);
	}
	എങ്കിൽ (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		തിരികെ -1;

	/* find previous record in hash chain */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(rec->full_hash), &i) == -1)
		തിരികെ -1;
	വേണ്ടി (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		എങ്കിൽ (tdb_rec_read(tdb, i, &lastrec) == -1)
			തിരികെ -1;

	/* unlink it: next ptr is at start of record. */
	എങ്കിൽ (last_ptr == 0)
		last_ptr = TDB_HASH_മുകൾ(rec->full_hash);
	എങ്കിൽ (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		തിരികെ -1;

	/* recover the space */
	എങ്കിൽ (tdb_free(tdb, rec_ptr, rec) == -1)
		തിരികെ -1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_എണ്ണം_dead(ഘടന tdb_context *tdb, u32 hash)
{
	പൂർണ്ണസംഖ്യ res = 0;
	tdb_off_t rec_ptr;
	ഘടന list_struct rec;

	/* read in the hash top */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(hash), &rec_ptr) == -1)
		തിരികെ 0;

	ആകുമ്പോൾ (rec_ptr) {
		എങ്കിൽ (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			തിരികെ 0;

		എങ്കിൽ (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	തിരികെ res;
}

/*
 * Purge all DEAD records from a hash chain
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_purge_dead(ഘടന tdb_context *tdb, u32 hash)
{
	പൂർണ്ണസംഖ്യ res = -1;
	ഘടന list_struct rec;
	tdb_off_t rec_ptr;

	എങ്കിൽ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		തിരികെ -1;
	}

	/* read in the hash top */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(hash), &rec_ptr) == -1)
		പോകുക fail;

	ആകുമ്പോൾ (rec_ptr) {
		tdb_off_t next;

		എങ്കിൽ (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			പോകുക fail;
		}

		next = rec.next;

		എങ്കിൽ (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			പോകുക fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	തിരികെ res;
}

/* delete an entry in the database given a key */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_delete_hash(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, u32 hash)
{
	tdb_off_t rec_ptr;
	ഘടന list_struct rec;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		എങ്കിൽ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			തിരികെ -1;

		എങ്കിൽ (tdb_എണ്ണം_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		എങ്കിൽ (!(rec_ptr = tdb_find(tdb, താക്കോൽ, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			തിരികെ -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	അല്ലെങ്കിൽ {
		എങ്കിൽ (!(rec_ptr = tdb_find_lock_hash(tdb, താക്കോൽ, hash, F_WRLCK,
						   &rec)))
			തിരികെ -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	എങ്കിൽ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	എങ്കിൽ (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ tdb_delete(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ)
{
	u32 hash = tdb->hash_fn(&താക്കോൽ);
	തിരികെ tdb_delete_hash(tdb, താക്കോൽ, hash);
}

/*
 * See if we have a dead record around with enough space
 */
സ്ഥിതം tdb_off_t tdb_find_dead(ഘടന tdb_context *tdb, u32 hash,
			       ഘടന list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(hash), &rec_ptr) == -1)
		തിരികെ 0;

	/* keep looking until we find the right record */
	ആകുമ്പോൾ (rec_ptr) {
		എങ്കിൽ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			തിരികെ 0;

		എങ്കിൽ (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			തിരികെ rec_ptr;
		}
		rec_ptr = r->next;
	}
	തിരികെ 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
പൂർണ്ണസംഖ്യ tdb_store(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, പൂർണ്ണസംഖ്യ flag)
{
	ഘടന list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	അക്ഷരം *p = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = -1;

	എങ്കിൽ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		തിരികെ -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&താക്കോൽ);
	എങ്കിൽ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		തിരികെ -1;

	/* check for it existing, on insert. */
	എങ്കിൽ (flag == TDB_തിരുകുക) {
		എങ്കിൽ (tdb_exists_hash(tdb, താക്കോൽ, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			പോകുക fail;
		}
	} അല്ലെങ്കിൽ {
		/* first try in-place update, on modify or replace. */
		എങ്കിൽ (tdb_update_hash(tdb, താക്കോൽ, hash, dbuf) == 0) {
			പോകുക done;
		}
		എങ്കിൽ (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			പോകുക fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	എങ്കിൽ (flag != TDB_തിരുകുക)
		tdb_delete_hash(tdb, താക്കോൽ, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	എങ്കിൽ (!(p = (അക്ഷരം *)malloc(താക്കോൽ.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		പോകുക fail;
	}

	memcpy(p, താക്കോൽ.dptr, താക്കോൽ.dsize);
	എങ്കിൽ (dbuf.dsize)
		memcpy(p+താക്കോൽ.dsize, dbuf.dptr, dbuf.dsize);

	എങ്കിൽ (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			താക്കോൽ.dsize + dbuf.dsize + വലുപ്പം(tdb_off_t));

		എങ്കിൽ (rec_ptr != 0) {
			rec.താക്കോൽ_len = താക്കോൽ.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			എങ്കിൽ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + വലുപ്പം(rec),
				    p, താക്കോൽ.dsize + dbuf.dsize) == -1) {
				പോകുക fail;
			}
			പോകുക done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	എങ്കിൽ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		പോകുക fail;
	}

	എങ്കിൽ ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		പോകുക fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, താക്കോൽ.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	എങ്കിൽ (rec_ptr == 0) {
		പോകുക fail;
	}

	/* Read hash top into next ptr */
	എങ്കിൽ (tdb_ofs_read(tdb, TDB_HASH_മുകൾ(hash), &rec.next) == -1)
		പോകുക fail;

	rec.താക്കോൽ_len = താക്കോൽ.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	എങ്കിൽ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+വലുപ്പം(rec), p, താക്കോൽ.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_മുകൾ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		പോകുക fail;
	}

 done:
	ret = 0;
 fail:
	എങ്കിൽ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	തിരികെ ret;
}


/* Append to an entry. Create if not exist. */
പൂർണ്ണസംഖ്യ tdb_append(ഘടന tdb_context *tdb, TDB_DATA താക്കോൽ, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	പൂർണ്ണസംഖ്യ ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&താക്കോൽ);
	എങ്കിൽ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		തിരികെ -1;

	dbuf = tdb_fetch(tdb, താക്കോൽ);

	എങ്കിൽ (dbuf.dptr == ശൂന്യ) {
		dbuf.dptr = (unsigned അക്ഷരം *)malloc(new_dbuf.dsize);
	} അല്ലെങ്കിൽ {
		unsigned അക്ഷരം *new_dptr = (unsigned അക്ഷരം *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		എങ്കിൽ (new_dptr == ശൂന്യ) {
			സ്വതന്ത്രം(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	എങ്കിൽ (dbuf.dptr == ശൂന്യ) {
		tdb->ecode = TDB_ERR_OOM;
		പോകുക failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, താക്കോൽ, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	തിരികെ ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
സ്ഥിരം അക്ഷരം *tdb_name(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
പൂർണ്ണസംഖ്യ tdb_fd(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->log.log_fn;
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
പൂർണ്ണസംഖ്യ tdb_get_seqnum(ഘടന tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	എങ്കിൽ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		തിരികെ 0;
	തിരികെ seqnum;
}

പൂർണ്ണസംഖ്യ tdb_hash_size(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->header.hash_size;
}

size_t tdb_map_size(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->map_size;
}

പൂർണ്ണസംഖ്യ tdb_get_flags(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
ശൂന്യം tdb_enable_seqnum(ഘടന tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
സ്ഥിതം ഘടന tdb_context *tdbs = ശൂന്യ;


/* This is from a hash algorithm suggested by Rogier Wolff */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ default_tdb_hash(TDB_DATA *താക്കോൽ)
{
	u32 മൂല്യം;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	വേണ്ടി (മൂല്യം = 0, i=0; i < താക്കോൽ->dsize; i++)
		മൂല്യം = മൂല്യം * 256 + താക്കോൽ->dptr[i] + (മൂല്യം >> 24) * 241;

	തിരികെ മൂല്യം;
}


/* initialise a new database with a specified hash size */
സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_new_database(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ hash_size)
{
	ഘടന tdb_header *newdb;
	പൂർണ്ണസംഖ്യ size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = വലുപ്പം(ഘടന tdb_header) + (hash_size+1)*വലുപ്പം(tdb_off_t);
	എങ്കിൽ (!(newdb = (ഘടന tdb_header *)calloc(size, 1)))
		തിരികെ TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	എങ്കിൽ (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (അക്ഷരം *)newdb;
		memcpy(&tdb->header, newdb, വലുപ്പം(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		തിരികെ 0;
	}
	എങ്കിൽ (lseek(tdb->fd, 0, SEEK_SET) == -1)
		പോകുക fail;

	എങ്കിൽ (ftruncate(tdb->fd, 0) == -1)
		പോകുക fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, വലുപ്പം(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	എങ്കിൽ (ലിഖ(tdb->fd, newdb, size) != size) {
		ret = -1;
	} അല്ലെങ്കിൽ {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	തിരികെ ret;
}



സ്ഥിതം പൂർണ്ണസംഖ്യ tdb_already_open(dev_t device,
			    ino_t ino)
{
	ഘടന tdb_context *i;

	വേണ്ടി (i = tdbs; i; i = i->next) {
		എങ്കിൽ (i->device == device && i->inode == ino) {
			തിരികെ 1;
		}
	}

	തിരികെ 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ഘടന tdb_context *tdb_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ hash_size, പൂർണ്ണസംഖ്യ tdb_flags,
		      പൂർണ്ണസംഖ്യ open_flags, mode_t mode)
{
	തിരികെ tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, ശൂന്യ, ശൂന്യ);
}

/* a default logging function */
സ്ഥിതം ശൂന്യം null_log_fn(ഘടന tdb_context *tdb, എണ്ണൽ tdb_debug_level level, സ്ഥിരം അക്ഷരം *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
സ്ഥിതം ശൂന്യം null_log_fn(ഘടന tdb_context *tdb EXT2FS_ATTR((unused)),
			എണ്ണൽ tdb_debug_level level EXT2FS_ATTR((unused)),
			സ്ഥിരം അക്ഷരം *fmt EXT2FS_ATTR((unused)), ...)
{
}


ഘടന tdb_context *tdb_open_ex(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ hash_size, പൂർണ്ണസംഖ്യ tdb_flags,
				പൂർണ്ണസംഖ്യ open_flags, mode_t mode,
				സ്ഥിരം ഘടന tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ഘടന tdb_context *tdb;
	ഘടന stat st;
	പൂർണ്ണസംഖ്യ rev = 0, locked = 0;
	unsigned അക്ഷരം *vp;
	u32 vertest;

	എങ്കിൽ (!(tdb = (ഘടന tdb_context *)calloc(1, വലുപ്പം *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		പോകുക fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ശൂന്യ;
	tdb->map_ptr = ശൂന്യ;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	എങ്കിൽ (log_ctx) {
		tdb->log = *log_ctx;
	} അല്ലെങ്കിൽ {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = ശൂന്യ;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	എങ്കിൽ (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	എങ്കിൽ ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		പോകുക fail;
	}

	എങ്കിൽ (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	എങ്കിൽ ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	എങ്കിൽ (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		എങ്കിൽ (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			പോകുക fail;
		}
		പോകുക internal;
	}

	എങ്കിൽ ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		പോകുക fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	എങ്കിൽ (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		പോകുക fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	എങ്കിൽ ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		എങ്കിൽ (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			പോകുക fail; /* errno set by ftruncate */
		}
	}

	എങ്കിൽ (പഠ(tdb->fd, &tdb->header, വലുപ്പം(tdb->header)) != വലുപ്പം(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      വലുപ്പം(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		എങ്കിൽ (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			പോകുക fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned അക്ഷരം *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	എങ്കിൽ (!rev)
		tdb->flags &= ~TDB_CONVERT;
	അല്ലെങ്കിൽ {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, വലുപ്പം(tdb->header));
	}
	എങ്കിൽ (fstat(tdb->fd, &st) == -1)
		പോകുക fail;

	എങ്കിൽ (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		പോകുക fail;
	}

	/* Is it already in the open list?  If so, fail. */
	എങ്കിൽ (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (പൂർണ്ണസംഖ്യ)st.st_dev, (പൂർണ്ണസംഖ്യ)st.st_ino));
		errno = EBUSY;
		പോകുക fail;
	}

	എങ്കിൽ (!(tdb->name = (അക്ഷരം *)strdup(name))) {
		errno = ENOMEM;
		പോകുക fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	എങ്കിൽ (locked) {
		എങ്കിൽ (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			പോകുക fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	എങ്കിൽ (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		എങ്കിൽ (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			പോകുക fail;
	}

	/* if needed, run recovery */
	എങ്കിൽ (tdb_transaction_recover(tdb) == -1) {
		പോകുക fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	എങ്കിൽ (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		പോകുക fail;
	tdb->next = tdbs;
	tdbs = tdb;
	തിരികെ tdb;

 fail:
	{ പൂർണ്ണസംഖ്യ save_errno = errno;

	എങ്കിൽ (!tdb)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (tdb->map_ptr) {
		എങ്കിൽ (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		അല്ലെങ്കിൽ
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	എങ്കിൽ (tdb->fd != -1)
		എങ്കിൽ (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	തിരികെ ശൂന്യ;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

ശൂന്യം tdb_set_max_dead(ഘടന tdb_context *tdb, പൂർണ്ണസംഖ്യ max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
പൂർണ്ണസംഖ്യ tdb_close(ഘടന tdb_context *tdb)
{
	ഘടന tdb_context **i;
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	എങ്കിൽ (tdb->map_ptr) {
		എങ്കിൽ (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		അല്ലെങ്കിൽ
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	എങ്കിൽ (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	വേണ്ടി (i = &tdbs; *i; i = &(*i)->next) {
		എങ്കിൽ (*i == tdb) {
			*i = tdb->next;
			നിർത്തുക;
		}
	}

	memset(tdb, 0, വലുപ്പം(*tdb));
	SAFE_FREE(tdb);

	തിരികെ ret;
}

/* register a logging function */
ശൂന്യം tdb_set_logging_function(ഘടന tdb_context *tdb,
                              സ്ഥിരം ഘടന tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

ശൂന്യം *tdb_get_logging_private(ഘടന tdb_context *tdb)
{
	തിരികെ tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
പൂർണ്ണസംഖ്യ tdb_reopen(ഘടന tdb_context *tdb)
{
	ഘടന stat st;

	എങ്കിൽ (tdb->flags & TDB_INTERNAL) {
		തിരികെ 0; /* Nothing to do. */
	}

	എങ്കിൽ (tdb->അക്കം_locks != 0 || tdb->global_lock.എണ്ണം) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		പോകുക fail;
	}

	എങ്കിൽ (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		പോകുക fail;
	}

	എങ്കിൽ (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		പോകുക fail;
	}
	എങ്കിൽ (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	എങ്കിൽ (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		പോകുക fail;
	}
	എങ്കിൽ ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		പോകുക fail;
	}
	എങ്കിൽ (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		പോകുക fail;
	}
	എങ്കിൽ (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		പോകുക fail;
	}
	tdb_mmap(tdb);

	തിരികെ 0;

fail:
	tdb_close(tdb);
	തിരികെ -1;
}

/* reopen all tdb's */
പൂർണ്ണസംഖ്യ tdb_reopen_all(പൂർണ്ണസംഖ്യ parent_longlived)
{
	ഘടന tdb_context *tdb;

	വേണ്ടി (tdb=tdbs; tdb; tdb = tdb->next) {
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
		എങ്കിൽ (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		എങ്കിൽ (tdb_reopen(tdb) != 0)
			തിരികെ -1;
	}

	തിരികെ 0;
}

/**
 * Flush a database file from the page cache.
 **/
പൂർണ്ണസംഖ്യ tdb_flush(ഘടന tdb_context *tdb)
{
	എങ്കിൽ (tdb->fd != -1)
		തിരികെ fsync(tdb->fd);
	തിരികെ 0;
}
