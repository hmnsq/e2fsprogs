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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <string.h>
#ifdef HAVE_SYS_SELECT_H
#અન્તર્ભાવ <sys/select.h>
#endif
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <time.h>
#ifdef HAVE_UTIME_H
#અન્તર્ભાવ <utime.h>
#endif
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/file.h>
#અન્તર્ભાવ <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#અન્તર્ભાવ <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#નહીંતર
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((શૂન્ય *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
સ્થિર અક્ષર *rep_strdup(અચલ અક્ષર *s)
{
	અક્ષર *ret;
	પૂર્ણાંક length;

	જો (!s)
		પાછા ખાલી;
	length = strlen(s);
	ret = malloc(length + 1);
	જો (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	પાછા ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#જો (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#નહીંતર
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#જો defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
પ્રકારવ્યાખ્યા પૂર્ણાંક bool;
#endif

#અન્તર્ભાવ "tdb.h"

સ્થિર TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

પ્રકારવ્યાખ્યા u32 tdb_len_t;
પ્રકારવ્યાખ્યા u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned પૂર્ણાંક)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*કદ(સંરચના list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_ટોચ (કદ(સંરચના tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_ટોચ(hash) (FREELIST_ટોચ + (BUCKET(hash)+1)*કદ(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*કદ(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_ટોચ(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(સંરચના tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(સંરચના tdb_header, sequence_સંખ્યા)
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
#define SAFE_FREE(x) કરો { જો ((x) != ખાલી) {મુક્ત(x); (x)=ખાલી;} } જ્યારે(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, કદ(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
સંરચના list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t ચાવી_len; /* byte length of key */
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
સંરચના tdb_header {
	અક્ષર magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_સંખ્યા; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

સંરચના tdb_lock_type {
	પૂર્ણાંક list;
	u32 ગણતરી;
	u32 ltype;
};

સંરચના tdb_traverse_lock {
	સંરચના tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	પૂર્ણાંક lock_rw;
};


સંરચના tdb_methods {
	પૂર્ણાંક (*tdb_read)(સંરચના tdb_context *, tdb_off_t , શૂન્ય *, tdb_len_t , પૂર્ણાંક );
	પૂર્ણાંક (*tdb_write)(સંરચના tdb_context *, tdb_off_t, અચલ શૂન્ય *, tdb_len_t);
	શૂન્ય (*next_hash_chain)(સંરચના tdb_context *, u32 *);
	પૂર્ણાંક (*tdb_oob)(સંરચના tdb_context *, tdb_off_t , પૂર્ણાંક );
	પૂર્ણાંક (*tdb_expand_file)(સંરચના tdb_context *, tdb_off_t , tdb_off_t );
	પૂર્ણાંક (*tdb_brlock)(સંરચના tdb_context *, tdb_off_t , પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક, size_t);
};

સંરચના tdb_context {
	અક્ષર *name; /* the name of the database */
	શૂન્ય *map_ptr; /* where it is currently mapped */
	પૂર્ણાંક fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	પૂર્ણાંક read_only; /* opened read-only */
	પૂર્ણાંક traverse_read; /* read-only traversal */
	સંરચના tdb_lock_type global_lock;
	પૂર્ણાંક અંક_lockrecs;
	સંરચના tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	ગણના TDB_ERROR ecode; /* error code for last tdb error */
	સંરચના tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	સંરચના tdb_traverse_lock travlocks; /* current traversal locks */
	સંરચના tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	સંરચના tdb_logging_context log;
	unsigned પૂર્ણાંક (*hash_fn)(TDB_DATA *ચાવી);
	પૂર્ણાંક open_flags; /* flags used in the open - needed by reopen */
	unsigned પૂર્ણાંક અંક_locks; /* number of chain locks held */
	અચલ સંરચના tdb_methods *methods;
	સંરચના tdb_transaction *transaction;
	પૂર્ણાંક page_size;
	પૂર્ણાંક max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
સ્થિર પૂર્ણાંક tdb_munmap(સંરચના tdb_context *tdb);
સ્થિર શૂન્ય tdb_mmap(સંરચના tdb_context *tdb);
સ્થિર પૂર્ણાંક tdb_lock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype);
પૂર્ણાંક tdb_lock_nonblock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype);
સ્થિર પૂર્ણાંક tdb_unlock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype);
સ્થિર પૂર્ણાંક tdb_brlock(સંરચના tdb_context *tdb, tdb_off_t offset, પૂર્ણાંક rw_type, પૂર્ણાંક lck_type, પૂર્ણાંક probe, size_t len);
સ્થિર પૂર્ણાંક tdb_transaction_lock(સંરચના tdb_context *tdb, પૂર્ણાંક ltype);
સ્થિર પૂર્ણાંક tdb_transaction_unlock(સંરચના tdb_context *tdb);
સ્થિર પૂર્ણાંક tdb_brlock_upgrade(સંરચના tdb_context *tdb, tdb_off_t offset, size_t len);
સ્થિર પૂર્ણાંક tdb_write_lock_record(સંરચના tdb_context *tdb, tdb_off_t off);
સ્થિર પૂર્ણાંક tdb_write_unlock_record(સંરચના tdb_context *tdb, tdb_off_t off);
સ્થિર પૂર્ણાંક tdb_ofs_read(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
સ્થિર પૂર્ણાંક tdb_ofs_write(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
સ્થિર શૂન્ય *tdb_convert(શૂન્ય *buf, u32 size);
સ્થિર પૂર્ણાંક tdb_free(સંરચના tdb_context *tdb, tdb_off_t offset, સંરચના list_struct *rec);
સ્થિર tdb_off_t tdb_allocate(સંરચના tdb_context *tdb, tdb_len_t length, સંરચના list_struct *rec);
સ્થિર પૂર્ણાંક tdb_ofs_read(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
સ્થિર પૂર્ણાંક tdb_ofs_write(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
સ્થિર પૂર્ણાંક tdb_lock_record(સંરચના tdb_context *tdb, tdb_off_t off);
સ્થિર પૂર્ણાંક tdb_unlock_record(સંરચના tdb_context *tdb, tdb_off_t off);
સ્થિર પૂર્ણાંક tdb_rec_read(સંરચના tdb_context *tdb, tdb_off_t offset, સંરચના list_struct *rec);
સ્થિર પૂર્ણાંક tdb_rec_write(સંરચના tdb_context *tdb, tdb_off_t offset, સંરચના list_struct *rec);
સ્થિર પૂર્ણાંક tdb_do_delete(સંરચના tdb_context *tdb, tdb_off_t rec_ptr, સંરચના list_struct *rec);
સ્થિર unsigned અક્ષર *tdb_alloc_read(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
સ્થિર પૂર્ણાંક tdb_parse_data(સંરચના tdb_context *tdb, TDB_DATA ચાવી,
		   tdb_off_t offset, tdb_len_t len,
		   પૂર્ણાંક (*parser)(TDB_DATA ચાવી, TDB_DATA data,
				 શૂન્ય *private_data),
		   શૂન્ય *private_data);
સ્થિર tdb_off_t tdb_find_lock_hash(સંરચના tdb_context *tdb, TDB_DATA ચાવી, u32 hash, પૂર્ણાંક locktype,
			   સંરચના list_struct *rec);
સ્થિર શૂન્ય tdb_io_init(સંરચના tdb_context *tdb);
સ્થિર પૂર્ણાંક tdb_expand(સંરચના tdb_context *tdb, tdb_off_t size);
સ્થિર પૂર્ણાંક tdb_rec_free_read(સંરચના tdb_context *tdb, tdb_off_t off,
		      સંરચના list_struct *rec);


/* file: error.c */

ગણના TDB_ERROR tdb_error(સંરચના tdb_context *tdb)
{
	પાછા tdb->ecode;
}

સ્થિર સંરચના tdb_errname {
	ગણના TDB_ERROR ecode; અચલ અક્ષર *estring;
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
અચલ અક્ષર *tdb_errorstr(સંરચના tdb_context *tdb)
{
	u32 i;
	માટે (i = 0; i < કદ(emap) / કદ(સંરચના tdb_errname); i++)
		જો (tdb->ecode == emap[i].ecode)
			પાછા emap[i].estring;
	પાછા "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
પૂર્ણાંક tdb_brlock(સંરચના tdb_context *tdb, tdb_off_t offset,
	       પૂર્ણાંક rw_type, પૂર્ણાંક lck_type, પૂર્ણાંક probe, size_t len)
{
	સંરચના flock fl;
	પૂર્ણાંક ret;

	જો (tdb->flags & TDB_NOLOCK) {
		પાછા 0;
	}

	જો ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		પાછા -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	કરો {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} જ્યારે (ret == -1 && errno == EINTR);

	જો (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		જો (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (પૂર્ણાંક)len));
		}
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	પાછા 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
પૂર્ણાંક tdb_brlock_upgrade(સંરચના tdb_context *tdb, tdb_off_t offset, size_t len)
{
	પૂર્ણાંક ગણતરી = 1000;
	જ્યારે (ગણતરી--) {
		સંરચના timeval tv;
		જો (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			પાછા 0;
		}
		જો (errno != EDEADLK) {
			તોડો;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ખાલી, ખાલી, ખાલી, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	પાછા -1;
}


/* lock a list in the database. list -1 is the alloc list */
સ્થિર પૂર્ણાંક _tdb_lock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype, પૂર્ણાંક op)
{
	સંરચના tdb_lock_type *new_lck;
	પૂર્ણાંક i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	જો (tdb->global_lock.ગણતરી &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		પાછા 0;
	}

	જો (tdb->global_lock.ગણતરી) {
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	જો (list < -1 || list >= (પૂર્ણાંક)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		પાછા -1;
	}
	જો (tdb->flags & TDB_NOLOCK)
		પાછા 0;

	માટે (i=0; i<tdb->અંક_lockrecs; i++) {
		જો (tdb->lockrecs[i].list == list) {
			જો (tdb->lockrecs[i].ગણતરી == 0) {
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
			tdb->lockrecs[i].ગણતરી++;
			પાછા 0;
		}
	}

	new_lck = (સંરચના tdb_lock_type *)realloc(
		tdb->lockrecs,
		કદ(*tdb->lockrecs) * (tdb->અંક_lockrecs+1));
	જો (new_lck == ખાલી) {
		errno = ENOMEM;
		પાછા -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	જો (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_ટોચ+4*list, ltype, op,
				     0, 1)) {
		પાછા -1;
	}

	tdb->અંક_locks++;

	tdb->lockrecs[tdb->અંક_lockrecs].list = list;
	tdb->lockrecs[tdb->અંક_lockrecs].ગણતરી = 1;
	tdb->lockrecs[tdb->અંક_lockrecs].ltype = ltype;
	tdb->અંક_lockrecs += 1;

	પાછા 0;
}

/* lock a list in the database. list -1 is the alloc list */
પૂર્ણાંક tdb_lock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype)
{
	પૂર્ણાંક ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	જો (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	પાછા ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
પૂર્ણાંક tdb_lock_nonblock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype)
{
	પાછા _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
પૂર્ણાંક tdb_unlock(સંરચના tdb_context *tdb, પૂર્ણાંક list, પૂર્ણાંક ltype)
{
	પૂર્ણાંક ret = -1;
	પૂર્ણાંક i;
	સંરચના tdb_lock_type *lck = ખાલી;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	જો (tdb->global_lock.ગણતરી &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		પાછા 0;
	}

	જો (tdb->global_lock.ગણતરી) {
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	જો (tdb->flags & TDB_NOLOCK)
		પાછા 0;

	/* Sanity checks */
	જો (list < -1 || list >= (પૂર્ણાંક)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		પાછા ret;
	}

	માટે (i=0; i<tdb->અંક_lockrecs; i++) {
		જો (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			તોડો;
		}
	}

	જો ((lck == ખાલી) || (lck->ગણતરી == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		પાછા -1;
	}

	જો (lck->ગણતરી > 1) {
		lck->ગણતરી--;
		પાછા 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	જો (mark_lock) {
		ret = 0;
	} નહીંતર {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_ટોચ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->અંક_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	જો (tdb->અંક_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->અંક_lockrecs-1];
	}
	tdb->અંક_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	જો (tdb->અંક_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	જો (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	પાછા ret;
}

/*
  get the transaction lock
 */
પૂર્ણાંક tdb_transaction_lock(સંરચના tdb_context *tdb, પૂર્ણાંક ltype)
{
	જો (tdb->have_transaction_lock || tdb->global_lock.ગણતરી) {
		પાછા 0;
	}
	જો (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		પાછા -1;
	}
	tdb->have_transaction_lock = 1;
	પાછા 0;
}

/*
  release the transaction lock
 */
પૂર્ણાંક tdb_transaction_unlock(સંરચના tdb_context *tdb)
{
	પૂર્ણાંક ret;
	જો (!tdb->have_transaction_lock) {
		પાછા 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	જો (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	પાછા ret;
}




/* lock/unlock entire database */
સ્થિર પૂર્ણાંક _tdb_lockall(સંરચના tdb_context *tdb, પૂર્ણાંક ltype, પૂર્ણાંક op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	જો (tdb->read_only || tdb->traverse_read)
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);

	જો (tdb->global_lock.ગણતરી && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.ગણતરી++;
		પાછા 0;
	}

	જો (tdb->global_lock.ગણતરી) {
		/* a global lock of a different type exists */
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	જો (tdb->અંક_locks != 0) {
		/* can't combine global and chain locks */
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	જો (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ટોચ, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		જો (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		પાછા -1;
	}

	tdb->global_lock.ગણતરી = 1;
	tdb->global_lock.ltype = ltype;

	પાછા 0;
}



/* unlock entire db */
સ્થિર પૂર્ણાંક _tdb_unlockall(સંરચના tdb_context *tdb, પૂર્ણાંક ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	જો (tdb->read_only || tdb->traverse_read) {
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	જો (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.ગણતરી == 0) {
		પાછા TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	જો (tdb->global_lock.ગણતરી > 1) {
		tdb->global_lock.ગણતરી--;
		પાછા 0;
	}

	જો (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ટોચ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		પાછા -1;
	}

	tdb->global_lock.ગણતરી = 0;
	tdb->global_lock.ltype = 0;

	પાછા 0;
}

/* lock entire database with write lock */
પૂર્ણાંક tdb_lockall(સંરચના tdb_context *tdb)
{
	પાછા _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
પૂર્ણાંક tdb_lockall_mark(સંરચના tdb_context *tdb)
{
	પાછા _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
પૂર્ણાંક tdb_lockall_unmark(સંરચના tdb_context *tdb)
{
	પાછા _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
પૂર્ણાંક tdb_lockall_nonblock(સંરચના tdb_context *tdb)
{
	પાછા _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
પૂર્ણાંક tdb_unlockall(સંરચના tdb_context *tdb)
{
	પાછા _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
પૂર્ણાંક tdb_lockall_read(સંરચના tdb_context *tdb)
{
	પાછા _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
પૂર્ણાંક tdb_lockall_read_nonblock(સંરચના tdb_context *tdb)
{
	પાછા _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
પૂર્ણાંક tdb_unlockall_read(સંરચના tdb_context *tdb)
{
	પાછા _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
પૂર્ણાંક tdb_chainlock(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_lock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
પૂર્ણાંક tdb_chainlock_nonblock(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
પૂર્ણાંક tdb_chainlock_mark(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_lock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
પૂર્ણાંક tdb_chainlock_unmark(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_WRLCK | TDB_MARK_LOCK);
}

પૂર્ણાંક tdb_chainunlock(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_WRLCK);
}

પૂર્ણાંક tdb_chainlock_read(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_lock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_RDLCK);
}

પૂર્ણાંક tdb_chainunlock_read(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	પાછા tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ચાવી)), F_RDLCK);
}



/* record lock stops delete underneath */
પૂર્ણાંક tdb_lock_record(સંરચના tdb_context *tdb, tdb_off_t off)
{
	પાછા off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
પૂર્ણાંક tdb_write_lock_record(સંરચના tdb_context *tdb, tdb_off_t off)
{
	સંરચના tdb_traverse_lock *i;
	માટે (i = &tdb->travlocks; i; i = i->next)
		જો (i->off == off)
			પાછા -1;
	પાછા tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
પૂર્ણાંક tdb_write_unlock_record(સંરચના tdb_context *tdb, tdb_off_t off)
{
	પાછા tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
પૂર્ણાંક tdb_unlock_record(સંરચના tdb_context *tdb, tdb_off_t off)
{
	સંરચના tdb_traverse_lock *i;
	u32 ગણતરી = 0;

	જો (off == 0)
		પાછા 0;
	માટે (i = &tdb->travlocks; i; i = i->next)
		જો (i->off == off)
			ગણતરી++;
	પાછા (ગણતરી == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
સ્થિર પૂર્ણાંક tdb_oob(સંરચના tdb_context *tdb, tdb_off_t len, પૂર્ણાંક probe)
{
	સંરચના stat st;
	જો (len <= tdb->map_size)
		પાછા 0;
	જો (tdb->flags & TDB_INTERNAL) {
		જો (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (પૂર્ણાંક)len, (પૂર્ણાંક)tdb->map_size));
		}
		પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	જો (fstat(tdb->fd, &st) == -1) {
		પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	જો (st.st_size < (off_t)len) {
		જો (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (પૂર્ણાંક)len, (પૂર્ણાંક)st.st_size));
		}
		પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	જો (tdb_munmap(tdb) == -1)
		પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	પાછા 0;
}

/* write a lump of data at a specified offset */
સ્થિર પૂર્ણાંક tdb_write(સંરચના tdb_context *tdb, tdb_off_t off,
		     અચલ શૂન્ય *buf, tdb_len_t len)
{
	જો (len == 0) {
		પાછા 0;
	}

	જો (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		પાછા -1;
	}

	જો (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		પાછા -1;

	જો (tdb->map_ptr) {
		memcpy(off + (અક્ષર *)tdb->map_ptr, buf, len);
	} નહીંતર જો (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	પાછા 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
શૂન્ય *tdb_convert(શૂન્ય *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	માટે (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	પાછા buf;
}


/* read a lump of data at a specified offset, maybe convert */
સ્થિર પૂર્ણાંક tdb_read(સંરચના tdb_context *tdb, tdb_off_t off, શૂન્ય *buf,
		    tdb_len_t len, પૂર્ણાંક cv)
{
	જો (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		પાછા -1;
	}

	જો (tdb->map_ptr) {
		memcpy(buf, off + (અક્ષર *)tdb->map_ptr, len);
	} નહીંતર {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		જો (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (પૂર્ણાંક)off, (પૂર્ણાંક)len, (પૂર્ણાંક)ret, strerror(errno),
				 (પૂર્ણાંક)tdb->map_size));
			પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	જો (cv) {
		tdb_convert(buf, len);
	}
	પાછા 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
સ્થિર શૂન્ય tdb_next_hash_chain(સંરચના tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	જો (tdb->map_ptr) {
		માટે (;h < tdb->header.hash_size;h++) {
			જો (0 != *(u32 *)(TDB_HASH_ટોચ(h) + (unsigned અક્ષર *)tdb->map_ptr)) {
				તોડો;
			}
		}
	} નહીંતર {
		u32 off=0;
		માટે (;h < tdb->header.hash_size;h++) {
			જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(h), &off) != 0 || off != 0) {
				તોડો;
			}
		}
	}
	(*chain) = h;
}


પૂર્ણાંક tdb_munmap(સંરચના tdb_context *tdb)
{
	જો (tdb->flags & TDB_INTERNAL)
		પાછા 0;

#ifdef HAVE_MMAP
	જો (tdb->map_ptr) {
		પૂર્ણાંક ret = munmap(tdb->map_ptr, tdb->real_map_size);
		જો (ret != 0)
			પાછા ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = ખાલી;
	પાછા 0;
}

શૂન્ય tdb_mmap(સંરચના tdb_context *tdb)
{
	જો (tdb->flags & TDB_INTERNAL)
		પાછા;

#ifdef HAVE_MMAP
	જો (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ખાલી, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		જો (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = ખાલી;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} નહીંતર {
		tdb->map_ptr = ખાલી;
	}
#નહીંતર
	tdb->map_ptr = ખાલી;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
સ્થિર પૂર્ણાંક tdb_expand_file(સંરચના tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	અક્ષર buf[1024];

	જો (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		પાછા -1;
	}

	જો (ftruncate(tdb->fd, size+addition) == -1) {
		અક્ષર b = 0;
		જો (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			પાછા -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, કદ(buf));
	જ્યારે (addition) {
		પૂર્ણાંક n = addition>કદ(buf)?કદ(buf):addition;
		પૂર્ણાંક ret = pwrite(tdb->fd, buf, n, size);
		જો (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			પાછા -1;
		}
		addition -= n;
		size += n;
	}
	પાછા 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
પૂર્ણાંક tdb_expand(સંરચના tdb_context *tdb, tdb_off_t size)
{
	સંરચના list_struct rec;
	tdb_off_t offset;

	જો (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		પાછા -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	જો (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	જો (!(tdb->flags & TDB_INTERNAL)) {
		જો (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			જાઓ fail;
	}

	tdb->map_size += size;

	જો (tdb->flags & TDB_INTERNAL) {
		અક્ષર *new_map_ptr = (અક્ષર *)realloc(tdb->map_ptr,
						    tdb->map_size);
		જો (!new_map_ptr) {
			tdb->map_size -= size;
			જાઓ fail;
		}
		tdb->map_ptr = new_map_ptr;
	} નહીંતર {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',કદ(rec));
	rec.rec_len = size - કદ(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	જો (tdb_free(tdb, offset, &rec) == -1)
		જાઓ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	પાછા 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	પાછા -1;
}

/* read/write a tdb_off_t */
પૂર્ણાંક tdb_ofs_read(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	પાછા tdb->methods->tdb_read(tdb, offset, (અક્ષર*)d, કદ(*d), DOCONV());
}

પૂર્ણાંક tdb_ofs_write(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	પાછા tdb->methods->tdb_write(tdb, offset, CONVERT(off), કદ(*d));
}


/* read a lump of data, allocating the space for it */
unsigned અક્ષર *tdb_alloc_read(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned અક્ષર *buf;

	/* some systems don't like zero length malloc */
	જો (len == 0) {
		len = 1;
	}

	જો (!(buf = (unsigned અક્ષર *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		પાછા TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	જો (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		પાછા ખાલી;
	}
	પાછા buf;
}

/* Give a piece of tdb data to a parser */

પૂર્ણાંક tdb_parse_data(સંરચના tdb_context *tdb, TDB_DATA ચાવી,
		   tdb_off_t offset, tdb_len_t len,
		   પૂર્ણાંક (*parser)(TDB_DATA ચાવી, TDB_DATA data,
				 શૂન્ય *private_data),
		   શૂન્ય *private_data)
{
	TDB_DATA data;
	પૂર્ણાંક result;

	data.dsize = len;

	જો ((tdb->transaction == ખાલી) && (tdb->map_ptr != ખાલી)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		જો (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			પાછા -1;
		}
		data.dptr = offset + (unsigned અક્ષર *)tdb->map_ptr;
		પાછા parser(ચાવી, data, private_data);
	}

	જો (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		પાછા -1;
	}

	result = parser(ચાવી, data, private_data);
	મુક્ત(data.dptr);
	પાછા result;
}

/* read/write a record */
પૂર્ણાંક tdb_rec_read(સંરચના tdb_context *tdb, tdb_off_t offset, સંરચના list_struct *rec)
{
	જો (tdb->methods->tdb_read(tdb, offset, rec, કદ(*rec),DOCONV()) == -1)
		પાછા -1;
	જો (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		પાછા TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	પાછા tdb->methods->tdb_oob(tdb, rec->next+કદ(*rec), 0);
}

પૂર્ણાંક tdb_rec_write(સંરચના tdb_context *tdb, tdb_off_t offset, સંરચના list_struct *rec)
{
	સંરચના list_struct r = *rec;
	પાછા tdb->methods->tdb_write(tdb, offset, CONVERT(r), કદ(r));
}

સ્થિર અચલ સંરચના tdb_methods io_methods = {
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
શૂન્ય tdb_io_init(સંરચના tdb_context *tdb)
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

સંરચના tdb_transaction_el {
	સંરચના tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned અક્ષર *data;
};

/*
  hold the context of any current transaction
*/
સંરચના tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	અચલ સંરચના tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	સંરચના tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	પૂર્ણાંક transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	પૂર્ણાંક nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
સ્થિર પૂર્ણાંક transaction_read(સંરચના tdb_context *tdb, tdb_off_t off, શૂન્ય *buf,
			    tdb_len_t len, પૂર્ણાંક cv)
{
	સંરચના tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	માટે (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		જો (off+len <= el->offset) {
			ચાલુ;
		}
		જો (off >= el->offset + el->length) {
			ચાલુ;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		જો (off < el->offset) {
			partial = el->offset - off;
			જો (transaction_read(tdb, off, buf, partial, cv) != 0) {
				જાઓ fail;
			}
			len -= partial;
			off += partial;
			buf = (શૂન્ય *)(partial + (અક્ષર *)buf);
		}
		જો (off + len <= el->offset + el->length) {
			partial = len;
		} નહીંતર {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		જો (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (શૂન્ય *)(partial + (અક્ષર *)buf);

		જો (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			જાઓ fail;
		}

		પાછા 0;
	}

	/* its not in the transaction elements - do a real read */
	પાછા tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	પાછા -1;
}


/*
  write while in a transaction
*/
સ્થિર પૂર્ણાંક transaction_write(સંરચના tdb_context *tdb, tdb_off_t off,
			     અચલ શૂન્ય *buf, tdb_len_t len)
{
	સંરચના tdb_transaction_el *el, *best_el=ખાલી;

	જો (len == 0) {
		પાછા 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	જો (len == કદ(tdb_off_t) && off >= FREELIST_ટોચ &&
	    off < FREELIST_ટોચ+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_ટોચ) / કદ(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	માટે (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		જો (best_el == ખાલી && off == el->offset+el->length) {
			best_el = el;
		}

		જો (off+len <= el->offset) {
			ચાલુ;
		}
		જો (off >= el->offset + el->length) {
			ચાલુ;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		જો (off < el->offset) {
			partial = el->offset - off;
			જો (transaction_write(tdb, off, buf, partial) != 0) {
				જાઓ fail;
			}
			len -= partial;
			off += partial;
			buf = (અચલ શૂન્ય *)(partial + (અચલ અક્ષર *)buf);
		}
		જો (off + len <= el->offset + el->length) {
			partial = len;
		} નહીંતર {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (અચલ શૂન્ય *)(partial + (અચલ અક્ષર *)buf);

		જો (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			જાઓ fail;
		}

		પાછા 0;
	}

	/* see if we can append the new entry to an existing entry */
	જો (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned અક્ષર *data = best_el->data;
		el = best_el;
		el->data = (unsigned અક્ષર *)realloc(el->data,
						    el->length + len);
		જો (el->data == ખાલી) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			પાછા -1;
		}
		જો (buf) {
			memcpy(el->data + el->length, buf, len);
		} નહીંતર {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		પાછા 0;
	}

	/* add a new entry at the end of the list */
	el = (સંરચના tdb_transaction_el *)malloc(કદ(*el));
	જો (el == ખાલી) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		પાછા -1;
	}
	el->next = ખાલી;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned અક્ષર *)malloc(len);
	જો (el->data == ખાલી) {
		મુક્ત(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		પાછા -1;
	}
	જો (buf) {
		memcpy(el->data, buf, len);
	} નહીંતર {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	જો (el->prev) {
		el->prev->next = el;
	} નહીંતર {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	પાછા 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	પાછા -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
સ્થિર શૂન્ય transaction_next_hash_chain(સંરચના tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	માટે (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		જો (0 != tdb->transaction->hash_heads[h+1]) {
			તોડો;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
સ્થિર પૂર્ણાંક transaction_oob(સંરચના tdb_context *tdb, tdb_off_t len,
			   પૂર્ણાંક probe EXT2FS_ATTR((unused)))
{
	જો (len <= tdb->map_size) {
		પાછા 0;
	}
	પાછા TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
સ્થિર પૂર્ણાંક transaction_expand_file(સંરચના tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	જો (transaction_write(tdb, size, ખાલી, addition) != 0) {
		પાછા -1;
	}

	પાછા 0;
}

/*
  brlock during a transaction - ignore them
*/
સ્થિર પૂર્ણાંક transaction_brlock(સંરચના tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      પૂર્ણાંક rw_type EXT2FS_ATTR((unused)),
			      પૂર્ણાંક lck_type EXT2FS_ATTR((unused)),
			      પૂર્ણાંક probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	પાછા 0;
}

સ્થિર અચલ સંરચના tdb_methods transaction_methods = {
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
પૂર્ણાંક tdb_transaction_start(સંરચના tdb_context *tdb)
{
	/* some sanity checks */
	જો (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		પાછા -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	જો (tdb->transaction != ખાલી) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		પાછા 0;
	}

	જો (tdb->અંક_locks != 0 || tdb->global_lock.ગણતરી) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		પાછા -1;
	}

	જો (tdb->travlocks.next != ખાલી) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		પાછા -1;
	}

	tdb->transaction = (સંરચના tdb_transaction *)
		calloc(કદ(સંરચના tdb_transaction), 1);
	જો (tdb->transaction == ખાલી) {
		tdb->ecode = TDB_ERR_OOM;
		પાછા -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	જો (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		પાછા -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	જો (tdb_brlock(tdb, FREELIST_ટોચ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		જાઓ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, કદ(u32));
	જો (tdb->transaction->hash_heads == ખાલી) {
		tdb->ecode = TDB_ERR_OOM;
		જાઓ fail;
	}
	જો (tdb->methods->tdb_read(tdb, FREELIST_ટોચ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		જાઓ fail;
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
	જો (transaction_write(tdb, FREELIST_ટોચ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		જાઓ fail;
	}

	પાછા 0;

fail:
	tdb_brlock(tdb, FREELIST_ટોચ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	પાછા -1;
}


/*
  cancel the current transaction
*/
પૂર્ણાંક tdb_transaction_cancel(સંરચના tdb_context *tdb)
{
	જો (tdb->transaction == ખાલી) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		પાછા -1;
	}

	જો (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		પાછા 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	જ્યારે (tdb->transaction->elements) {
		સંરચના tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		મુક્ત(el->data);
		મુક્ત(el);
	}

	/* remove any global lock created during the transaction */
	જો (tdb->global_lock.ગણતરી != 0) {
		tdb_brlock(tdb, FREELIST_ટોચ, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.ગણતરી = 0;
	}

	/* remove any locks created during the transaction */
	જો (tdb->અંક_locks != 0) {
		પૂર્ણાંક i;
		માટે (i=0;i<tdb->અંક_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_ટોચ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->અંક_locks = 0;
		tdb->અંક_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_ટોચ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	પાછા 0;
}

/*
  sync to disk
*/
સ્થિર પૂર્ણાંક transaction_sync(સંરચના tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	જો (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		પાછા -1;
	}
#જો defined(HAVE_MSYNC) && defined(MS_SYNC)
	જો (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		જો (msync(moffset + (અક્ષર *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			પાછા -1;
		}
	}
#endif
	પાછા 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
સ્થિર tdb_len_t tdb_recovery_size(સંરચના tdb_context *tdb)
{
	સંરચના tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = કદ(u32);
	માટે (el=tdb->transaction->elements;el;el=el->next) {
		જો (el->offset >= tdb->transaction->old_map_size) {
			ચાલુ;
		}
		recovery_size += 2*કદ(tdb_off_t) + el->length;
	}

	પાછા recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
સ્થિર પૂર્ણાંક tdb_recovery_allocate(સંરચના tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	સંરચના list_struct rec;
	અચલ સંરચના tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	જો (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		પાછા -1;
	}

	rec.rec_len = 0;

	જો (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, કદ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		પાછા -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	જો (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		પાછા 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	જો (recovery_head != 0) {
		જો (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			પાછા -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(કદ(rec) + *recovery_size, tdb->page_size) - કદ(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	જો (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     કદ(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		પાછા -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	જો (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, કદ(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		પાછા -1;
	}

	પાછા 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
સ્થિર પૂર્ણાંક transaction_setup_recovery(સંરચના tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	સંરચના tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned અક્ષર *data, *p;
	અચલ સંરચના tdb_methods *methods = tdb->transaction->io_methods;
	સંરચના list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	જો (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		પાછા -1;
	}

	data = (unsigned અક્ષર *)malloc(recovery_size + કદ(*rec));
	જો (data == ખાલી) {
		tdb->ecode = TDB_ERR_OOM;
		પાછા -1;
	}

	rec = (સંરચના list_struct *)data;
	memset(rec, 0, કદ(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->ચાવી_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + કદ(*rec);
	માટે (el=tdb->transaction->elements;el;el=el->next) {
		જો (el->offset >= old_map_size) {
			ચાલુ;
		}
		જો (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			મુક્ત(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			પાછા -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		જો (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		જો (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			મુક્ત(data);
			tdb->ecode = TDB_ERR_IO;
			પાછા -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = કદ(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	જો (methods->tdb_write(tdb, recovery_offset, data, કદ(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		મુક્ત(data);
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	જો (transaction_sync(tdb, recovery_offset, કદ(*rec) + recovery_size) == -1) {
		મુક્ત(data);
		પાછા -1;
	}

	મુક્ત(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(સંરચના list_struct, magic);

	જો (methods->tdb_write(tdb, *magic_offset, &magic, કદ(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	/* ensure the recovery magic marker is on disk */
	જો (transaction_sync(tdb, *magic_offset, કદ(magic)) == -1) {
		પાછા -1;
	}

	પાછા 0;
}

/*
  commit the current transaction
*/
પૂર્ણાંક tdb_transaction_commit(સંરચના tdb_context *tdb)
{
	અચલ સંરચના tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	જો (tdb->transaction == ખાલી) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		પાછા -1;
	}

	જો (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		પાછા -1;
	}

	જો (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		પાછા 0;
	}

	/* check for a null transaction */
	જો (tdb->transaction->elements == ખાલી) {
		tdb_transaction_cancel(tdb);
		પાછા 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	જો (tdb->અંક_locks || tdb->global_lock.ગણતરી) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		પાછા -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	જો (tdb_brlock_upgrade(tdb, FREELIST_ટોચ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		પાછા -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	જો (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		પાછા -1;
	}

	જો (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		જો (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			પાછા -1;
		}
	}

	/* expand the file to the new size if needed */
	જો (tdb->map_size != tdb->transaction->old_map_size) {
		જો (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			પાછા -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	જ્યારે (tdb->transaction->elements) {
		સંરચના tdb_transaction_el *el = tdb->transaction->elements;

		જો (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			પાછા -1;
		}
		tdb->transaction->elements = el->next;
		મુક્ત(el->data);
		મુક્ત(el);
	}

	જો (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		જો (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			પાછા -1;
		}

		/* remove the recovery marker */
		જો (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			પાછા -1;
		}

		/* ensure the recovery marker has been removed on disk */
		જો (transaction_sync(tdb, magic_offset, 4) == -1) {
			પાછા -1;
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
	utime(tdb->name, ખાલી);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	પાછા 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
પૂર્ણાંક tdb_transaction_recover(સંરચના tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned અક્ષર *data, *p;
	u32 zero = 0;
	સંરચના list_struct rec;

	/* find the recovery area */
	જો (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	જો (recovery_head == 0) {
		/* we have never allocated a recovery record */
		પાછા 0;
	}

	/* read the recovery record */
	જો (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   કદ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	જો (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		પાછા 0;
	}

	જો (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		પાછા -1;
	}

	recovery_eof = rec.ચાવી_len;

	data = (unsigned અક્ષર *)malloc(rec.data_len);
	જો (data == ખાલી) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		પાછા -1;
	}

	/* read the full recovery data */
	જો (tdb->methods->tdb_read(tdb, recovery_head + કદ(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		મુક્ત(data);
		પાછા -1;
	}

	/* recover the file data */
	p = data;
	જ્યારે (p+8 < data + rec.data_len) {
		u32 ofs, len;
		જો (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		જો (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			મુક્ત(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			પાછા -1;
		}
		p += 8 + len;
	}

	મુક્ત(data);

	જો (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	જો (recovery_eof <= recovery_head) {
		જો (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			પાછા -1;
		}
	}

	/* remove the recovery magic */
	જો (tdb_ofs_write(tdb, recovery_head + offsetof(સંરચના list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	જો (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	જો (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		પાછા -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	પાછા 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
સ્થિર પૂર્ણાંક tdb_rec_free_read(સંરચના tdb_context *tdb, tdb_off_t off, સંરચના list_struct *rec)
{
	જો (tdb->methods->tdb_read(tdb, off, rec, કદ(*rec),DOCONV()) == -1)
		પાછા -1;

	જો (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		જો (tdb->methods->tdb_write(tdb, off, rec, કદ(*rec)) == -1)
			પાછા -1;
	}

	જો (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		પાછા TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	જો (tdb->methods->tdb_oob(tdb, rec->next+કદ(*rec), 0) != 0)
		પાછા -1;
	પાછા 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
સ્થિર પૂર્ણાંક remove_from_freelist(સંરચના tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_ટોચ;
	જ્યારે (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		જો (i == off) {
			/* We've found it! */
			પાછા tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	પાછા TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
સ્થિર પૂર્ણાંક update_tailer(સંરચના tdb_context *tdb, tdb_off_t offset,
			 અચલ સંરચના list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = કદ(*rec) + rec->rec_len;
	પાછા tdb_ofs_write(tdb, offset + totalsize - કદ(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
પૂર્ણાંક tdb_free(સંરચના tdb_context *tdb, tdb_off_t offset, સંરચના list_struct *rec)
{
	tdb_off_t જમણું, ડાબું;

	/* Allocation and tailer lock */
	જો (tdb_lock(tdb, -1, F_WRLCK) != 0)
		પાછા -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	જો (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		જાઓ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	જમણું = offset + કદ(*rec) + rec->rec_len;
	જો (જમણું + કદ(*rec) <= tdb->map_size) {
		સંરચના list_struct r;

		જો (tdb->methods->tdb_read(tdb, જમણું, &r, કદ(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", જમણું));
			જાઓ ડાબું;
		}

		/* If it's free, expand to include it. */
		જો (r.magic == TDB_FREE_MAGIC) {
			જો (remove_from_freelist(tdb, જમણું, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", જમણું));
				જાઓ ડાબું;
			}
			rec->rec_len += કદ(r) + r.rec_len;
		}
	}

ડાબું:
	/* Look left */
	ડાબું = offset - કદ(tdb_off_t);
	જો (ડાબું > TDB_DATA_START(tdb->header.hash_size)) {
		સંરચના list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		જો (tdb_ofs_read(tdb, ડાબું, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ડાબું));
			જાઓ update;
		}

		/* it could be uninitialised data */
		જો (leftsize == 0 || leftsize == TDB_PAD_U32) {
			જાઓ update;
		}

		ડાબું = offset - leftsize;

		/* Now read in record */
		જો (tdb->methods->tdb_read(tdb, ડાબું, &l, કદ(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ડાબું, leftsize));
			જાઓ update;
		}

		/* If it's free, expand to include it. */
		જો (l.magic == TDB_FREE_MAGIC) {
			જો (remove_from_freelist(tdb, ડાબું, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ડાબું));
				જાઓ update;
			} નહીંતર {
				offset = ડાબું;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	જો (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		જાઓ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	જો (tdb_ofs_read(tdb, FREELIST_ટોચ, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_ટોચ, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		જાઓ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	પાછા 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	પાછા -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
સ્થિર tdb_off_t tdb_allocate_ofs(સંરચના tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				સંરચના list_struct *rec, tdb_off_t last_ptr)
{
	સંરચના list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', કદ(newrec));

	/* found it - now possibly split it up  */
	જો (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (કદ(*rec) + length);
		newrec_ptr = rec_ptr + કદ(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} નહીંતર {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	જો (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		પાછા 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	જો (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		પાછા 0;
	}

	/* Did we create new block? */
	જો (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		જો (update_tailer(tdb, rec_ptr, rec) == -1) {
			પાછા 0;
		}

		/* Free new record */
		જો (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			પાછા 0;
		}
	}

	/* all done - return the new record offset */
	પાછા rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(સંરચના tdb_context *tdb, tdb_len_t length, સંરચના list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	સંરચના {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	જો (tdb_lock(tdb, -1, F_WRLCK) == -1)
		પાછા 0;

	/* Extra bytes required for tailer */
	length += કદ(tdb_off_t);

 again:
	last_ptr = FREELIST_ટોચ;

	/* read in the freelist top */
	જો (tdb_ofs_read(tdb, FREELIST_ટોચ, &rec_ptr) == -1)
		જાઓ fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	જ્યારે (rec_ptr) {
		જો (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			જાઓ fail;
		}

		જો (rec->rec_len >= length) {
			જો (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				જો (bestfit.rec_len < 2*length) {
					તોડો;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	જો (bestfit.rec_ptr != 0) {
		જો (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			જાઓ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		પાછા newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	જો (tdb_expand(tdb, length + કદ(*rec)) == 0)
		જાઓ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	પાછા 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

સ્થિર પૂર્ણાંક seen_ઉમેરો(સંરચના tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA ચાવી, data;

	memset(&data, '\0', કદ(data));
	ચાવી.dptr = (unsigned અક્ષર *)&rec_ptr;
	ચાવી.dsize = કદ(rec_ptr);
	પાછા tdb_store(mem_tdb, ચાવી, data, TDB_ઉમેરો);
}

પૂર્ણાંક tdb_validate_freelist(સંરચના tdb_context *tdb, પૂર્ણાંક *pnum_entries)
{
	સંરચના tdb_context *mem_tdb = ખાલી;
	સંરચના list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	પૂર્ણાંક ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	જો (!mem_tdb) {
		પાછા -1;
	}

	જો (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		પાછા 0;
	}

	last_ptr = FREELIST_ટોચ;

	/* Store the FREELIST_TOP record. */
	જો (seen_ઉમેરો(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		જાઓ fail;
	}

	/* read in the freelist top */
	જો (tdb_ofs_read(tdb, FREELIST_ટોચ, &rec_ptr) == -1) {
		જાઓ fail;
	}

	જ્યારે (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		જો (seen_ઉમેરો(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			જાઓ fail;
		}

		જો (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			જાઓ fail;
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
	પાછા ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
સ્થિર પૂર્ણાંક tdb_next_lock(સંરચના tdb_context *tdb, સંરચના tdb_traverse_lock *tlock,
			 સંરચના list_struct *rec)
{
	પૂર્ણાંક want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	માટે (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		જો (!tlock->off && tlock->hash != 0) {
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
			જો (tlock->hash == tdb->header.hash_size) {
				ચાલુ;
			}
		}

		જો (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			પાછા -1;

		/* No previous record?  Start at top of chain. */
		જો (!tlock->off) {
			જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(tlock->hash),
				     &tlock->off) == -1)
				જાઓ fail;
		} નહીંતર {
			/* Otherwise unlock the previous record. */
			જો (tdb_unlock_record(tdb, tlock->off) != 0)
				જાઓ fail;
		}

		જો (want_next) {
			/* We have offset of old record: grab next */
			જો (tdb_rec_read(tdb, tlock->off, rec) == -1)
				જાઓ fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		જ્યારે( tlock->off) {
			tdb_off_t current;
			જો (tdb_rec_read(tdb, tlock->off, rec) == -1)
				જાઓ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			જો (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				જાઓ fail;
			}

			જો (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				જો (tdb_lock_record(tdb, tlock->off) != 0)
					જાઓ fail;
				પાછા tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			જો (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				જાઓ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	પાછા TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	જો (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	પાછા -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
સ્થિર પૂર્ણાંક tdb_traverse_internal(સંરચના tdb_context *tdb,
				 tdb_traverse_func fn, શૂન્ય *private_data,
				 સંરચના tdb_traverse_lock *tl)
{
	TDB_DATA ચાવી, dbuf;
	સંરચના list_struct rec;
	પૂર્ણાંક ret, ગણતરી = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	જ્યારે ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		ગણતરી++;
		/* now read the full record */
		ચાવી.dptr = tdb_alloc_read(tdb, tl->off + કદ(rec),
					  rec.ચાવી_len + rec.data_len);
		જો (!ચાવી.dptr) {
			ret = -1;
			જો (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				જાઓ out;
			જો (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			જાઓ out;
		}
		ચાવી.dsize = rec.ચાવી_len;
		dbuf.dptr = ચાવી.dptr + rec.ચાવી_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		જો (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(ચાવી.dptr);
			જાઓ out;
		}
		જો (fn && fn(tdb, ચાવી, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = ગણતરી;
			જો (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(ચાવી.dptr);
			જાઓ out;
		}
		SAFE_FREE(ચાવી.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	જો (ret < 0)
		પાછા -1;
	નહીંતર
		પાછા ગણતરી;
}


/*
  a write style traverse - temporarily marks the db read only
*/
પૂર્ણાંક tdb_traverse_read(સંરચના tdb_context *tdb,
		      tdb_traverse_func fn, શૂન્ય *private_data)
{
	સંરચના tdb_traverse_lock tl = { ખાલી, 0, 0, F_RDLCK };
	પૂર્ણાંક ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	જો (tdb_transaction_lock(tdb, F_RDLCK)) {
		પાછા -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	પાછા ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
પૂર્ણાંક tdb_traverse(સંરચના tdb_context *tdb,
		 tdb_traverse_func fn, શૂન્ય *private_data)
{
	સંરચના tdb_traverse_lock tl = { ખાલી, 0, 0, F_WRLCK };
	પૂર્ણાંક ret;

	જો (tdb->read_only || tdb->traverse_read) {
		પાછા tdb_traverse_read(tdb, fn, private_data);
	}

	જો (tdb_transaction_lock(tdb, F_WRLCK)) {
		પાછા -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	પાછા ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(સંરચના tdb_context *tdb)
{
	TDB_DATA ચાવી;
	સંરચના list_struct rec;

	/* release any old lock */
	જો (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		પાછા tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	જો (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		પાછા tdb_null;
	/* now read the key */
	ચાવી.dsize = rec.ચાવી_len;
	ચાવી.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+કદ(rec),ચાવી.dsize);

	/* Unlock the hash chain of the record we just read. */
	જો (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	પાછા ચાવી;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(સંરચના tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA ચાવી = tdb_null;
	સંરચના list_struct rec;
	unsigned અક્ષર *k = ખાલી;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	જો (tdb->travlocks.off) {
		જો (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			પાછા tdb_null;
		જો (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+કદ(rec),
					    rec.ચાવી_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			જો (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				પાછા tdb_null;
			}
			જો (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				પાછા tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	જો (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		જો (!tdb->travlocks.off)
			પાછા tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		જો (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			પાછા tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	જો (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		ચાવી.dsize = rec.ચાવી_len;
		ચાવી.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+કદ(rec),
					  ચાવી.dsize);
		/* Unlock the chain of this new record */
		જો (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	જો (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	પાછા ચાવી;
}

/* file: dump.c */

સ્થિર tdb_off_t tdb_dump_record(સંરચના tdb_context *tdb, પૂર્ણાંક hash,
				 tdb_off_t offset)
{
	સંરચના list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	જો (tdb->methods->tdb_read(tdb, offset, (અક્ષર *)&rec,
				   કદ(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		પાછા 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.ચાવી_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + કદ(rec) + rec.rec_len - કદ(tdb_off_t);

	જો (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		પાછા rec.next;
	}

	જો (tailer != rec.rec_len + કદ(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned પૂર્ણાંક)tailer, (unsigned પૂર્ણાંક)(rec.rec_len + કદ(rec)));
	}
	પાછા rec.next;
}

સ્થિર પૂર્ણાંક tdb_dump_chain(સંરચના tdb_context *tdb, પૂર્ણાંક i)
{
	tdb_off_t rec_ptr, ટોચ;

	ટોચ = TDB_HASH_ટોચ(i);

	જો (tdb_lock(tdb, i, F_WRLCK) != 0)
		પાછા -1;

	જો (tdb_ofs_read(tdb, ટોચ, &rec_ptr) == -1)
		પાછા tdb_unlock(tdb, i, F_WRLCK);

	જો (rec_ptr)
		printf("hash=%d\n", i);

	જ્યારે (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	પાછા tdb_unlock(tdb, i, F_WRLCK);
}

શૂન્ય tdb_dump_all(સંરચના tdb_context *tdb)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < (પૂર્ણાંક)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

પૂર્ણાંક tdb_printfreelist(સંરચના tdb_context *tdb)
{
	પૂર્ણાંક ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	સંરચના list_struct rec;

	જો ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		પાછા ret;

	offset = FREELIST_ટોચ;

	/* read in the freelist top */
	જો (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		પાછા 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	જ્યારે (rec_ptr) {
		જો (tdb->methods->tdb_read(tdb, rec_ptr, (અક્ષર *)&rec,
					   કદ(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			પાછા -1;
		}

		જો (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			પાછા -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (પૂર્ણાંક)total_free,
               (પૂર્ણાંક)total_free);

	પાછા tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
શૂન્ય tdb_increment_seqnum_nonblock(સંરચના tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	જો (!(tdb->flags & TDB_SEQNUM)) {
		પાછા;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	જો (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		પાછા;
	seqnum++;
	(શૂન્ય) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
સ્થિર શૂન્ય tdb_increment_seqnum(સંરચના tdb_context *tdb)
{
	જો (!(tdb->flags & TDB_SEQNUM)) {
		પાછા;
	}

	જો (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		પાછા;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

સ્થિર પૂર્ણાંક tdb_ચાવી_compare(TDB_DATA ચાવી, TDB_DATA data,
			   શૂન્ય *private_data EXT2FS_ATTR((unused)))
{
	પાછા memcmp(data.dptr, ચાવી.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
સ્થિર tdb_off_t tdb_find(સંરચના tdb_context *tdb, TDB_DATA ચાવી, u32 hash,
			સંરચના list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(hash), &rec_ptr) == -1)
		પાછા 0;

	/* keep looking until we find the right record */
	જ્યારે (rec_ptr) {
		જો (tdb_rec_read(tdb, rec_ptr, r) == -1)
			પાછા 0;

		જો (!TDB_DEAD(r) && hash==r->full_hash
		    && ચાવી.dsize==r->ચાવી_len
		    && tdb_parse_data(tdb, ચાવી, rec_ptr + કદ(*r),
				      r->ચાવી_len, tdb_ચાવી_compare,
				      ખાલી) == 0) {
			પાછા rec_ptr;
		}
		rec_ptr = r->next;
	}
	પાછા TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(સંરચના tdb_context *tdb, TDB_DATA ચાવી, u32 hash, પૂર્ણાંક locktype,
			   સંરચના list_struct *rec)
{
	u32 rec_ptr;

	જો (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		પાછા 0;
	જો (!(rec_ptr = tdb_find(tdb, ચાવી, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	પાછા rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
સ્થિર પૂર્ણાંક tdb_update_hash(સંરચના tdb_context *tdb, TDB_DATA ચાવી, u32 hash, TDB_DATA dbuf)
{
	સંરચના list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	જો (!(rec_ptr = tdb_find(tdb, ચાવી, hash, &rec)))
		પાછા -1;

	/* must be long enough key, data and tailer */
	જો (rec.rec_len < ચાવી.dsize + dbuf.dsize + કદ(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		પાછા -1;
	}

	જો (tdb->methods->tdb_write(tdb, rec_ptr + કદ(rec) + rec.ચાવી_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		પાછા -1;

	જો (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		પાછા tdb_rec_write(tdb, rec_ptr, &rec);
	}

	પાછા 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	tdb_off_t rec_ptr;
	સંરચના list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ચાવી);
	જો (!(rec_ptr = tdb_find_lock_hash(tdb,ચાવી,hash,F_RDLCK,&rec)))
		પાછા tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + કદ(rec) + rec.ચાવી_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	પાછા ret;
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

પૂર્ણાંક tdb_parse_record(સંરચના tdb_context *tdb, TDB_DATA ચાવી,
		     પૂર્ણાંક (*parser)(TDB_DATA ચાવી, TDB_DATA data,
				   શૂન્ય *private_data),
		     શૂન્ય *private_data)
{
	tdb_off_t rec_ptr;
	સંરચના list_struct rec;
	પૂર્ણાંક ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ચાવી);

	જો (!(rec_ptr = tdb_find_lock_hash(tdb,ચાવી,hash,F_RDLCK,&rec))) {
		પાછા TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, ચાવી, rec_ptr + કદ(rec) + rec.ચાવી_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	પાછા ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
સ્થિર પૂર્ણાંક tdb_exists_hash(સંરચના tdb_context *tdb, TDB_DATA ચાવી, u32 hash)
{
	સંરચના list_struct rec;

	જો (tdb_find_lock_hash(tdb, ચાવી, hash, F_RDLCK, &rec) == 0)
		પાછા 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	પાછા 1;
}

પૂર્ણાંક tdb_exists(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	u32 hash = tdb->hash_fn(&ચાવી);
	પાછા tdb_exists_hash(tdb, ચાવી, hash);
}

/* actually delete an entry in the database given the offset */
પૂર્ણાંક tdb_do_delete(સંરચના tdb_context *tdb, tdb_off_t rec_ptr, સંરચના list_struct*rec)
{
	tdb_off_t last_ptr, i;
	સંરચના list_struct lastrec;

	જો (tdb->read_only || tdb->traverse_read) પાછા -1;

	જો (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		પાછા tdb_rec_write(tdb, rec_ptr, rec);
	}
	જો (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		પાછા -1;

	/* find previous record in hash chain */
	જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(rec->full_hash), &i) == -1)
		પાછા -1;
	માટે (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		જો (tdb_rec_read(tdb, i, &lastrec) == -1)
			પાછા -1;

	/* unlink it: next ptr is at start of record. */
	જો (last_ptr == 0)
		last_ptr = TDB_HASH_ટોચ(rec->full_hash);
	જો (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		પાછા -1;

	/* recover the space */
	જો (tdb_free(tdb, rec_ptr, rec) == -1)
		પાછા -1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક tdb_ગણતરી_dead(સંરચના tdb_context *tdb, u32 hash)
{
	પૂર્ણાંક res = 0;
	tdb_off_t rec_ptr;
	સંરચના list_struct rec;

	/* read in the hash top */
	જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(hash), &rec_ptr) == -1)
		પાછા 0;

	જ્યારે (rec_ptr) {
		જો (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			પાછા 0;

		જો (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	પાછા res;
}

/*
 * Purge all DEAD records from a hash chain
 */
સ્થિર પૂર્ણાંક tdb_purge_dead(સંરચના tdb_context *tdb, u32 hash)
{
	પૂર્ણાંક res = -1;
	સંરચના list_struct rec;
	tdb_off_t rec_ptr;

	જો (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		પાછા -1;
	}

	/* read in the hash top */
	જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(hash), &rec_ptr) == -1)
		જાઓ fail;

	જ્યારે (rec_ptr) {
		tdb_off_t next;

		જો (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			જાઓ fail;
		}

		next = rec.next;

		જો (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			જાઓ fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	પાછા res;
}

/* delete an entry in the database given a key */
સ્થિર પૂર્ણાંક tdb_delete_hash(સંરચના tdb_context *tdb, TDB_DATA ચાવી, u32 hash)
{
	tdb_off_t rec_ptr;
	સંરચના list_struct rec;
	પૂર્ણાંક ret;

	જો (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		જો (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			પાછા -1;

		જો (tdb_ગણતરી_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		જો (!(rec_ptr = tdb_find(tdb, ચાવી, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			પાછા -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	નહીંતર {
		જો (!(rec_ptr = tdb_find_lock_hash(tdb, ચાવી, hash, F_WRLCK,
						   &rec)))
			પાછા -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	જો (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	જો (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	પાછા ret;
}

પૂર્ણાંક tdb_delete(સંરચના tdb_context *tdb, TDB_DATA ચાવી)
{
	u32 hash = tdb->hash_fn(&ચાવી);
	પાછા tdb_delete_hash(tdb, ચાવી, hash);
}

/*
 * See if we have a dead record around with enough space
 */
સ્થિર tdb_off_t tdb_find_dead(સંરચના tdb_context *tdb, u32 hash,
			       સંરચના list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(hash), &rec_ptr) == -1)
		પાછા 0;

	/* keep looking until we find the right record */
	જ્યારે (rec_ptr) {
		જો (tdb_rec_read(tdb, rec_ptr, r) == -1)
			પાછા 0;

		જો (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			પાછા rec_ptr;
		}
		rec_ptr = r->next;
	}
	પાછા 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
પૂર્ણાંક tdb_store(સંરચના tdb_context *tdb, TDB_DATA ચાવી, TDB_DATA dbuf, પૂર્ણાંક flag)
{
	સંરચના list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	અક્ષર *p = ખાલી;
	પૂર્ણાંક ret = -1;

	જો (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		પાછા -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ચાવી);
	જો (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		પાછા -1;

	/* check for it existing, on insert. */
	જો (flag == TDB_ઉમેરો) {
		જો (tdb_exists_hash(tdb, ચાવી, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			જાઓ fail;
		}
	} નહીંતર {
		/* first try in-place update, on modify or replace. */
		જો (tdb_update_hash(tdb, ચાવી, hash, dbuf) == 0) {
			જાઓ done;
		}
		જો (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			જાઓ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	જો (flag != TDB_ઉમેરો)
		tdb_delete_hash(tdb, ચાવી, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	જો (!(p = (અક્ષર *)malloc(ચાવી.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		જાઓ fail;
	}

	memcpy(p, ચાવી.dptr, ચાવી.dsize);
	જો (dbuf.dsize)
		memcpy(p+ચાવી.dsize, dbuf.dptr, dbuf.dsize);

	જો (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			ચાવી.dsize + dbuf.dsize + કદ(tdb_off_t));

		જો (rec_ptr != 0) {
			rec.ચાવી_len = ચાવી.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			જો (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + કદ(rec),
				    p, ચાવી.dsize + dbuf.dsize) == -1) {
				જાઓ fail;
			}
			જાઓ done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	જો (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		જાઓ fail;
	}

	જો ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		જાઓ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, ચાવી.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	જો (rec_ptr == 0) {
		જાઓ fail;
	}

	/* Read hash top into next ptr */
	જો (tdb_ofs_read(tdb, TDB_HASH_ટોચ(hash), &rec.next) == -1)
		જાઓ fail;

	rec.ચાવી_len = ચાવી.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	જો (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+કદ(rec), p, ચાવી.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_ટોચ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		જાઓ fail;
	}

 done:
	ret = 0;
 fail:
	જો (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	પાછા ret;
}


/* Append to an entry. Create if not exist. */
પૂર્ણાંક tdb_append(સંરચના tdb_context *tdb, TDB_DATA ચાવી, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	પૂર્ણાંક ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ચાવી);
	જો (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		પાછા -1;

	dbuf = tdb_fetch(tdb, ચાવી);

	જો (dbuf.dptr == ખાલી) {
		dbuf.dptr = (unsigned અક્ષર *)malloc(new_dbuf.dsize);
	} નહીંતર {
		unsigned અક્ષર *new_dptr = (unsigned અક્ષર *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		જો (new_dptr == ખાલી) {
			મુક્ત(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	જો (dbuf.dptr == ખાલી) {
		tdb->ecode = TDB_ERR_OOM;
		જાઓ failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, ચાવી, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	પાછા ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
અચલ અક્ષર *tdb_name(સંરચના tdb_context *tdb)
{
	પાછા tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
પૂર્ણાંક tdb_fd(સંરચના tdb_context *tdb)
{
	પાછા tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(સંરચના tdb_context *tdb)
{
	પાછા tdb->log.log_fn;
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
પૂર્ણાંક tdb_get_seqnum(સંરચના tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	જો (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		પાછા 0;
	પાછા seqnum;
}

પૂર્ણાંક tdb_hash_size(સંરચના tdb_context *tdb)
{
	પાછા tdb->header.hash_size;
}

size_t tdb_map_size(સંરચના tdb_context *tdb)
{
	પાછા tdb->map_size;
}

પૂર્ણાંક tdb_get_flags(સંરચના tdb_context *tdb)
{
	પાછા tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
શૂન્ય tdb_enable_seqnum(સંરચના tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
સ્થિર સંરચના tdb_context *tdbs = ખાલી;


/* This is from a hash algorithm suggested by Rogier Wolff */
સ્થિર unsigned પૂર્ણાંક default_tdb_hash(TDB_DATA *ચાવી)
{
	u32 કિંમત;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	માટે (કિંમત = 0, i=0; i < ચાવી->dsize; i++)
		કિંમત = કિંમત * 256 + ચાવી->dptr[i] + (કિંમત >> 24) * 241;

	પાછા કિંમત;
}


/* initialise a new database with a specified hash size */
સ્થિર પૂર્ણાંક tdb_new_database(સંરચના tdb_context *tdb, પૂર્ણાંક hash_size)
{
	સંરચના tdb_header *newdb;
	પૂર્ણાંક size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = કદ(સંરચના tdb_header) + (hash_size+1)*કદ(tdb_off_t);
	જો (!(newdb = (સંરચના tdb_header *)calloc(size, 1)))
		પાછા TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	જો (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (અક્ષર *)newdb;
		memcpy(&tdb->header, newdb, કદ(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		પાછા 0;
	}
	જો (lseek(tdb->fd, 0, SEEK_SET) == -1)
		જાઓ fail;

	જો (ftruncate(tdb->fd, 0) == -1)
		જાઓ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, કદ(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	જો (લિખ(tdb->fd, newdb, size) != size) {
		ret = -1;
	} નહીંતર {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	પાછા ret;
}



સ્થિર પૂર્ણાંક tdb_already_open(dev_t device,
			    ino_t ino)
{
	સંરચના tdb_context *i;

	માટે (i = tdbs; i; i = i->next) {
		જો (i->device == device && i->inode == ino) {
			પાછા 1;
		}
	}

	પાછા 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
સંરચના tdb_context *tdb_open(અચલ અક્ષર *name, પૂર્ણાંક hash_size, પૂર્ણાંક tdb_flags,
		      પૂર્ણાંક open_flags, mode_t mode)
{
	પાછા tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, ખાલી, ખાલી);
}

/* a default logging function */
સ્થિર શૂન્ય null_log_fn(સંરચના tdb_context *tdb, ગણના tdb_debug_level level, અચલ અક્ષર *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
સ્થિર શૂન્ય null_log_fn(સંરચના tdb_context *tdb EXT2FS_ATTR((unused)),
			ગણના tdb_debug_level level EXT2FS_ATTR((unused)),
			અચલ અક્ષર *fmt EXT2FS_ATTR((unused)), ...)
{
}


સંરચના tdb_context *tdb_open_ex(અચલ અક્ષર *name, પૂર્ણાંક hash_size, પૂર્ણાંક tdb_flags,
				પૂર્ણાંક open_flags, mode_t mode,
				અચલ સંરચના tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	સંરચના tdb_context *tdb;
	સંરચના stat st;
	પૂર્ણાંક rev = 0, locked = 0;
	unsigned અક્ષર *vp;
	u32 vertest;

	જો (!(tdb = (સંરચના tdb_context *)calloc(1, કદ *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		જાઓ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ખાલી;
	tdb->map_ptr = ખાલી;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	જો (log_ctx) {
		tdb->log = *log_ctx;
	} નહીંતર {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = ખાલી;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	જો (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	જો ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		જાઓ fail;
	}

	જો (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	જો ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	જો (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		જો (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			જાઓ fail;
		}
		જાઓ internal;
	}

	જો ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		જાઓ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	જો (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		જાઓ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	જો ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		જો (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			જાઓ fail; /* errno set by ftruncate */
		}
	}

	જો (પઠ(tdb->fd, &tdb->header, કદ(tdb->header)) != કદ(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      કદ(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		જો (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			જાઓ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned અક્ષર *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	જો (!rev)
		tdb->flags &= ~TDB_CONVERT;
	નહીંતર {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, કદ(tdb->header));
	}
	જો (fstat(tdb->fd, &st) == -1)
		જાઓ fail;

	જો (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		જાઓ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	જો (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (પૂર્ણાંક)st.st_dev, (પૂર્ણાંક)st.st_ino));
		errno = EBUSY;
		જાઓ fail;
	}

	જો (!(tdb->name = (અક્ષર *)strdup(name))) {
		errno = ENOMEM;
		જાઓ fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	જો (locked) {
		જો (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			જાઓ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	જો (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		જો (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			જાઓ fail;
	}

	/* if needed, run recovery */
	જો (tdb_transaction_recover(tdb) == -1) {
		જાઓ fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	જો (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		જાઓ fail;
	tdb->next = tdbs;
	tdbs = tdb;
	પાછા tdb;

 fail:
	{ પૂર્ણાંક save_errno = errno;

	જો (!tdb)
		પાછા ખાલી;

	જો (tdb->map_ptr) {
		જો (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		નહીંતર
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	જો (tdb->fd != -1)
		જો (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	પાછા ખાલી;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

શૂન્ય tdb_set_max_dead(સંરચના tdb_context *tdb, પૂર્ણાંક max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
પૂર્ણાંક tdb_close(સંરચના tdb_context *tdb)
{
	સંરચના tdb_context **i;
	પૂર્ણાંક ret = 0;

	જો (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	જો (tdb->map_ptr) {
		જો (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		નહીંતર
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	જો (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	માટે (i = &tdbs; *i; i = &(*i)->next) {
		જો (*i == tdb) {
			*i = tdb->next;
			તોડો;
		}
	}

	memset(tdb, 0, કદ(*tdb));
	SAFE_FREE(tdb);

	પાછા ret;
}

/* register a logging function */
શૂન્ય tdb_set_logging_function(સંરચના tdb_context *tdb,
                              અચલ સંરચના tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

શૂન્ય *tdb_get_logging_private(સંરચના tdb_context *tdb)
{
	પાછા tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
પૂર્ણાંક tdb_reopen(સંરચના tdb_context *tdb)
{
	સંરચના stat st;

	જો (tdb->flags & TDB_INTERNAL) {
		પાછા 0; /* Nothing to do. */
	}

	જો (tdb->અંક_locks != 0 || tdb->global_lock.ગણતરી) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		જાઓ fail;
	}

	જો (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		જાઓ fail;
	}

	જો (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		જાઓ fail;
	}
	જો (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	જો (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		જાઓ fail;
	}
	જો ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		જાઓ fail;
	}
	જો (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		જાઓ fail;
	}
	જો (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		જાઓ fail;
	}
	tdb_mmap(tdb);

	પાછા 0;

fail:
	tdb_close(tdb);
	પાછા -1;
}

/* reopen all tdb's */
પૂર્ણાંક tdb_reopen_all(પૂર્ણાંક parent_longlived)
{
	સંરચના tdb_context *tdb;

	માટે (tdb=tdbs; tdb; tdb = tdb->next) {
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
		જો (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		જો (tdb_reopen(tdb) != 0)
			પાછા -1;
	}

	પાછા 0;
}

/**
 * Flush a database file from the page cache.
 **/
પૂર્ણાંક tdb_flush(સંરચના tdb_context *tdb)
{
	જો (tdb->fd != -1)
		પાછા fsync(tdb->fd);
	પાછા 0;
}
