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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ifdef HAVE_SYS_SELECT_H
#ಅನ್ತರ್ಭಾವ <sys/select.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ifdef HAVE_UTIME_H
#ಅನ್ತರ್ಭಾವ <utime.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/file.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#ಅನ್ಯಥಾ
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((ಶೂನ್ಯ *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
ಸ್ಥಿರ ವರ್ಣ *rep_strdup(ಧ್ರುವ ವರ್ಣ *s)
{
	ವರ್ಣ *ret;
	ಪೂರ್ಣಾಙ್ಕ length;

	ಯದಿ (!s)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	length = strlen(s);
	ret = malloc(length + 1);
	ಯದಿ (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#ಯದಿ (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ಅನ್ಯಥಾ
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#ಯದಿ defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ bool;
#endif

#ಅನ್ತರ್ಭಾವ "tdb.h"

ಸ್ಥಿರ TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

ಪ್ರಕಾರನಿರ್ದೇಶ u32 tdb_len_t;
ಪ್ರಕಾರನಿರ್ದೇಶ u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned ಪೂರ್ಣಾಙ್ಕ)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*ಪರಿಮಾಣ(ಸಂರಚನಾ list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_ತುದಿ (ಪರಿಮಾಣ(ಸಂರಚನಾ tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_ತುದಿ(hash) (FREELIST_ತುದಿ + (BUCKET(hash)+1)*ಪರಿಮಾಣ(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*ಪರಿಮಾಣ(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_ತುದಿ(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(ಸಂರಚನಾ tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(ಸಂರಚನಾ tdb_header, sequence_ಸಂಖ್ಯೆ)
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
#define SAFE_FREE(x) ಕುರು { ಯದಿ ((x) != ರಿಕ್ತ) {ಮುಕ್ತ(x); (x)=ರಿಕ್ತ;} } ಯಾವತ್(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, ಪರಿಮಾಣ(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ಸಂರಚನಾ list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t ಕೀಲಿ_len; /* byte length of key */
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
ಸಂರಚನಾ tdb_header {
	ವರ್ಣ magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_ಸಂಖ್ಯೆ; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ಸಂರಚನಾ tdb_lock_type {
	ಪೂರ್ಣಾಙ್ಕ list;
	u32 ಎಣಿಕೆ;
	u32 ltype;
};

ಸಂರಚನಾ tdb_traverse_lock {
	ಸಂರಚನಾ tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	ಪೂರ್ಣಾಙ್ಕ lock_rw;
};


ಸಂರಚನಾ tdb_methods {
	ಪೂರ್ಣಾಙ್ಕ (*tdb_read)(ಸಂರಚನಾ tdb_context *, tdb_off_t , ಶೂನ್ಯ *, tdb_len_t , ಪೂರ್ಣಾಙ್ಕ );
	ಪೂರ್ಣಾಙ್ಕ (*tdb_write)(ಸಂರಚನಾ tdb_context *, tdb_off_t, ಧ್ರುವ ಶೂನ್ಯ *, tdb_len_t);
	ಶೂನ್ಯ (*next_hash_chain)(ಸಂರಚನಾ tdb_context *, u32 *);
	ಪೂರ್ಣಾಙ್ಕ (*tdb_oob)(ಸಂರಚನಾ tdb_context *, tdb_off_t , ಪೂರ್ಣಾಙ್ಕ );
	ಪೂರ್ಣಾಙ್ಕ (*tdb_expand_file)(ಸಂರಚನಾ tdb_context *, tdb_off_t , tdb_off_t );
	ಪೂರ್ಣಾಙ್ಕ (*tdb_brlock)(ಸಂರಚನಾ tdb_context *, tdb_off_t , ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, size_t);
};

ಸಂರಚನಾ tdb_context {
	ವರ್ಣ *name; /* the name of the database */
	ಶೂನ್ಯ *map_ptr; /* where it is currently mapped */
	ಪೂರ್ಣಾಙ್ಕ fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	ಪೂರ್ಣಾಙ್ಕ read_only; /* opened read-only */
	ಪೂರ್ಣಾಙ್ಕ traverse_read; /* read-only traversal */
	ಸಂರಚನಾ tdb_lock_type global_lock;
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_lockrecs;
	ಸಂರಚನಾ tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	ಗಣನಾ TDB_ERROR ecode; /* error code for last tdb error */
	ಸಂರಚನಾ tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ಸಂರಚನಾ tdb_traverse_lock travlocks; /* current traversal locks */
	ಸಂರಚನಾ tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ಸಂರಚನಾ tdb_logging_context log;
	unsigned ಪೂರ್ಣಾಙ್ಕ (*hash_fn)(TDB_DATA *ಕೀಲಿ);
	ಪೂರ್ಣಾಙ್ಕ open_flags; /* flags used in the open - needed by reopen */
	unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_locks; /* number of chain locks held */
	ಧ್ರುವ ಸಂರಚನಾ tdb_methods *methods;
	ಸಂರಚನಾ tdb_transaction *transaction;
	ಪೂರ್ಣಾಙ್ಕ page_size;
	ಪೂರ್ಣಾಙ್ಕ max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_munmap(ಸಂರಚನಾ tdb_context *tdb);
ಸ್ಥಿರ ಶೂನ್ಯ tdb_mmap(ಸಂರಚನಾ tdb_context *tdb);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_lock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype);
ಪೂರ್ಣಾಙ್ಕ tdb_lock_nonblock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_unlock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_brlock(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಪೂರ್ಣಾಙ್ಕ rw_type, ಪೂರ್ಣಾಙ್ಕ lck_type, ಪೂರ್ಣಾಙ್ಕ probe, size_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_transaction_lock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ ltype);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_transaction_unlock(ಸಂರಚನಾ tdb_context *tdb);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_brlock_upgrade(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, size_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_write_lock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_write_unlock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_ofs_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_ofs_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ಸ್ಥಿರ ಶೂನ್ಯ *tdb_convert(ಶೂನ್ಯ *buf, u32 size);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_free(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಸಂರಚನಾ list_struct *rec);
ಸ್ಥಿರ tdb_off_t tdb_allocate(ಸಂರಚನಾ tdb_context *tdb, tdb_len_t length, ಸಂರಚನಾ list_struct *rec);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_ofs_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_ofs_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_lock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_unlock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_rec_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಸಂರಚನಾ list_struct *rec);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_rec_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಸಂರಚನಾ list_struct *rec);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_do_delete(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t rec_ptr, ಸಂರಚನಾ list_struct *rec);
ಸ್ಥಿರ unsigned ವರ್ಣ *tdb_alloc_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_parse_data(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ,
		   tdb_off_t offset, tdb_len_t len,
		   ಪೂರ್ಣಾಙ್ಕ (*parser)(TDB_DATA ಕೀಲಿ, TDB_DATA data,
				 ಶೂನ್ಯ *private_data),
		   ಶೂನ್ಯ *private_data);
ಸ್ಥಿರ tdb_off_t tdb_find_lock_hash(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, u32 hash, ಪೂರ್ಣಾಙ್ಕ locktype,
			   ಸಂರಚನಾ list_struct *rec);
ಸ್ಥಿರ ಶೂನ್ಯ tdb_io_init(ಸಂರಚನಾ tdb_context *tdb);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_expand(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t size);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_rec_free_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off,
		      ಸಂರಚನಾ list_struct *rec);


/* file: error.c */

ಗಣನಾ TDB_ERROR tdb_error(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->ecode;
}

ಸ್ಥಿರ ಸಂರಚನಾ tdb_errname {
	ಗಣನಾ TDB_ERROR ecode; ಧ್ರುವ ವರ್ಣ *estring;
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
ಧ್ರುವ ವರ್ಣ *tdb_errorstr(ಸಂರಚನಾ tdb_context *tdb)
{
	u32 i;
	ಪ್ರತಿ (i = 0; i < ಪರಿಮಾಣ(emap) / ಪರಿಮಾಣ(ಸಂರಚನಾ tdb_errname); i++)
		ಯದಿ (tdb->ecode == emap[i].ecode)
			ಪ್ರತ್ಯಾಗಮ emap[i].estring;
	ಪ್ರತ್ಯಾಗಮ "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
ಪೂರ್ಣಾಙ್ಕ tdb_brlock(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset,
	       ಪೂರ್ಣಾಙ್ಕ rw_type, ಪೂರ್ಣಾಙ್ಕ lck_type, ಪೂರ್ಣಾಙ್ಕ probe, size_t len)
{
	ಸಂರಚನಾ flock fl;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (tdb->flags & TDB_NOLOCK) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	ಕುರು {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} ಯಾವತ್ (ret == -1 && errno == EINTR);

	ಯದಿ (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		ಯದಿ (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (ಪೂರ್ಣಾಙ್ಕ)len));
		}
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
ಪೂರ್ಣಾಙ್ಕ tdb_brlock_upgrade(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, size_t len)
{
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 1000;
	ಯಾವತ್ (ಎಣಿಕೆ--) {
		ಸಂರಚನಾ timeval tv;
		ಯದಿ (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		ಯದಿ (errno != EDEADLK) {
			ಭಙ್ಗ;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	ಪ್ರತ್ಯಾಗಮ -1;
}


/* lock a list in the database. list -1 is the alloc list */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _tdb_lock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype, ಪೂರ್ಣಾಙ್ಕ op)
{
	ಸಂರಚನಾ tdb_lock_type *new_lck;
	ಪೂರ್ಣಾಙ್ಕ i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ) {
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ಯದಿ (list < -1 || list >= (ಪೂರ್ಣಾಙ್ಕ)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಯದಿ (tdb->flags & TDB_NOLOCK)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತಿ (i=0; i<tdb->ಅಂಕೆ_lockrecs; i++) {
		ಯದಿ (tdb->lockrecs[i].list == list) {
			ಯದಿ (tdb->lockrecs[i].ಎಣಿಕೆ == 0) {
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
			tdb->lockrecs[i].ಎಣಿಕೆ++;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	new_lck = (ಸಂರಚನಾ tdb_lock_type *)realloc(
		tdb->lockrecs,
		ಪರಿಮಾಣ(*tdb->lockrecs) * (tdb->ಅಂಕೆ_lockrecs+1));
	ಯದಿ (new_lck == ರಿಕ್ತ) {
		errno = ENOMEM;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	ಯದಿ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_ತುದಿ+4*list, ltype, op,
				     0, 1)) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	tdb->ಅಂಕೆ_locks++;

	tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs].list = list;
	tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs].ಎಣಿಕೆ = 1;
	tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs].ltype = ltype;
	tdb->ಅಂಕೆ_lockrecs += 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* lock a list in the database. list -1 is the alloc list */
ಪೂರ್ಣಾಙ್ಕ tdb_lock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	ಯದಿ (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
ಪೂರ್ಣಾಙ್ಕ tdb_lock_nonblock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
ಪೂರ್ಣಾಙ್ಕ tdb_unlock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ list, ಪೂರ್ಣಾಙ್ಕ ltype)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ tdb_lock_type *lck = ರಿಕ್ತ;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ) {
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ಯದಿ (tdb->flags & TDB_NOLOCK)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Sanity checks */
	ಯದಿ (list < -1 || list >= (ಪೂರ್ಣಾಙ್ಕ)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಪ್ರತಿ (i=0; i<tdb->ಅಂಕೆ_lockrecs; i++) {
		ಯದಿ (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			ಭಙ್ಗ;
		}
	}

	ಯದಿ ((lck == ರಿಕ್ತ) || (lck->ಎಣಿಕೆ == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (lck->ಎಣಿಕೆ > 1) {
		lck->ಎಣಿಕೆ--;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	ಯದಿ (mark_lock) {
		ret = 0;
	} ಅನ್ಯಥಾ {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_ತುದಿ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->ಅಂಕೆ_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	ಯದಿ (tdb->ಅಂಕೆ_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs-1];
	}
	tdb->ಅಂಕೆ_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	ಯದಿ (tdb->ಅಂಕೆ_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	ಯದಿ (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
  get the transaction lock
 */
ಪೂರ್ಣಾಙ್ಕ tdb_transaction_lock(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ ltype)
{
	ಯದಿ (tdb->have_transaction_lock || tdb->global_lock.ಎಣಿಕೆ) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	tdb->have_transaction_lock = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
  release the transaction lock
 */
ಪೂರ್ಣಾಙ್ಕ tdb_transaction_unlock(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಯದಿ (!tdb->have_transaction_lock) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	ಯದಿ (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}




/* lock/unlock entire database */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _tdb_lockall(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ ltype, ಪೂರ್ಣಾಙ್ಕ op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	ಯದಿ (tdb->read_only || tdb->traverse_read)
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);

	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.ಎಣಿಕೆ++;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ) {
		/* a global lock of a different type exists */
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ಯದಿ (tdb->ಅಂಕೆ_locks != 0) {
		/* can't combine global and chain locks */
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ಯದಿ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ತುದಿ, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		ಯದಿ (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	tdb->global_lock.ಎಣಿಕೆ = 1;
	tdb->global_lock.ltype = ltype;

	ಪ್ರತ್ಯಾಗಮ 0;
}



/* unlock entire db */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _tdb_unlockall(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	ಯದಿ (tdb->read_only || tdb->traverse_read) {
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ಯದಿ (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.ಎಣಿಕೆ == 0) {
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ > 1) {
		tdb->global_lock.ಎಣಿಕೆ--;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	tdb->global_lock.ಎಣಿಕೆ = 0;
	tdb->global_lock.ltype = 0;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* lock entire database with write lock */
ಪೂರ್ಣಾಙ್ಕ tdb_lockall(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
ಪೂರ್ಣಾಙ್ಕ tdb_lockall_mark(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
ಪೂರ್ಣಾಙ್ಕ tdb_lockall_unmark(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
ಪೂರ್ಣಾಙ್ಕ tdb_lockall_nonblock(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
ಪೂರ್ಣಾಙ್ಕ tdb_unlockall(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
ಪೂರ್ಣಾಙ್ಕ tdb_lockall_read(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
ಪೂರ್ಣಾಙ್ಕ tdb_lockall_read_nonblock(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
ಪೂರ್ಣಾಙ್ಕ tdb_unlockall_read(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
ಪೂರ್ಣಾಙ್ಕ tdb_chainlock(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_lock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
ಪೂರ್ಣಾಙ್ಕ tdb_chainlock_nonblock(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
ಪೂರ್ಣಾಙ್ಕ tdb_chainlock_mark(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_lock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
ಪೂರ್ಣಾಙ್ಕ tdb_chainlock_unmark(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK | TDB_MARK_LOCK);
}

ಪೂರ್ಣಾಙ್ಕ tdb_chainunlock(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK);
}

ಪೂರ್ಣಾಙ್ಕ tdb_chainlock_read(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_lock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_RDLCK);
}

ಪೂರ್ಣಾಙ್ಕ tdb_chainunlock_read(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ಕೀಲಿ)), F_RDLCK);
}



/* record lock stops delete underneath */
ಪೂರ್ಣಾಙ್ಕ tdb_lock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off)
{
	ಪ್ರತ್ಯಾಗಮ off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ಪೂರ್ಣಾಙ್ಕ tdb_write_lock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off)
{
	ಸಂರಚನಾ tdb_traverse_lock *i;
	ಪ್ರತಿ (i = &tdb->travlocks; i; i = i->next)
		ಯದಿ (i->off == off)
			ಪ್ರತ್ಯಾಗಮ -1;
	ಪ್ರತ್ಯಾಗಮ tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ಪೂರ್ಣಾಙ್ಕ tdb_write_unlock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off)
{
	ಪ್ರತ್ಯಾಗಮ tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
ಪೂರ್ಣಾಙ್ಕ tdb_unlock_record(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off)
{
	ಸಂರಚನಾ tdb_traverse_lock *i;
	u32 ಎಣಿಕೆ = 0;

	ಯದಿ (off == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತಿ (i = &tdb->travlocks; i; i = i->next)
		ಯದಿ (i->off == off)
			ಎಣಿಕೆ++;
	ಪ್ರತ್ಯಾಗಮ (ಎಣಿಕೆ == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_oob(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t len, ಪೂರ್ಣಾಙ್ಕ probe)
{
	ಸಂರಚನಾ stat st;
	ಯದಿ (len <= tdb->map_size)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (tdb->flags & TDB_INTERNAL) {
		ಯದಿ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (ಪೂರ್ಣಾಙ್ಕ)len, (ಪೂರ್ಣಾಙ್ಕ)tdb->map_size));
		}
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ಯದಿ (fstat(tdb->fd, &st) == -1) {
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ಯದಿ (st.st_size < (off_t)len) {
		ಯದಿ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (ಪೂರ್ಣಾಙ್ಕ)len, (ಪೂರ್ಣಾಙ್ಕ)st.st_size));
		}
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	ಯದಿ (tdb_munmap(tdb) == -1)
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* write a lump of data at a specified offset */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off,
		     ಧ್ರುವ ಶೂನ್ಯ *buf, tdb_len_t len)
{
	ಯದಿ (len == 0) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (tdb->map_ptr) {
		memcpy(off + (ವರ್ಣ *)tdb->map_ptr, buf, len);
	} ಅನ್ಯಥಾ ಯದಿ (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
ಶೂನ್ಯ *tdb_convert(ಶೂನ್ಯ *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	ಪ್ರತಿ (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	ಪ್ರತ್ಯಾಗಮ buf;
}


/* read a lump of data at a specified offset, maybe convert */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off, ಶೂನ್ಯ *buf,
		    tdb_len_t len, ಪೂರ್ಣಾಙ್ಕ cv)
{
	ಯದಿ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->map_ptr) {
		memcpy(buf, off + (ವರ್ಣ *)tdb->map_ptr, len);
	} ಅನ್ಯಥಾ {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		ಯದಿ (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (ಪೂರ್ಣಾಙ್ಕ)off, (ಪೂರ್ಣಾಙ್ಕ)len, (ಪೂರ್ಣಾಙ್ಕ)ret, strerror(errno),
				 (ಪೂರ್ಣಾಙ್ಕ)tdb->map_size));
			ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	ಯದಿ (cv) {
		tdb_convert(buf, len);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ಸ್ಥಿರ ಶೂನ್ಯ tdb_next_hash_chain(ಸಂರಚನಾ tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ಯದಿ (tdb->map_ptr) {
		ಪ್ರತಿ (;h < tdb->header.hash_size;h++) {
			ಯದಿ (0 != *(u32 *)(TDB_HASH_ತುದಿ(h) + (unsigned ವರ್ಣ *)tdb->map_ptr)) {
				ಭಙ್ಗ;
			}
		}
	} ಅನ್ಯಥಾ {
		u32 off=0;
		ಪ್ರತಿ (;h < tdb->header.hash_size;h++) {
			ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(h), &off) != 0 || off != 0) {
				ಭಙ್ಗ;
			}
		}
	}
	(*chain) = h;
}


ಪೂರ್ಣಾಙ್ಕ tdb_munmap(ಸಂರಚನಾ tdb_context *tdb)
{
	ಯದಿ (tdb->flags & TDB_INTERNAL)
		ಪ್ರತ್ಯಾಗಮ 0;

#ifdef HAVE_MMAP
	ಯದಿ (tdb->map_ptr) {
		ಪೂರ್ಣಾಙ್ಕ ret = munmap(tdb->map_ptr, tdb->real_map_size);
		ಯದಿ (ret != 0)
			ಪ್ರತ್ಯಾಗಮ ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ tdb_mmap(ಸಂರಚನಾ tdb_context *tdb)
{
	ಯದಿ (tdb->flags & TDB_INTERNAL)
		ಪ್ರತ್ಯಾಗಮ;

#ifdef HAVE_MMAP
	ಯದಿ (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ರಿಕ್ತ, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		ಯದಿ (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = ರಿಕ್ತ;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} ಅನ್ಯಥಾ {
		tdb->map_ptr = ರಿಕ್ತ;
	}
#ಅನ್ಯಥಾ
	tdb->map_ptr = ರಿಕ್ತ;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_expand_file(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	ವರ್ಣ buf[1024];

	ಯದಿ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (ftruncate(tdb->fd, size+addition) == -1) {
		ವರ್ಣ b = 0;
		ಯದಿ (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, ಪರಿಮಾಣ(buf));
	ಯಾವತ್ (addition) {
		ಪೂರ್ಣಾಙ್ಕ n = addition>ಪರಿಮಾಣ(buf)?ಪರಿಮಾಣ(buf):addition;
		ಪೂರ್ಣಾಙ್ಕ ret = pwrite(tdb->fd, buf, n, size);
		ಯದಿ (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		addition -= n;
		size += n;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
ಪೂರ್ಣಾಙ್ಕ tdb_expand(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t size)
{
	ಸಂರಚನಾ list_struct rec;
	tdb_off_t offset;

	ಯದಿ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	ಯದಿ (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	ಯದಿ (!(tdb->flags & TDB_INTERNAL)) {
		ಯದಿ (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			ಗಚ್ಛ fail;
	}

	tdb->map_size += size;

	ಯದಿ (tdb->flags & TDB_INTERNAL) {
		ವರ್ಣ *new_map_ptr = (ವರ್ಣ *)realloc(tdb->map_ptr,
						    tdb->map_size);
		ಯದಿ (!new_map_ptr) {
			tdb->map_size -= size;
			ಗಚ್ಛ fail;
		}
		tdb->map_ptr = new_map_ptr;
	} ಅನ್ಯಥಾ {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',ಪರಿಮಾಣ(rec));
	rec.rec_len = size - ಪರಿಮಾಣ(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	ಯದಿ (tdb_free(tdb, offset, &rec) == -1)
		ಗಚ್ಛ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ -1;
}

/* read/write a tdb_off_t */
ಪೂರ್ಣಾಙ್ಕ tdb_ofs_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	ಪ್ರತ್ಯಾಗಮ tdb->methods->tdb_read(tdb, offset, (ವರ್ಣ*)d, ಪರಿಮಾಣ(*d), DOCONV());
}

ಪೂರ್ಣಾಙ್ಕ tdb_ofs_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	ಪ್ರತ್ಯಾಗಮ tdb->methods->tdb_write(tdb, offset, CONVERT(off), ಪರಿಮಾಣ(*d));
}


/* read a lump of data, allocating the space for it */
unsigned ವರ್ಣ *tdb_alloc_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned ವರ್ಣ *buf;

	/* some systems don't like zero length malloc */
	ಯದಿ (len == 0) {
		len = 1;
	}

	ಯದಿ (!(buf = (unsigned ವರ್ಣ *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	ಯದಿ (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
	ಪ್ರತ್ಯಾಗಮ buf;
}

/* Give a piece of tdb data to a parser */

ಪೂರ್ಣಾಙ್ಕ tdb_parse_data(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ,
		   tdb_off_t offset, tdb_len_t len,
		   ಪೂರ್ಣಾಙ್ಕ (*parser)(TDB_DATA ಕೀಲಿ, TDB_DATA data,
				 ಶೂನ್ಯ *private_data),
		   ಶೂನ್ಯ *private_data)
{
	TDB_DATA data;
	ಪೂರ್ಣಾಙ್ಕ result;

	data.dsize = len;

	ಯದಿ ((tdb->transaction == ರಿಕ್ತ) && (tdb->map_ptr != ರಿಕ್ತ)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		ಯದಿ (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		data.dptr = offset + (unsigned ವರ್ಣ *)tdb->map_ptr;
		ಪ್ರತ್ಯಾಗಮ parser(ಕೀಲಿ, data, private_data);
	}

	ಯದಿ (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	result = parser(ಕೀಲಿ, data, private_data);
	ಮುಕ್ತ(data.dptr);
	ಪ್ರತ್ಯಾಗಮ result;
}

/* read/write a record */
ಪೂರ್ಣಾಙ್ಕ tdb_rec_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಸಂರಚನಾ list_struct *rec)
{
	ಯದಿ (tdb->methods->tdb_read(tdb, offset, rec, ಪರಿಮಾಣ(*rec),DOCONV()) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ಪ್ರತ್ಯಾಗಮ tdb->methods->tdb_oob(tdb, rec->next+ಪರಿಮಾಣ(*rec), 0);
}

ಪೂರ್ಣಾಙ್ಕ tdb_rec_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಸಂರಚನಾ list_struct *rec)
{
	ಸಂರಚನಾ list_struct r = *rec;
	ಪ್ರತ್ಯಾಗಮ tdb->methods->tdb_write(tdb, offset, CONVERT(r), ಪರಿಮಾಣ(r));
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ tdb_methods io_methods = {
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
ಶೂನ್ಯ tdb_io_init(ಸಂರಚನಾ tdb_context *tdb)
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

ಸಂರಚನಾ tdb_transaction_el {
	ಸಂರಚನಾ tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned ವರ್ಣ *data;
};

/*
  hold the context of any current transaction
*/
ಸಂರಚನಾ tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ಧ್ರುವ ಸಂರಚನಾ tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ಸಂರಚನಾ tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	ಪೂರ್ಣಾಙ್ಕ transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	ಪೂರ್ಣಾಙ್ಕ nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off, ಶೂನ್ಯ *buf,
			    tdb_len_t len, ಪೂರ್ಣಾಙ್ಕ cv)
{
	ಸಂರಚನಾ tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	ಪ್ರತಿ (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		ಯದಿ (off+len <= el->offset) {
			ಅನುವರ್ತ;
		}
		ಯದಿ (off >= el->offset + el->length) {
			ಅನುವರ್ತ;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		ಯದಿ (off < el->offset) {
			partial = el->offset - off;
			ಯದಿ (transaction_read(tdb, off, buf, partial, cv) != 0) {
				ಗಚ್ಛ fail;
			}
			len -= partial;
			off += partial;
			buf = (ಶೂನ್ಯ *)(partial + (ವರ್ಣ *)buf);
		}
		ಯದಿ (off + len <= el->offset + el->length) {
			partial = len;
		} ಅನ್ಯಥಾ {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		ಯದಿ (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (ಶೂನ್ಯ *)(partial + (ವರ್ಣ *)buf);

		ಯದಿ (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			ಗಚ್ಛ fail;
		}

		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* its not in the transaction elements - do a real read */
	ಪ್ರತ್ಯಾಗಮ tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ಪ್ರತ್ಯಾಗಮ -1;
}


/*
  write while in a transaction
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_write(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off,
			     ಧ್ರುವ ಶೂನ್ಯ *buf, tdb_len_t len)
{
	ಸಂರಚನಾ tdb_transaction_el *el, *best_el=ರಿಕ್ತ;

	ಯದಿ (len == 0) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	ಯದಿ (len == ಪರಿಮಾಣ(tdb_off_t) && off >= FREELIST_ತುದಿ &&
	    off < FREELIST_ತುದಿ+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_ತುದಿ) / ಪರಿಮಾಣ(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	ಪ್ರತಿ (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		ಯದಿ (best_el == ರಿಕ್ತ && off == el->offset+el->length) {
			best_el = el;
		}

		ಯದಿ (off+len <= el->offset) {
			ಅನುವರ್ತ;
		}
		ಯದಿ (off >= el->offset + el->length) {
			ಅನುವರ್ತ;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		ಯದಿ (off < el->offset) {
			partial = el->offset - off;
			ಯದಿ (transaction_write(tdb, off, buf, partial) != 0) {
				ಗಚ್ಛ fail;
			}
			len -= partial;
			off += partial;
			buf = (ಧ್ರುವ ಶೂನ್ಯ *)(partial + (ಧ್ರುವ ವರ್ಣ *)buf);
		}
		ಯದಿ (off + len <= el->offset + el->length) {
			partial = len;
		} ಅನ್ಯಥಾ {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (ಧ್ರುವ ಶೂನ್ಯ *)(partial + (ಧ್ರುವ ವರ್ಣ *)buf);

		ಯದಿ (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			ಗಚ್ಛ fail;
		}

		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* see if we can append the new entry to an existing entry */
	ಯದಿ (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned ವರ್ಣ *data = best_el->data;
		el = best_el;
		el->data = (unsigned ವರ್ಣ *)realloc(el->data,
						    el->length + len);
		ಯದಿ (el->data == ರಿಕ್ತ) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		ಯದಿ (buf) {
			memcpy(el->data + el->length, buf, len);
		} ಅನ್ಯಥಾ {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* add a new entry at the end of the list */
	el = (ಸಂರಚನಾ tdb_transaction_el *)malloc(ಪರಿಮಾಣ(*el));
	ಯದಿ (el == ರಿಕ್ತ) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	el->next = ರಿಕ್ತ;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned ವರ್ಣ *)malloc(len);
	ಯದಿ (el->data == ರಿಕ್ತ) {
		ಮುಕ್ತ(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಯದಿ (buf) {
		memcpy(el->data, buf, len);
	} ಅನ್ಯಥಾ {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	ಯದಿ (el->prev) {
		el->prev->next = el;
	} ಅನ್ಯಥಾ {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	ಪ್ರತ್ಯಾಗಮ 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ಪ್ರತ್ಯಾಗಮ -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ಸ್ಥಿರ ಶೂನ್ಯ transaction_next_hash_chain(ಸಂರಚನಾ tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ಪ್ರತಿ (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		ಯದಿ (0 != tdb->transaction->hash_heads[h+1]) {
			ಭಙ್ಗ;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_oob(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t len,
			   ಪೂರ್ಣಾಙ್ಕ probe EXT2FS_ATTR((unused)))
{
	ಯದಿ (len <= tdb->map_size) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_expand_file(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	ಯದಿ (transaction_write(tdb, size, ರಿಕ್ತ, addition) != 0) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
  brlock during a transaction - ignore them
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_brlock(ಸಂರಚನಾ tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      ಪೂರ್ಣಾಙ್ಕ rw_type EXT2FS_ATTR((unused)),
			      ಪೂರ್ಣಾಙ್ಕ lck_type EXT2FS_ATTR((unused)),
			      ಪೂರ್ಣಾಙ್ಕ probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ tdb_methods transaction_methods = {
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
ಪೂರ್ಣಾಙ್ಕ tdb_transaction_start(ಸಂರಚನಾ tdb_context *tdb)
{
	/* some sanity checks */
	ಯದಿ (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	ಯದಿ (tdb->transaction != ರಿಕ್ತ) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (tdb->ಅಂಕೆ_locks != 0 || tdb->global_lock.ಎಣಿಕೆ) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->travlocks.next != ರಿಕ್ತ) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	tdb->transaction = (ಸಂರಚನಾ tdb_transaction *)
		calloc(ಪರಿಮಾಣ(ಸಂರಚನಾ tdb_transaction), 1);
	ಯದಿ (tdb->transaction == ರಿಕ್ತ) {
		tdb->ecode = TDB_ERR_OOM;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	ಯದಿ (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	ಯದಿ (tdb_brlock(tdb, FREELIST_ತುದಿ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ಗಚ್ಛ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, ಪರಿಮಾಣ(u32));
	ಯದಿ (tdb->transaction->hash_heads == ರಿಕ್ತ) {
		tdb->ecode = TDB_ERR_OOM;
		ಗಚ್ಛ fail;
	}
	ಯದಿ (tdb->methods->tdb_read(tdb, FREELIST_ತುದಿ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		ಗಚ್ಛ fail;
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
	ಯದಿ (transaction_write(tdb, FREELIST_ತುದಿ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		ಗಚ್ಛ fail;
	}

	ಪ್ರತ್ಯಾಗಮ 0;

fail:
	tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	ಪ್ರತ್ಯಾಗಮ -1;
}


/*
  cancel the current transaction
*/
ಪೂರ್ಣಾಙ್ಕ tdb_transaction_cancel(ಸಂರಚನಾ tdb_context *tdb)
{
	ಯದಿ (tdb->transaction == ರಿಕ್ತ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	ಯಾವತ್ (tdb->transaction->elements) {
		ಸಂರಚನಾ tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		ಮುಕ್ತ(el->data);
		ಮುಕ್ತ(el);
	}

	/* remove any global lock created during the transaction */
	ಯದಿ (tdb->global_lock.ಎಣಿಕೆ != 0) {
		tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.ಎಣಿಕೆ = 0;
	}

	/* remove any locks created during the transaction */
	ಯದಿ (tdb->ಅಂಕೆ_locks != 0) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಪ್ರತಿ (i=0;i<tdb->ಅಂಕೆ_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_ತುದಿ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->ಅಂಕೆ_locks = 0;
		tdb->ಅಂಕೆ_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
  sync to disk
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_sync(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	ಯದಿ (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}
#ಯದಿ defined(HAVE_MSYNC) && defined(MS_SYNC)
	ಯದಿ (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		ಯದಿ (msync(moffset + (ವರ್ಣ *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}
#endif
	ಪ್ರತ್ಯಾಗಮ 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ಸ್ಥಿರ tdb_len_t tdb_recovery_size(ಸಂರಚನಾ tdb_context *tdb)
{
	ಸಂರಚನಾ tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = ಪರಿಮಾಣ(u32);
	ಪ್ರತಿ (el=tdb->transaction->elements;el;el=el->next) {
		ಯದಿ (el->offset >= tdb->transaction->old_map_size) {
			ಅನುವರ್ತ;
		}
		recovery_size += 2*ಪರಿಮಾಣ(tdb_off_t) + el->length;
	}

	ಪ್ರತ್ಯಾಗಮ recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_recovery_allocate(ಸಂರಚನಾ tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	ಸಂರಚನಾ list_struct rec;
	ಧ್ರುವ ಸಂರಚನಾ tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	ಯದಿ (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	rec.rec_len = 0;

	ಯದಿ (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, ಪರಿಮಾಣ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	ಯದಿ (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	ಯದಿ (recovery_head != 0) {
		ಯದಿ (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(ಪರಿಮಾಣ(rec) + *recovery_size, tdb->page_size) - ಪರಿಮಾಣ(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	ಯದಿ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     ಪರಿಮಾಣ(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	ಯದಿ (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, ಪರಿಮಾಣ(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ transaction_setup_recovery(ಸಂರಚನಾ tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	ಸಂರಚನಾ tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned ವರ್ಣ *data, *p;
	ಧ್ರುವ ಸಂರಚನಾ tdb_methods *methods = tdb->transaction->io_methods;
	ಸಂರಚನಾ list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	ಯದಿ (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	data = (unsigned ವರ್ಣ *)malloc(recovery_size + ಪರಿಮಾಣ(*rec));
	ಯದಿ (data == ರಿಕ್ತ) {
		tdb->ecode = TDB_ERR_OOM;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	rec = (ಸಂರಚನಾ list_struct *)data;
	memset(rec, 0, ಪರಿಮಾಣ(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->ಕೀಲಿ_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + ಪರಿಮಾಣ(*rec);
	ಪ್ರತಿ (el=tdb->transaction->elements;el;el=el->next) {
		ಯದಿ (el->offset >= old_map_size) {
			ಅನುವರ್ತ;
		}
		ಯದಿ (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			ಮುಕ್ತ(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		ಯದಿ (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		ಯದಿ (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			ಮುಕ್ತ(data);
			tdb->ecode = TDB_ERR_IO;
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = ಪರಿಮಾಣ(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	ಯದಿ (methods->tdb_write(tdb, recovery_offset, data, ಪರಿಮಾಣ(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		ಮುಕ್ತ(data);
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	ಯದಿ (transaction_sync(tdb, recovery_offset, ಪರಿಮಾಣ(*rec) + recovery_size) == -1) {
		ಮುಕ್ತ(data);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಮುಕ್ತ(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(ಸಂರಚನಾ list_struct, magic);

	ಯದಿ (methods->tdb_write(tdb, *magic_offset, &magic, ಪರಿಮಾಣ(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* ensure the recovery magic marker is on disk */
	ಯದಿ (transaction_sync(tdb, *magic_offset, ಪರಿಮಾಣ(magic)) == -1) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
  commit the current transaction
*/
ಪೂರ್ಣಾಙ್ಕ tdb_transaction_commit(ಸಂರಚನಾ tdb_context *tdb)
{
	ಧ್ರುವ ಸಂರಚನಾ tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	ಯದಿ (tdb->transaction == ರಿಕ್ತ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* check for a null transaction */
	ಯದಿ (tdb->transaction->elements == ರಿಕ್ತ) {
		tdb_transaction_cancel(tdb);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	ಯದಿ (tdb->ಅಂಕೆ_locks || tdb->global_lock.ಎಣಿಕೆ) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	ಯದಿ (tdb_brlock_upgrade(tdb, FREELIST_ತುದಿ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	ಯದಿ (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		ಯದಿ (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}

	/* expand the file to the new size if needed */
	ಯದಿ (tdb->map_size != tdb->transaction->old_map_size) {
		ಯದಿ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	ಯಾವತ್ (tdb->transaction->elements) {
		ಸಂರಚನಾ tdb_transaction_el *el = tdb->transaction->elements;

		ಯದಿ (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		tdb->transaction->elements = el->next;
		ಮುಕ್ತ(el->data);
		ಮುಕ್ತ(el);
	}

	ಯದಿ (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		ಯದಿ (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			ಪ್ರತ್ಯಾಗಮ -1;
		}

		/* remove the recovery marker */
		ಯದಿ (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			ಪ್ರತ್ಯಾಗಮ -1;
		}

		/* ensure the recovery marker has been removed on disk */
		ಯದಿ (transaction_sync(tdb, magic_offset, 4) == -1) {
			ಪ್ರತ್ಯಾಗಮ -1;
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
	utime(tdb->name, ರಿಕ್ತ);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	ಪ್ರತ್ಯಾಗಮ 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
ಪೂರ್ಣಾಙ್ಕ tdb_transaction_recover(ಸಂರಚನಾ tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned ವರ್ಣ *data, *p;
	u32 zero = 0;
	ಸಂರಚನಾ list_struct rec;

	/* find the recovery area */
	ಯದಿ (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (recovery_head == 0) {
		/* we have never allocated a recovery record */
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* read the recovery record */
	ಯದಿ (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   ಪರಿಮಾಣ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	recovery_eof = rec.ಕೀಲಿ_len;

	data = (unsigned ವರ್ಣ *)malloc(rec.data_len);
	ಯದಿ (data == ರಿಕ್ತ) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* read the full recovery data */
	ಯದಿ (tdb->methods->tdb_read(tdb, recovery_head + ಪರಿಮಾಣ(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		ಮುಕ್ತ(data);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* recover the file data */
	p = data;
	ಯಾವತ್ (p+8 < data + rec.data_len) {
		u32 ofs, len;
		ಯದಿ (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		ಯದಿ (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			ಮುಕ್ತ(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		p += 8 + len;
	}

	ಮುಕ್ತ(data);

	ಯದಿ (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	ಯದಿ (recovery_eof <= recovery_head) {
		ಯದಿ (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}

	/* remove the recovery magic */
	ಯದಿ (tdb_ofs_write(tdb, recovery_head + offsetof(ಸಂರಚನಾ list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	ಯದಿ (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	ಯದಿ (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_rec_free_read(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off, ಸಂರಚನಾ list_struct *rec)
{
	ಯದಿ (tdb->methods->tdb_read(tdb, off, rec, ಪರಿಮಾಣ(*rec),DOCONV()) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		ಯದಿ (tdb->methods->tdb_write(tdb, off, rec, ಪರಿಮಾಣ(*rec)) == -1)
			ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ಯದಿ (tdb->methods->tdb_oob(tdb, rec->next+ಪರಿಮಾಣ(*rec), 0) != 0)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಪ್ರತ್ಯಾಗಮ 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_from_freelist(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_ತುದಿ;
	ಯಾವತ್ (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		ಯದಿ (i == off) {
			/* We've found it! */
			ಪ್ರತ್ಯಾಗಮ tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_tailer(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset,
			 ಧ್ರುವ ಸಂರಚನಾ list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = ಪರಿಮಾಣ(*rec) + rec->rec_len;
	ಪ್ರತ್ಯಾಗಮ tdb_ofs_write(tdb, offset + totalsize - ಪರಿಮಾಣ(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
ಪೂರ್ಣಾಙ್ಕ tdb_free(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t offset, ಸಂರಚನಾ list_struct *rec)
{
	tdb_off_t ಬಲ, ಎಡ;

	/* Allocation and tailer lock */
	ಯದಿ (tdb_lock(tdb, -1, F_WRLCK) != 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	ಯದಿ (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		ಗಚ್ಛ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	ಬಲ = offset + ಪರಿಮಾಣ(*rec) + rec->rec_len;
	ಯದಿ (ಬಲ + ಪರಿಮಾಣ(*rec) <= tdb->map_size) {
		ಸಂರಚನಾ list_struct r;

		ಯದಿ (tdb->methods->tdb_read(tdb, ಬಲ, &r, ಪರಿಮಾಣ(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", ಬಲ));
			ಗಚ್ಛ ಎಡ;
		}

		/* If it's free, expand to include it. */
		ಯದಿ (r.magic == TDB_FREE_MAGIC) {
			ಯದಿ (remove_from_freelist(tdb, ಬಲ, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", ಬಲ));
				ಗಚ್ಛ ಎಡ;
			}
			rec->rec_len += ಪರಿಮಾಣ(r) + r.rec_len;
		}
	}

ಎಡ:
	/* Look left */
	ಎಡ = offset - ಪರಿಮಾಣ(tdb_off_t);
	ಯದಿ (ಎಡ > TDB_DATA_START(tdb->header.hash_size)) {
		ಸಂರಚನಾ list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		ಯದಿ (tdb_ofs_read(tdb, ಎಡ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ಎಡ));
			ಗಚ್ಛ update;
		}

		/* it could be uninitialised data */
		ಯದಿ (leftsize == 0 || leftsize == TDB_PAD_U32) {
			ಗಚ್ಛ update;
		}

		ಎಡ = offset - leftsize;

		/* Now read in record */
		ಯದಿ (tdb->methods->tdb_read(tdb, ಎಡ, &l, ಪರಿಮಾಣ(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ಎಡ, leftsize));
			ಗಚ್ಛ update;
		}

		/* If it's free, expand to include it. */
		ಯದಿ (l.magic == TDB_FREE_MAGIC) {
			ಯದಿ (remove_from_freelist(tdb, ಎಡ, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ಎಡ));
				ಗಚ್ಛ update;
			} ಅನ್ಯಥಾ {
				offset = ಎಡ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	ಯದಿ (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		ಗಚ್ಛ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	ಯದಿ (tdb_ofs_read(tdb, FREELIST_ತುದಿ, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_ತುದಿ, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		ಗಚ್ಛ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ಸ್ಥಿರ tdb_off_t tdb_allocate_ofs(ಸಂರಚನಾ tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				ಸಂರಚನಾ list_struct *rec, tdb_off_t last_ptr)
{
	ಸಂರಚನಾ list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', ಪರಿಮಾಣ(newrec));

	/* found it - now possibly split it up  */
	ಯದಿ (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (ಪರಿಮಾಣ(*rec) + length);
		newrec_ptr = rec_ptr + ಪರಿಮಾಣ(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} ಅನ್ಯಥಾ {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	ಯದಿ (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	ಯದಿ (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* Did we create new block? */
	ಯದಿ (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		ಯದಿ (update_tailer(tdb, rec_ptr, rec) == -1) {
			ಪ್ರತ್ಯಾಗಮ 0;
		}

		/* Free new record */
		ಯದಿ (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	/* all done - return the new record offset */
	ಪ್ರತ್ಯಾಗಮ rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ಸಂರಚನಾ tdb_context *tdb, tdb_len_t length, ಸಂರಚನಾ list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	ಸಂರಚನಾ {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	ಯದಿ (tdb_lock(tdb, -1, F_WRLCK) == -1)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Extra bytes required for tailer */
	length += ಪರಿಮಾಣ(tdb_off_t);

 again:
	last_ptr = FREELIST_ತುದಿ;

	/* read in the freelist top */
	ಯದಿ (tdb_ofs_read(tdb, FREELIST_ತುದಿ, &rec_ptr) == -1)
		ಗಚ್ಛ fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	ಯಾವತ್ (rec_ptr) {
		ಯದಿ (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			ಗಚ್ಛ fail;
		}

		ಯದಿ (rec->rec_len >= length) {
			ಯದಿ (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				ಯದಿ (bestfit.rec_len < 2*length) {
					ಭಙ್ಗ;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	ಯದಿ (bestfit.rec_ptr != 0) {
		ಯದಿ (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			ಗಚ್ಛ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		ಪ್ರತ್ಯಾಗಮ newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	ಯದಿ (tdb_expand(tdb, length + ಪರಿಮಾಣ(*rec)) == 0)
		ಗಚ್ಛ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ seen_ಸೇರಿಸು(ಸಂರಚನಾ tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA ಕೀಲಿ, data;

	memset(&data, '\0', ಪರಿಮಾಣ(data));
	ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *)&rec_ptr;
	ಕೀಲಿ.dsize = ಪರಿಮಾಣ(rec_ptr);
	ಪ್ರತ್ಯಾಗಮ tdb_store(mem_tdb, ಕೀಲಿ, data, TDB_ಸೇರಿಸು);
}

ಪೂರ್ಣಾಙ್ಕ tdb_validate_freelist(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ *pnum_entries)
{
	ಸಂರಚನಾ tdb_context *mem_tdb = ರಿಕ್ತ;
	ಸಂರಚನಾ list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	ಯದಿ (!mem_tdb) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	last_ptr = FREELIST_ತುದಿ;

	/* Store the FREELIST_TOP record. */
	ಯದಿ (seen_ಸೇರಿಸು(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		ಗಚ್ಛ fail;
	}

	/* read in the freelist top */
	ಯದಿ (tdb_ofs_read(tdb, FREELIST_ತುದಿ, &rec_ptr) == -1) {
		ಗಚ್ಛ fail;
	}

	ಯಾವತ್ (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		ಯದಿ (seen_ಸೇರಿಸು(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			ಗಚ್ಛ fail;
		}

		ಯದಿ (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			ಗಚ್ಛ fail;
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
	ಪ್ರತ್ಯಾಗಮ ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_next_lock(ಸಂರಚನಾ tdb_context *tdb, ಸಂರಚನಾ tdb_traverse_lock *tlock,
			 ಸಂರಚನಾ list_struct *rec)
{
	ಪೂರ್ಣಾಙ್ಕ want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	ಪ್ರತಿ (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		ಯದಿ (!tlock->off && tlock->hash != 0) {
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
			ಯದಿ (tlock->hash == tdb->header.hash_size) {
				ಅನುವರ್ತ;
			}
		}

		ಯದಿ (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			ಪ್ರತ್ಯಾಗಮ -1;

		/* No previous record?  Start at top of chain. */
		ಯದಿ (!tlock->off) {
			ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(tlock->hash),
				     &tlock->off) == -1)
				ಗಚ್ಛ fail;
		} ಅನ್ಯಥಾ {
			/* Otherwise unlock the previous record. */
			ಯದಿ (tdb_unlock_record(tdb, tlock->off) != 0)
				ಗಚ್ಛ fail;
		}

		ಯದಿ (want_next) {
			/* We have offset of old record: grab next */
			ಯದಿ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ಗಚ್ಛ fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		ಯಾವತ್( tlock->off) {
			tdb_off_t current;
			ಯದಿ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ಗಚ್ಛ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			ಯದಿ (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				ಗಚ್ಛ fail;
			}

			ಯದಿ (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				ಯದಿ (tdb_lock_record(tdb, tlock->off) != 0)
					ಗಚ್ಛ fail;
				ಪ್ರತ್ಯಾಗಮ tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			ಯದಿ (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				ಗಚ್ಛ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	ಯದಿ (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	ಪ್ರತ್ಯಾಗಮ -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_traverse_internal(ಸಂರಚನಾ tdb_context *tdb,
				 tdb_traverse_func fn, ಶೂನ್ಯ *private_data,
				 ಸಂರಚನಾ tdb_traverse_lock *tl)
{
	TDB_DATA ಕೀಲಿ, dbuf;
	ಸಂರಚನಾ list_struct rec;
	ಪೂರ್ಣಾಙ್ಕ ret, ಎಣಿಕೆ = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	ಯಾವತ್ ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		ಎಣಿಕೆ++;
		/* now read the full record */
		ಕೀಲಿ.dptr = tdb_alloc_read(tdb, tl->off + ಪರಿಮಾಣ(rec),
					  rec.ಕೀಲಿ_len + rec.data_len);
		ಯದಿ (!ಕೀಲಿ.dptr) {
			ret = -1;
			ಯದಿ (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				ಗಚ್ಛ out;
			ಯದಿ (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			ಗಚ್ಛ out;
		}
		ಕೀಲಿ.dsize = rec.ಕೀಲಿ_len;
		dbuf.dptr = ಕೀಲಿ.dptr + rec.ಕೀಲಿ_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		ಯದಿ (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(ಕೀಲಿ.dptr);
			ಗಚ್ಛ out;
		}
		ಯದಿ (fn && fn(tdb, ಕೀಲಿ, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = ಎಣಿಕೆ;
			ಯದಿ (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(ಕೀಲಿ.dptr);
			ಗಚ್ಛ out;
		}
		SAFE_FREE(ಕೀಲಿ.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	ಯದಿ (ret < 0)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}


/*
  a write style traverse - temporarily marks the db read only
*/
ಪೂರ್ಣಾಙ್ಕ tdb_traverse_read(ಸಂರಚನಾ tdb_context *tdb,
		      tdb_traverse_func fn, ಶೂನ್ಯ *private_data)
{
	ಸಂರಚನಾ tdb_traverse_lock tl = { ರಿಕ್ತ, 0, 0, F_RDLCK };
	ಪೂರ್ಣಾಙ್ಕ ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	ಯದಿ (tdb_transaction_lock(tdb, F_RDLCK)) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
ಪೂರ್ಣಾಙ್ಕ tdb_traverse(ಸಂರಚನಾ tdb_context *tdb,
		 tdb_traverse_func fn, ಶೂನ್ಯ *private_data)
{
	ಸಂರಚನಾ tdb_traverse_lock tl = { ರಿಕ್ತ, 0, 0, F_WRLCK };
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (tdb->read_only || tdb->traverse_read) {
		ಪ್ರತ್ಯಾಗಮ tdb_traverse_read(tdb, fn, private_data);
	}

	ಯದಿ (tdb_transaction_lock(tdb, F_WRLCK)) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	ಪ್ರತ್ಯಾಗಮ ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(ಸಂರಚನಾ tdb_context *tdb)
{
	TDB_DATA ಕೀಲಿ;
	ಸಂರಚನಾ list_struct rec;

	/* release any old lock */
	ಯದಿ (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		ಪ್ರತ್ಯಾಗಮ tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	ಯದಿ (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		ಪ್ರತ್ಯಾಗಮ tdb_null;
	/* now read the key */
	ಕೀಲಿ.dsize = rec.ಕೀಲಿ_len;
	ಕೀಲಿ.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+ಪರಿಮಾಣ(rec),ಕೀಲಿ.dsize);

	/* Unlock the hash chain of the record we just read. */
	ಯದಿ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA ಕೀಲಿ = tdb_null;
	ಸಂರಚನಾ list_struct rec;
	unsigned ವರ್ಣ *k = ರಿಕ್ತ;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	ಯದಿ (tdb->travlocks.off) {
		ಯದಿ (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			ಪ್ರತ್ಯಾಗಮ tdb_null;
		ಯದಿ (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+ಪರಿಮಾಣ(rec),
					    rec.ಕೀಲಿ_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			ಯದಿ (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				ಪ್ರತ್ಯಾಗಮ tdb_null;
			}
			ಯದಿ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				ಪ್ರತ್ಯಾಗಮ tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	ಯದಿ (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		ಯದಿ (!tdb->travlocks.off)
			ಪ್ರತ್ಯಾಗಮ tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		ಯದಿ (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			ಪ್ರತ್ಯಾಗಮ tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	ಯದಿ (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		ಕೀಲಿ.dsize = rec.ಕೀಲಿ_len;
		ಕೀಲಿ.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+ಪರಿಮಾಣ(rec),
					  ಕೀಲಿ.dsize);
		/* Unlock the chain of this new record */
		ಯದಿ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	ಯದಿ (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}

/* file: dump.c */

ಸ್ಥಿರ tdb_off_t tdb_dump_record(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ hash,
				 tdb_off_t offset)
{
	ಸಂರಚನಾ list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	ಯದಿ (tdb->methods->tdb_read(tdb, offset, (ವರ್ಣ *)&rec,
				   ಪರಿಮಾಣ(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.ಕೀಲಿ_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + ಪರಿಮಾಣ(rec) + rec.rec_len - ಪರಿಮಾಣ(tdb_off_t);

	ಯದಿ (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		ಪ್ರತ್ಯಾಗಮ rec.next;
	}

	ಯದಿ (tailer != rec.rec_len + ಪರಿಮಾಣ(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned ಪೂರ್ಣಾಙ್ಕ)tailer, (unsigned ಪೂರ್ಣಾಙ್ಕ)(rec.rec_len + ಪರಿಮಾಣ(rec)));
	}
	ಪ್ರತ್ಯಾಗಮ rec.next;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_dump_chain(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ i)
{
	tdb_off_t rec_ptr, ತುದಿ;

	ತುದಿ = TDB_HASH_ತುದಿ(i);

	ಯದಿ (tdb_lock(tdb, i, F_WRLCK) != 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (tdb_ofs_read(tdb, ತುದಿ, &rec_ptr) == -1)
		ಪ್ರತ್ಯಾಗಮ tdb_unlock(tdb, i, F_WRLCK);

	ಯದಿ (rec_ptr)
		printf("hash=%d\n", i);

	ಯಾವತ್ (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	ಪ್ರತ್ಯಾಗಮ tdb_unlock(tdb, i, F_WRLCK);
}

ಶೂನ್ಯ tdb_dump_all(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

ಪೂರ್ಣಾಙ್ಕ tdb_printfreelist(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	ಸಂರಚನಾ list_struct rec;

	ಯದಿ ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		ಪ್ರತ್ಯಾಗಮ ret;

	offset = FREELIST_ತುದಿ;

	/* read in the freelist top */
	ಯದಿ (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	ಯಾವತ್ (rec_ptr) {
		ಯದಿ (tdb->methods->tdb_read(tdb, rec_ptr, (ವರ್ಣ *)&rec,
					   ಪರಿಮಾಣ(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			ಪ್ರತ್ಯಾಗಮ -1;
		}

		ಯದಿ (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			ಪ್ರತ್ಯಾಗಮ -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (ಪೂರ್ಣಾಙ್ಕ)total_free,
               (ಪೂರ್ಣಾಙ್ಕ)total_free);

	ಪ್ರತ್ಯಾಗಮ tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ಶೂನ್ಯ tdb_increment_seqnum_nonblock(ಸಂರಚನಾ tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ಯದಿ (!(tdb->flags & TDB_SEQNUM)) {
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	ಯದಿ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ಪ್ರತ್ಯಾಗಮ;
	seqnum++;
	(ಶೂನ್ಯ) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ಸ್ಥಿರ ಶೂನ್ಯ tdb_increment_seqnum(ಸಂರಚನಾ tdb_context *tdb)
{
	ಯದಿ (!(tdb->flags & TDB_SEQNUM)) {
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		ಪ್ರತ್ಯಾಗಮ;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_ಕೀಲಿ_compare(TDB_DATA ಕೀಲಿ, TDB_DATA data,
			   ಶೂನ್ಯ *private_data EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ memcmp(data.dptr, ಕೀಲಿ.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ಸ್ಥಿರ tdb_off_t tdb_find(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, u32 hash,
			ಸಂರಚನಾ list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* keep looking until we find the right record */
	ಯಾವತ್ (rec_ptr) {
		ಯದಿ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (!TDB_DEAD(r) && hash==r->full_hash
		    && ಕೀಲಿ.dsize==r->ಕೀಲಿ_len
		    && tdb_parse_data(tdb, ಕೀಲಿ, rec_ptr + ಪರಿಮಾಣ(*r),
				      r->ಕೀಲಿ_len, tdb_ಕೀಲಿ_compare,
				      ರಿಕ್ತ) == 0) {
			ಪ್ರತ್ಯಾಗಮ rec_ptr;
		}
		rec_ptr = r->next;
	}
	ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, u32 hash, ಪೂರ್ಣಾಙ್ಕ locktype,
			   ಸಂರಚನಾ list_struct *rec)
{
	u32 rec_ptr;

	ಯದಿ (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (!(rec_ptr = tdb_find(tdb, ಕೀಲಿ, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	ಪ್ರತ್ಯಾಗಮ rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_update_hash(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, u32 hash, TDB_DATA dbuf)
{
	ಸಂರಚನಾ list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	ಯದಿ (!(rec_ptr = tdb_find(tdb, ಕೀಲಿ, hash, &rec)))
		ಪ್ರತ್ಯಾಗಮ -1;

	/* must be long enough key, data and tailer */
	ಯದಿ (rec.rec_len < ಕೀಲಿ.dsize + dbuf.dsize + ಪರಿಮಾಣ(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (tdb->methods->tdb_write(tdb, rec_ptr + ಪರಿಮಾಣ(rec) + rec.ಕೀಲಿ_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		ಪ್ರತ್ಯಾಗಮ tdb_rec_write(tdb, rec_ptr, &rec);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	tdb_off_t rec_ptr;
	ಸಂರಚನಾ list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);
	ಯದಿ (!(rec_ptr = tdb_find_lock_hash(tdb,ಕೀಲಿ,hash,F_RDLCK,&rec)))
		ಪ್ರತ್ಯಾಗಮ tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + ಪರಿಮಾಣ(rec) + rec.ಕೀಲಿ_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ಪ್ರತ್ಯಾಗಮ ret;
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

ಪೂರ್ಣಾಙ್ಕ tdb_parse_record(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ,
		     ಪೂರ್ಣಾಙ್ಕ (*parser)(TDB_DATA ಕೀಲಿ, TDB_DATA data,
				   ಶೂನ್ಯ *private_data),
		     ಶೂನ್ಯ *private_data)
{
	tdb_off_t rec_ptr;
	ಸಂರಚನಾ list_struct rec;
	ಪೂರ್ಣಾಙ್ಕ ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);

	ಯದಿ (!(rec_ptr = tdb_find_lock_hash(tdb,ಕೀಲಿ,hash,F_RDLCK,&rec))) {
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, ಕೀಲಿ, rec_ptr + ಪರಿಮಾಣ(rec) + rec.ಕೀಲಿ_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	ಪ್ರತ್ಯಾಗಮ ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_exists_hash(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, u32 hash)
{
	ಸಂರಚನಾ list_struct rec;

	ಯದಿ (tdb_find_lock_hash(tdb, ಕೀಲಿ, hash, F_RDLCK, &rec) == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ tdb_exists(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	u32 hash = tdb->hash_fn(&ಕೀಲಿ);
	ಪ್ರತ್ಯಾಗಮ tdb_exists_hash(tdb, ಕೀಲಿ, hash);
}

/* actually delete an entry in the database given the offset */
ಪೂರ್ಣಾಙ್ಕ tdb_do_delete(ಸಂರಚನಾ tdb_context *tdb, tdb_off_t rec_ptr, ಸಂರಚನಾ list_struct*rec)
{
	tdb_off_t last_ptr, i;
	ಸಂರಚನಾ list_struct lastrec;

	ಯದಿ (tdb->read_only || tdb->traverse_read) ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		ಪ್ರತ್ಯಾಗಮ tdb_rec_write(tdb, rec_ptr, rec);
	}
	ಯದಿ (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	/* find previous record in hash chain */
	ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(rec->full_hash), &i) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಪ್ರತಿ (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		ಯದಿ (tdb_rec_read(tdb, i, &lastrec) == -1)
			ಪ್ರತ್ಯಾಗಮ -1;

	/* unlink it: next ptr is at start of record. */
	ಯದಿ (last_ptr == 0)
		last_ptr = TDB_HASH_ತುದಿ(rec->full_hash);
	ಯದಿ (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;

	/* recover the space */
	ಯದಿ (tdb_free(tdb, rec_ptr, rec) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_ಎಣಿಕೆ_dead(ಸಂರಚನಾ tdb_context *tdb, u32 hash)
{
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	tdb_off_t rec_ptr;
	ಸಂರಚನಾ list_struct rec;

	/* read in the hash top */
	ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯಾವತ್ (rec_ptr) {
		ಯದಿ (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	ಪ್ರತ್ಯಾಗಮ res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_purge_dead(ಸಂರಚನಾ tdb_context *tdb, u32 hash)
{
	ಪೂರ್ಣಾಙ್ಕ res = -1;
	ಸಂರಚನಾ list_struct rec;
	tdb_off_t rec_ptr;

	ಯದಿ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* read in the hash top */
	ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಗಚ್ಛ fail;

	ಯಾವತ್ (rec_ptr) {
		tdb_off_t next;

		ಯದಿ (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			ಗಚ್ಛ fail;
		}

		next = rec.next;

		ಯದಿ (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			ಗಚ್ಛ fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ res;
}

/* delete an entry in the database given a key */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_delete_hash(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, u32 hash)
{
	tdb_off_t rec_ptr;
	ಸಂರಚನಾ list_struct rec;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		ಯದಿ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			ಪ್ರತ್ಯಾಗಮ -1;

		ಯದಿ (tdb_ಎಣಿಕೆ_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		ಯದಿ (!(rec_ptr = tdb_find(tdb, ಕೀಲಿ, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			ಪ್ರತ್ಯಾಗಮ -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	ಅನ್ಯಥಾ {
		ಯದಿ (!(rec_ptr = tdb_find_lock_hash(tdb, ಕೀಲಿ, hash, F_WRLCK,
						   &rec)))
			ಪ್ರತ್ಯಾಗಮ -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	ಯದಿ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ಯದಿ (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ tdb_delete(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ)
{
	u32 hash = tdb->hash_fn(&ಕೀಲಿ);
	ಪ್ರತ್ಯಾಗಮ tdb_delete_hash(tdb, ಕೀಲಿ, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ಸ್ಥಿರ tdb_off_t tdb_find_dead(ಸಂರಚನಾ tdb_context *tdb, u32 hash,
			       ಸಂರಚನಾ list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* keep looking until we find the right record */
	ಯಾವತ್ (rec_ptr) {
		ಯದಿ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			ಪ್ರತ್ಯಾಗಮ rec_ptr;
		}
		rec_ptr = r->next;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
ಪೂರ್ಣಾಙ್ಕ tdb_store(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಪೂರ್ಣಾಙ್ಕ flag)
{
	ಸಂರಚನಾ list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	ವರ್ಣ *p = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	ಯದಿ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);
	ಯದಿ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;

	/* check for it existing, on insert. */
	ಯದಿ (flag == TDB_ಸೇರಿಸು) {
		ಯದಿ (tdb_exists_hash(tdb, ಕೀಲಿ, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			ಗಚ್ಛ fail;
		}
	} ಅನ್ಯಥಾ {
		/* first try in-place update, on modify or replace. */
		ಯದಿ (tdb_update_hash(tdb, ಕೀಲಿ, hash, dbuf) == 0) {
			ಗಚ್ಛ done;
		}
		ಯದಿ (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			ಗಚ್ಛ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	ಯದಿ (flag != TDB_ಸೇರಿಸು)
		tdb_delete_hash(tdb, ಕೀಲಿ, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	ಯದಿ (!(p = (ವರ್ಣ *)malloc(ಕೀಲಿ.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		ಗಚ್ಛ fail;
	}

	memcpy(p, ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	ಯದಿ (dbuf.dsize)
		memcpy(p+ಕೀಲಿ.dsize, dbuf.dptr, dbuf.dsize);

	ಯದಿ (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			ಕೀಲಿ.dsize + dbuf.dsize + ಪರಿಮಾಣ(tdb_off_t));

		ಯದಿ (rec_ptr != 0) {
			rec.ಕೀಲಿ_len = ಕೀಲಿ.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			ಯದಿ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + ಪರಿಮಾಣ(rec),
				    p, ಕೀಲಿ.dsize + dbuf.dsize) == -1) {
				ಗಚ್ಛ fail;
			}
			ಗಚ್ಛ done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	ಯದಿ (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		ಗಚ್ಛ fail;
	}

	ಯದಿ ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ಗಚ್ಛ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, ಕೀಲಿ.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	ಯದಿ (rec_ptr == 0) {
		ಗಚ್ಛ fail;
	}

	/* Read hash top into next ptr */
	ಯದಿ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec.next) == -1)
		ಗಚ್ಛ fail;

	rec.ಕೀಲಿ_len = ಕೀಲಿ.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	ಯದಿ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+ಪರಿಮಾಣ(rec), p, ಕೀಲಿ.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		ಗಚ್ಛ fail;
	}

 done:
	ret = 0;
 fail:
	ಯದಿ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	ಪ್ರತ್ಯಾಗಮ ret;
}


/* Append to an entry. Create if not exist. */
ಪೂರ್ಣಾಙ್ಕ tdb_append(ಸಂರಚನಾ tdb_context *tdb, TDB_DATA ಕೀಲಿ, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);
	ಯದಿ (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ಪ್ರತ್ಯಾಗಮ -1;

	dbuf = tdb_fetch(tdb, ಕೀಲಿ);

	ಯದಿ (dbuf.dptr == ರಿಕ್ತ) {
		dbuf.dptr = (unsigned ವರ್ಣ *)malloc(new_dbuf.dsize);
	} ಅನ್ಯಥಾ {
		unsigned ವರ್ಣ *new_dptr = (unsigned ವರ್ಣ *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		ಯದಿ (new_dptr == ರಿಕ್ತ) {
			ಮುಕ್ತ(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	ಯದಿ (dbuf.dptr == ರಿಕ್ತ) {
		tdb->ecode = TDB_ERR_OOM;
		ಗಚ್ಛ failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, ಕೀಲಿ, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	ಪ್ರತ್ಯಾಗಮ ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ಧ್ರುವ ವರ್ಣ *tdb_name(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
ಪೂರ್ಣಾಙ್ಕ tdb_fd(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->log.log_fn;
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
ಪೂರ್ಣಾಙ್ಕ tdb_get_seqnum(ಸಂರಚನಾ tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ಯದಿ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ seqnum;
}

ಪೂರ್ಣಾಙ್ಕ tdb_hash_size(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->header.hash_size;
}

size_t tdb_map_size(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->map_size;
}

ಪೂರ್ಣಾಙ್ಕ tdb_get_flags(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
ಶೂನ್ಯ tdb_enable_seqnum(ಸಂರಚನಾ tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ಸ್ಥಿರ ಸಂರಚನಾ tdb_context *tdbs = ರಿಕ್ತ;


/* This is from a hash algorithm suggested by Rogier Wolff */
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ default_tdb_hash(TDB_DATA *ಕೀಲಿ)
{
	u32 ಮೌಲ್ಯ;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	ಪ್ರತಿ (ಮೌಲ್ಯ = 0, i=0; i < ಕೀಲಿ->dsize; i++)
		ಮೌಲ್ಯ = ಮೌಲ್ಯ * 256 + ಕೀಲಿ->dptr[i] + (ಮೌಲ್ಯ >> 24) * 241;

	ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


/* initialise a new database with a specified hash size */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_new_database(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ hash_size)
{
	ಸಂರಚನಾ tdb_header *newdb;
	ಪೂರ್ಣಾಙ್ಕ size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = ಪರಿಮಾಣ(ಸಂರಚನಾ tdb_header) + (hash_size+1)*ಪರಿಮಾಣ(tdb_off_t);
	ಯದಿ (!(newdb = (ಸಂರಚನಾ tdb_header *)calloc(size, 1)))
		ಪ್ರತ್ಯಾಗಮ TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	ಯದಿ (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (ವರ್ಣ *)newdb;
		memcpy(&tdb->header, newdb, ಪರಿಮಾಣ(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (lseek(tdb->fd, 0, SEEK_SET) == -1)
		ಗಚ್ಛ fail;

	ಯದಿ (ftruncate(tdb->fd, 0) == -1)
		ಗಚ್ಛ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, ಪರಿಮಾಣ(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	ಯದಿ (ಲಿಖ(tdb->fd, newdb, size) != size) {
		ret = -1;
	} ಅನ್ಯಥಾ {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	ಪ್ರತ್ಯಾಗಮ ret;
}



ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tdb_already_open(dev_t device,
			    ino_t ino)
{
	ಸಂರಚನಾ tdb_context *i;

	ಪ್ರತಿ (i = tdbs; i; i = i->next) {
		ಯದಿ (i->device == device && i->inode == ino) {
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ಸಂರಚನಾ tdb_context *tdb_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ hash_size, ಪೂರ್ಣಾಙ್ಕ tdb_flags,
		      ಪೂರ್ಣಾಙ್ಕ open_flags, mode_t mode)
{
	ಪ್ರತ್ಯಾಗಮ tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, ರಿಕ್ತ, ರಿಕ್ತ);
}

/* a default logging function */
ಸ್ಥಿರ ಶೂನ್ಯ null_log_fn(ಸಂರಚನಾ tdb_context *tdb, ಗಣನಾ tdb_debug_level level, ಧ್ರುವ ವರ್ಣ *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ಸ್ಥಿರ ಶೂನ್ಯ null_log_fn(ಸಂರಚನಾ tdb_context *tdb EXT2FS_ATTR((unused)),
			ಗಣನಾ tdb_debug_level level EXT2FS_ATTR((unused)),
			ಧ್ರುವ ವರ್ಣ *fmt EXT2FS_ATTR((unused)), ...)
{
}


ಸಂರಚನಾ tdb_context *tdb_open_ex(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ hash_size, ಪೂರ್ಣಾಙ್ಕ tdb_flags,
				ಪೂರ್ಣಾಙ್ಕ open_flags, mode_t mode,
				ಧ್ರುವ ಸಂರಚನಾ tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ಸಂರಚನಾ tdb_context *tdb;
	ಸಂರಚನಾ stat st;
	ಪೂರ್ಣಾಙ್ಕ rev = 0, locked = 0;
	unsigned ವರ್ಣ *vp;
	u32 vertest;

	ಯದಿ (!(tdb = (ಸಂರಚನಾ tdb_context *)calloc(1, ಪರಿಮಾಣ *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		ಗಚ್ಛ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ರಿಕ್ತ;
	tdb->map_ptr = ರಿಕ್ತ;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	ಯದಿ (log_ctx) {
		tdb->log = *log_ctx;
	} ಅನ್ಯಥಾ {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = ರಿಕ್ತ;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	ಯದಿ (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	ಯದಿ ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		ಗಚ್ಛ fail;
	}

	ಯದಿ (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	ಯದಿ ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	ಯದಿ (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		ಯದಿ (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			ಗಚ್ಛ fail;
		}
		ಗಚ್ಛ internal;
	}

	ಯದಿ ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		ಗಚ್ಛ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	ಯದಿ (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		ಗಚ್ಛ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	ಯದಿ ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		ಯದಿ (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			ಗಚ್ಛ fail; /* errno set by ftruncate */
		}
	}

	ಯದಿ (ಪಠ(tdb->fd, &tdb->header, ಪರಿಮಾಣ(tdb->header)) != ಪರಿಮಾಣ(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      ಪರಿಮಾಣ(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		ಯದಿ (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			ಗಚ್ಛ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned ವರ್ಣ *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	ಯದಿ (!rev)
		tdb->flags &= ~TDB_CONVERT;
	ಅನ್ಯಥಾ {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, ಪರಿಮಾಣ(tdb->header));
	}
	ಯದಿ (fstat(tdb->fd, &st) == -1)
		ಗಚ್ಛ fail;

	ಯದಿ (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		ಗಚ್ಛ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	ಯದಿ (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (ಪೂರ್ಣಾಙ್ಕ)st.st_dev, (ಪೂರ್ಣಾಙ್ಕ)st.st_ino));
		errno = EBUSY;
		ಗಚ್ಛ fail;
	}

	ಯದಿ (!(tdb->name = (ವರ್ಣ *)strdup(name))) {
		errno = ENOMEM;
		ಗಚ್ಛ fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	ಯದಿ (locked) {
		ಯದಿ (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			ಗಚ್ಛ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	ಯದಿ (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		ಯದಿ (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			ಗಚ್ಛ fail;
	}

	/* if needed, run recovery */
	ಯದಿ (tdb_transaction_recover(tdb) == -1) {
		ಗಚ್ಛ fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	ಯದಿ (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		ಗಚ್ಛ fail;
	tdb->next = tdbs;
	tdbs = tdb;
	ಪ್ರತ್ಯಾಗಮ tdb;

 fail:
	{ ಪೂರ್ಣಾಙ್ಕ save_errno = errno;

	ಯದಿ (!tdb)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (tdb->map_ptr) {
		ಯದಿ (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		ಅನ್ಯಥಾ
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	ಯದಿ (tdb->fd != -1)
		ಯದಿ (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

ಶೂನ್ಯ tdb_set_max_dead(ಸಂರಚನಾ tdb_context *tdb, ಪೂರ್ಣಾಙ್ಕ max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
ಪೂರ್ಣಾಙ್ಕ tdb_close(ಸಂರಚನಾ tdb_context *tdb)
{
	ಸಂರಚನಾ tdb_context **i;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	ಯದಿ (tdb->map_ptr) {
		ಯದಿ (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		ಅನ್ಯಥಾ
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	ಯದಿ (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	ಪ್ರತಿ (i = &tdbs; *i; i = &(*i)->next) {
		ಯದಿ (*i == tdb) {
			*i = tdb->next;
			ಭಙ್ಗ;
		}
	}

	memset(tdb, 0, ಪರಿಮಾಣ(*tdb));
	SAFE_FREE(tdb);

	ಪ್ರತ್ಯಾಗಮ ret;
}

/* register a logging function */
ಶೂನ್ಯ tdb_set_logging_function(ಸಂರಚನಾ tdb_context *tdb,
                              ಧ್ರುವ ಸಂರಚನಾ tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

ಶೂನ್ಯ *tdb_get_logging_private(ಸಂರಚನಾ tdb_context *tdb)
{
	ಪ್ರತ್ಯಾಗಮ tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
ಪೂರ್ಣಾಙ್ಕ tdb_reopen(ಸಂರಚನಾ tdb_context *tdb)
{
	ಸಂರಚನಾ stat st;

	ಯದಿ (tdb->flags & TDB_INTERNAL) {
		ಪ್ರತ್ಯಾಗಮ 0; /* Nothing to do. */
	}

	ಯದಿ (tdb->ಅಂಕೆ_locks != 0 || tdb->global_lock.ಎಣಿಕೆ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		ಗಚ್ಛ fail;
	}

	ಯದಿ (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		ಗಚ್ಛ fail;
	}

	ಯದಿ (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		ಗಚ್ಛ fail;
	}
	ಯದಿ (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	ಯದಿ (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		ಗಚ್ಛ fail;
	}
	ಯದಿ ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		ಗಚ್ಛ fail;
	}
	ಯದಿ (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		ಗಚ್ಛ fail;
	}
	ಯದಿ (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		ಗಚ್ಛ fail;
	}
	tdb_mmap(tdb);

	ಪ್ರತ್ಯಾಗಮ 0;

fail:
	tdb_close(tdb);
	ಪ್ರತ್ಯಾಗಮ -1;
}

/* reopen all tdb's */
ಪೂರ್ಣಾಙ್ಕ tdb_reopen_all(ಪೂರ್ಣಾಙ್ಕ parent_longlived)
{
	ಸಂರಚನಾ tdb_context *tdb;

	ಪ್ರತಿ (tdb=tdbs; tdb; tdb = tdb->next) {
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
		ಯದಿ (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		ಯದಿ (tdb_reopen(tdb) != 0)
			ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Flush a database file from the page cache.
 **/
ಪೂರ್ಣಾಙ್ಕ tdb_flush(ಸಂರಚನಾ tdb_context *tdb)
{
	ಯದಿ (tdb->fd != -1)
		ಪ್ರತ್ಯಾಗಮ fsync(tdb->fd);
	ಪ್ರತ್ಯಾಗಮ 0;
}
