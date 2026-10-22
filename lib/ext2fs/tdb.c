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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ifdef HAVE_SYS_SELECT_H
#ଅନ୍ତର୍ଭାଵ <sys/select.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ifdef HAVE_UTIME_H
#ଅନ୍ତର୍ଭାଵ <utime.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/file.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>

#ifdef HAVE_SYS_MMAN_H
#ଅନ୍ତର୍ଭାଵ <sys/mman.h>
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#ଅନ୍ୟଥା
#define EXT2FS_ATTR(x)
#endif

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((ଶୂନ୍ୟ *)-1)
#endif

#ifndef HAVE_STRDUP
#define strdup rep_strdup
ସ୍ଥିର ଅକ୍ଷର *rep_strdup(ଅଚଳ ଅକ୍ଷର *s)
{
	ଅକ୍ଷର *ret;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା length;

	ଯଦି (!s)
		ଫେରନ୍ତୁ ଖାଲି;
	length = strlen(s);
	ret = malloc(length + 1);
	ଯଦି (ret) {
		strncpy(ret, s, length);
		ret[length] = '\0';
	}
	ଫେରନ୍ତୁ ret;
}
#endif

#ifndef PRINTF_ATTRIBUTE
#ଯଦି (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ଅନ୍ୟଥା
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

#ଯଦି defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା bool;
#endif

#ଅନ୍ତର୍ଭାଵ "tdb.h"

ସ୍ଥିର TDB_DATA tdb_null;

#ifndef u32
#define u32 unsigned
#endif

ପ୍ରକାରବ୍ୟାଖ୍ୟା u32 tdb_len_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା u32 tdb_off_t;

#ifndef offsetof
#define offsetof(t,f) ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)&((t *)0)->f)
#endif

#define TDB_MAGIC_FOOD "TDB file\n"
#define TDB_VERSION (0x26011967 + 6)
#define TDB_MAGIC (0x26011999U)
#define TDB_FREE_MAGIC (~TDB_MAGIC)
#define TDB_DEAD_MAGIC (0xFEE1DEAD)
#define TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#define TDB_ALIGNMENT 4
#define MIN_REC_SIZE (2*ଆକାର(ସଂରଚନା list_struct) + TDB_ALIGNMENT)
#define DEFAULT_HASH_SIZE 131
#define FREELIST_ଶୀର୍ଷ (ଆକାର(ସଂରଚନା tdb_header))
#define TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#define TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#define TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#define TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#define TDB_HASH_ଶୀର୍ଷ(hash) (FREELIST_ଶୀର୍ଷ + (BUCKET(hash)+1)*ଆକାର(tdb_off_t))
#define TDB_HASHTABLE_SIZE(tdb) ((tdb->header.hash_size+1)*ଆକାର(tdb_off_t))
#define TDB_DATA_START(hash_size) TDB_HASH_ଶୀର୍ଷ(hash_size-1)
#define TDB_RECOVERY_HEAD offsetof(ସଂରଚନା tdb_header, recovery_start)
#define TDB_SEQNUM_OFS    offsetof(ସଂରଚନା tdb_header, sequence_ସଂଖ୍ୟା)
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
#define SAFE_FREE(x) କର { ଯଦି ((x) != ଖାଲି) {ମୁକ୍ତ(x); (x)=ଖାଲି;} } ଯେତେବେଳେ(0)
#endif

#define BUCKET(hash) ((hash) % tdb->header.hash_size)

#define DOCONV() (tdb->flags & TDB_CONVERT)
#define CONVERT(x) (DOCONV() ? tdb_convert(&x, ଆକାର(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ସଂରଚନା list_struct {
	tdb_off_t next; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t ଚାବି_len; /* byte length of key */
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
ସଂରଚନା tdb_header {
	ଅକ୍ଷର magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_size; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_ସଂଖ୍ୟା; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ସଂରଚନା tdb_lock_type {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା list;
	u32 ଗଣନା_2;
	u32 ltype;
};

ସଂରଚନା tdb_traverse_lock {
	ସଂରଚନା tdb_traverse_lock *next;
	u32 off;
	u32 hash;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lock_rw;
};


ସଂରଚନା tdb_methods {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*tdb_read)(ସଂରଚନା tdb_context *, tdb_off_t , ଶୂନ୍ୟ *, tdb_len_t , ପୂର୍ଣ୍ଣସଂଖ୍ୟା );
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*tdb_write)(ସଂରଚନା tdb_context *, tdb_off_t, ଅଚଳ ଶୂନ୍ୟ *, tdb_len_t);
	ଶୂନ୍ୟ (*next_hash_chain)(ସଂରଚନା tdb_context *, u32 *);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*tdb_oob)(ସଂରଚନା tdb_context *, tdb_off_t , ପୂର୍ଣ୍ଣସଂଖ୍ୟା );
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*tdb_expand_file)(ସଂରଚନା tdb_context *, tdb_off_t , tdb_off_t );
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*tdb_brlock)(ସଂରଚନା tdb_context *, tdb_off_t , ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, size_t);
};

ସଂରଚନା tdb_context {
	ଅକ୍ଷର *name; /* the name of the database */
	ଶୂନ୍ୟ *map_ptr; /* where it is currently mapped */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd; /* open file descriptor for the database */
	tdb_len_t map_size; /* how much space has been mapped */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_only; /* opened read-only */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା traverse_read; /* read-only traversal */
	ସଂରଚନା tdb_lock_type global_lock;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_lockrecs;
	ସଂରଚନା tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	ଗଣନା TDB_ERROR ecode; /* error code for last tdb error */
	ସଂରଚନା tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ସଂରଚନା tdb_traverse_lock travlocks; /* current traversal locks */
	ସଂରଚନା tdb_context *next; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ସଂରଚନା tdb_logging_context log;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*hash_fn)(TDB_DATA *ଚାବି);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags; /* flags used in the open - needed by reopen */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_locks; /* number of chain locks held */
	ଅଚଳ ସଂରଚନା tdb_methods *methods;
	ସଂରଚନା tdb_transaction *transaction;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା page_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_size; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_munmap(ସଂରଚନା tdb_context *tdb);
ସ୍ଥିର ଶୂନ୍ୟ tdb_mmap(ସଂରଚନା tdb_context *tdb);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lock_nonblock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_unlock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_brlock(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rw_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lck_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା probe, size_t len);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_lock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_unlock(ସଂରଚନା tdb_context *tdb);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_brlock_upgrade(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, size_t len);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_write_lock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_write_unlock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ofs_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ofs_write(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ସ୍ଥିର ଶୂନ୍ୟ *tdb_convert(ଶୂନ୍ୟ *buf, u32 size);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_free(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ସଂରଚନା list_struct *rec);
ସ୍ଥିର tdb_off_t tdb_allocate(ସଂରଚନା tdb_context *tdb, tdb_len_t length, ସଂରଚନା list_struct *rec);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ofs_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ofs_write(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_unlock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_rec_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ସଂରଚନା list_struct *rec);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_rec_write(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ସଂରଚନା list_struct *rec);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_do_delete(ସଂରଚନା tdb_context *tdb, tdb_off_t rec_ptr, ସଂରଚନା list_struct *rec);
ସ୍ଥିର unsigned ଅକ୍ଷର *tdb_alloc_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_parse_data(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି,
		   tdb_off_t offset, tdb_len_t len,
		   ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*parser)(TDB_DATA ଚାବି, TDB_DATA data,
				 ଶୂନ୍ୟ *private_data),
		   ଶୂନ୍ୟ *private_data);
ସ୍ଥିର tdb_off_t tdb_find_lock_hash(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, u32 hash, ପୂର୍ଣ୍ଣସଂଖ୍ୟା locktype,
			   ସଂରଚନା list_struct *rec);
ସ୍ଥିର ଶୂନ୍ୟ tdb_io_init(ସଂରଚନା tdb_context *tdb);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_expand(ସଂରଚନା tdb_context *tdb, tdb_off_t size);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_rec_free_read(ସଂରଚନା tdb_context *tdb, tdb_off_t off,
		      ସଂରଚନା list_struct *rec);


/* file: error.c */

ଗଣନା TDB_ERROR tdb_error(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->ecode;
}

ସ୍ଥିର ସଂରଚନା tdb_errname {
	ଗଣନା TDB_ERROR ecode; ଅଚଳ ଅକ୍ଷର *estring;
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
ଅଚଳ ଅକ୍ଷର *tdb_errorstr(ସଂରଚନା tdb_context *tdb)
{
	u32 i;
	ପାଇଁ (i = 0; i < ଆକାର(emap) / ଆକାର(ସଂରଚନା tdb_errname); i++)
		ଯଦି (tdb->ecode == emap[i].ecode)
			ଫେରନ୍ତୁ emap[i].estring;
	ଫେରନ୍ତୁ "Invalid error code";
}

/* file: lock.c */

#define TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_brlock(ସଂରଚନା tdb_context *tdb, tdb_off_t offset,
	       ପୂର୍ଣ୍ଣସଂଖ୍ୟା rw_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lck_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା probe, size_t len)
{
	ସଂରଚନା flock fl;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (tdb->flags & TDB_NOLOCK) {
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		ଫେରନ୍ତୁ -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	କର {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} ଯେତେବେଳେ (ret == -1 && errno == EINTR);

	ଯଦି (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		ଯଦି (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len));
		}
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	ଫେରନ୍ତୁ 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_brlock_upgrade(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, size_t len)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 1000;
	ଯେତେବେଳେ (ଗଣନା_2--) {
		ସଂରଚନା timeval tv;
		ଯଦି (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			ଫେରନ୍ତୁ 0;
		}
		ଯଦି (errno != EDEADLK) {
			ଭାଙ୍ଗ;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ଖାଲି, ଖାଲି, ଖାଲି, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	ଫେରନ୍ତୁ -1;
}


/* lock a list in the database. list -1 is the alloc list */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _tdb_lock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op)
{
	ସଂରଚନା tdb_lock_type *new_lck;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	ଯଦି (tdb->global_lock.ଗଣନା_2 &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (tdb->global_lock.ଗଣନା_2) {
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ଯଦି (list < -1 || list >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		ଫେରନ୍ତୁ -1;
	}
	ଯଦି (tdb->flags & TDB_NOLOCK)
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (i=0; i<tdb->ଅଙ୍କ_lockrecs; i++) {
		ଯଦି (tdb->lockrecs[i].list == list) {
			ଯଦି (tdb->lockrecs[i].ଗଣନା_2 == 0) {
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
			tdb->lockrecs[i].ଗଣନା_2++;
			ଫେରନ୍ତୁ 0;
		}
	}

	new_lck = (ସଂରଚନା tdb_lock_type *)realloc(
		tdb->lockrecs,
		ଆକାର(*tdb->lockrecs) * (tdb->ଅଙ୍କ_lockrecs+1));
	ଯଦି (new_lck == ଖାଲି) {
		errno = ENOMEM;
		ଫେରନ୍ତୁ -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	ଯଦି (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_ଶୀର୍ଷ+4*list, ltype, op,
				     0, 1)) {
		ଫେରନ୍ତୁ -1;
	}

	tdb->ଅଙ୍କ_locks++;

	tdb->lockrecs[tdb->ଅଙ୍କ_lockrecs].list = list;
	tdb->lockrecs[tdb->ଅଙ୍କ_lockrecs].ଗଣନା_2 = 1;
	tdb->lockrecs[tdb->ଅଙ୍କ_lockrecs].ltype = ltype;
	tdb->ଅଙ୍କ_lockrecs += 1;

	ଫେରନ୍ତୁ 0;
}

/* lock a list in the database. list -1 is the alloc list */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	ଯଦି (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	ଫେରନ୍ତୁ ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lock_nonblock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype)
{
	ଫେରନ୍ତୁ _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_unlock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା tdb_lock_type *lck = ଖାଲି;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	ଯଦି (tdb->global_lock.ଗଣନା_2 &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (tdb->global_lock.ଗଣନା_2) {
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ଯଦି (tdb->flags & TDB_NOLOCK)
		ଫେରନ୍ତୁ 0;

	/* Sanity checks */
	ଯଦି (list < -1 || list >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tdb->header.hash_size) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_size));
		ଫେରନ୍ତୁ ret;
	}

	ପାଇଁ (i=0; i<tdb->ଅଙ୍କ_lockrecs; i++) {
		ଯଦି (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି ((lck == ଖାଲି) || (lck->ଗଣନା_2 == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (lck->ଗଣନା_2 > 1) {
		lck->ଗଣନା_2--;
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	ଯଦି (mark_lock) {
		ret = 0;
	} ଅନ୍ୟଥା {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->ଅଙ୍କ_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	ଯଦି (tdb->ଅଙ୍କ_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->ଅଙ୍କ_lockrecs-1];
	}
	tdb->ଅଙ୍କ_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	ଯଦି (tdb->ଅଙ୍କ_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	ଯଦି (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	ଫେରନ୍ତୁ ret;
}

/*
  get the transaction lock
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_lock(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype)
{
	ଯଦି (tdb->have_transaction_lock || tdb->global_lock.ଗଣନା_2) {
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ଫେରନ୍ତୁ -1;
	}
	tdb->have_transaction_lock = 1;
	ଫେରନ୍ତୁ 0;
}

/*
  release the transaction lock
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_unlock(ସଂରଚନା tdb_context *tdb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ଯଦି (!tdb->have_transaction_lock) {
		ଫେରନ୍ତୁ 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	ଯଦି (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	ଫେରନ୍ତୁ ret;
}




/* lock/unlock entire database */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _tdb_lockall(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	ଯଦି (tdb->read_only || tdb->traverse_read)
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);

	ଯଦି (tdb->global_lock.ଗଣନା_2 && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.ଗଣନା_2++;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (tdb->global_lock.ଗଣନା_2) {
		/* a global lock of a different type exists */
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ଯଦି (tdb->ଅଙ୍କ_locks != 0) {
		/* can't combine global and chain locks */
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ଯଦି (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ, ltype, op,
				     0, 4*tdb->header.hash_size)) {
		ଯଦି (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		ଫେରନ୍ତୁ -1;
	}

	tdb->global_lock.ଗଣନା_2 = 1;
	tdb->global_lock.ltype = ltype;

	ଫେରନ୍ତୁ 0;
}



/* unlock entire db */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _tdb_unlockall(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	ଯଦି (tdb->read_only || tdb->traverse_read) {
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ଯଦି (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.ଗଣନା_2 == 0) {
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	ଯଦି (tdb->global_lock.ଗଣନା_2 > 1) {
		tdb->global_lock.ଗଣନା_2--;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_size)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		ଫେରନ୍ତୁ -1;
	}

	tdb->global_lock.ଗଣନା_2 = 0;
	tdb->global_lock.ltype = 0;

	ଫେରନ୍ତୁ 0;
}

/* lock entire database with write lock */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lockall(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lockall_mark(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lockall_unmark(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lockall_nonblock(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_unlockall(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lockall_read(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lockall_read_nonblock(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_unlockall_read(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainlock(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_lock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainlock_nonblock(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainlock_mark(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_lock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainlock_unmark(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_WRLCK | TDB_MARK_LOCK);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainunlock(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_WRLCK);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainlock_read(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_lock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_RDLCK);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_chainunlock_read(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	ଫେରନ୍ତୁ tdb_unlock(tdb, BUCKET(tdb->hash_fn(&ଚାବି)), F_RDLCK);
}



/* record lock stops delete underneath */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_lock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off)
{
	ଫେରନ୍ତୁ off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_write_lock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off)
{
	ସଂରଚନା tdb_traverse_lock *i;
	ପାଇଁ (i = &tdb->travlocks; i; i = i->next)
		ଯଦି (i->off == off)
			ଫେରନ୍ତୁ -1;
	ଫେରନ୍ତୁ tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_write_unlock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off)
{
	ଫେରନ୍ତୁ tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_unlock_record(ସଂରଚନା tdb_context *tdb, tdb_off_t off)
{
	ସଂରଚନା tdb_traverse_lock *i;
	u32 ଗଣନା_2 = 0;

	ଯଦି (off == 0)
		ଫେରନ୍ତୁ 0;
	ପାଇଁ (i = &tdb->travlocks; i; i = i->next)
		ଯଦି (i->off == off)
			ଗଣନା_2++;
	ଫେରନ୍ତୁ (ଗଣନା_2 == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_oob(ସଂରଚନା tdb_context *tdb, tdb_off_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା probe)
{
	ସଂରଚନା stat st;
	ଯଦି (len <= tdb->map_size)
		ଫେରନ୍ତୁ 0;
	ଯଦି (tdb->flags & TDB_INTERNAL) {
		ଯଦି (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tdb->map_size));
		}
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ଯଦି (fstat(tdb->fd, &st) == -1) {
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ଯଦି (st.st_size < (off_t)len) {
		ଯଦି (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)st.st_size));
		}
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	ଯଦି (tdb_munmap(tdb) == -1)
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_size = st.st_size;
	tdb_mmap(tdb);
	ଫେରନ୍ତୁ 0;
}

/* write a lump of data at a specified offset */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_write(ସଂରଚନା tdb_context *tdb, tdb_off_t off,
		     ଅଚଳ ଶୂନ୍ୟ *buf, tdb_len_t len)
{
	ଯଦି (len == 0) {
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		ଫେରନ୍ତୁ -1;

	ଯଦି (tdb->map_ptr) {
		memcpy(off + (ଅକ୍ଷର *)tdb->map_ptr, buf, len);
	} ଅନ୍ୟଥା ଯଦି (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	ଫେରନ୍ତୁ 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
ଶୂନ୍ୟ *tdb_convert(ଶୂନ୍ୟ *buf, u32 size)
{
	u32 i, *p = (u32 *)buf;
	ପାଇଁ (i = 0; i < size / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	ଫେରନ୍ତୁ buf;
}


/* read a lump of data at a specified offset, maybe convert */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_read(ସଂରଚନା tdb_context *tdb, tdb_off_t off, ଶୂନ୍ୟ *buf,
		    tdb_len_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv)
{
	ଯଦି (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->map_ptr) {
		memcpy(buf, off + (ଅକ୍ଷର *)tdb->map_ptr, len);
	} ଅନ୍ୟଥା {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		ଯଦି (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)off, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ret, strerror(errno),
				 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tdb->map_size));
			ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	ଯଦି (cv) {
		tdb_convert(buf, len);
	}
	ଫେରନ୍ତୁ 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ସ୍ଥିର ଶୂନ୍ୟ tdb_next_hash_chain(ସଂରଚନା tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ଯଦି (tdb->map_ptr) {
		ପାଇଁ (;h < tdb->header.hash_size;h++) {
			ଯଦି (0 != *(u32 *)(TDB_HASH_ଶୀର୍ଷ(h) + (unsigned ଅକ୍ଷର *)tdb->map_ptr)) {
				ଭାଙ୍ଗ;
			}
		}
	} ଅନ୍ୟଥା {
		u32 off=0;
		ପାଇଁ (;h < tdb->header.hash_size;h++) {
			ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(h), &off) != 0 || off != 0) {
				ଭାଙ୍ଗ;
			}
		}
	}
	(*chain) = h;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_munmap(ସଂରଚନା tdb_context *tdb)
{
	ଯଦି (tdb->flags & TDB_INTERNAL)
		ଫେରନ୍ତୁ 0;

#ifdef HAVE_MMAP
	ଯଦି (tdb->map_ptr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = munmap(tdb->map_ptr, tdb->real_map_size);
		ଯଦି (ret != 0)
			ଫେରନ୍ତୁ ret;
		tdb->real_map_size = 0;
	}
#endif
	tdb->map_ptr = ଖାଲି;
	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ tdb_mmap(ସଂରଚନା tdb_context *tdb)
{
	ଯଦି (tdb->flags & TDB_INTERNAL)
		ଫେରନ୍ତୁ;

#ifdef HAVE_MMAP
	ଯଦି (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ଖାଲି, tdb->map_size,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		ଯଦି (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_size = 0;
			tdb->map_ptr = ଖାଲି;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_size, strerror(errno)));
		}
		tdb->real_map_size = tdb->map_size;
	} ଅନ୍ୟଥା {
		tdb->map_ptr = ଖାଲି;
	}
#ଅନ୍ୟଥା
	tdb->map_ptr = ଖାଲି;
#endif
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_expand_file(ସଂରଚନା tdb_context *tdb, tdb_off_t size, tdb_off_t addition)
{
	ଅକ୍ଷର buf[1024];

	ଯଦି (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (ftruncate(tdb->fd, size+addition) == -1) {
		ଅକ୍ଷର b = 0;
		ଯଦି (pwrite(tdb->fd,  &b, 1, (size+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 size+addition, strerror(errno)));
			ଫେରନ୍ତୁ -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, ଆକାର(buf));
	ଯେତେବେଳେ (addition) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = addition>ଆକାର(buf)?ଆକାର(buf):addition;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = pwrite(tdb->fd, buf, n, size);
		ଯଦି (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			ଫେରନ୍ତୁ -1;
		}
		addition -= n;
		size += n;
	}
	ଫେରନ୍ତୁ 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_expand(ସଂରଚନା tdb_context *tdb, tdb_off_t size)
{
	ସଂରଚନା list_struct rec;
	tdb_off_t offset;

	ଯଦି (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		ଫେରନ୍ତୁ -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	size = TDB_ALIGN(tdb->map_size + size*10, tdb->page_size) - tdb->map_size;

	ଯଦି (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	ଯଦି (!(tdb->flags & TDB_INTERNAL)) {
		ଯଦି (tdb->methods->tdb_expand_file(tdb, tdb->map_size, size) != 0)
			ଯାଅ fail;
	}

	tdb->map_size += size;

	ଯଦି (tdb->flags & TDB_INTERNAL) {
		ଅକ୍ଷର *new_map_ptr = (ଅକ୍ଷର *)realloc(tdb->map_ptr,
						    tdb->map_size);
		ଯଦି (!new_map_ptr) {
			tdb->map_size -= size;
			ଯାଅ fail;
		}
		tdb->map_ptr = new_map_ptr;
	} ଅନ୍ୟଥା {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',ଆକାର(rec));
	rec.rec_len = size - ଆକାର(rec);

	/* link it into the free list */
	offset = tdb->map_size - size;
	ଯଦି (tdb_free(tdb, offset, &rec) == -1)
		ଯାଅ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	ଫେରନ୍ତୁ 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ଫେରନ୍ତୁ -1;
}

/* read/write a tdb_off_t */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ofs_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	ଫେରନ୍ତୁ tdb->methods->tdb_read(tdb, offset, (ଅକ୍ଷର*)d, ଆକାର(*d), DOCONV());
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ofs_write(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	ଫେରନ୍ତୁ tdb->methods->tdb_write(tdb, offset, CONVERT(off), ଆକାର(*d));
}


/* read a lump of data, allocating the space for it */
unsigned ଅକ୍ଷର *tdb_alloc_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	unsigned ଅକ୍ଷର *buf;

	/* some systems don't like zero length malloc */
	ଯଦି (len == 0) {
		len = 1;
	}

	ଯଦି (!(buf = (unsigned ଅକ୍ଷର *)malloc(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	ଯଦି (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		ଫେରନ୍ତୁ ଖାଲି;
	}
	ଫେରନ୍ତୁ buf;
}

/* Give a piece of tdb data to a parser */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_parse_data(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି,
		   tdb_off_t offset, tdb_len_t len,
		   ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*parser)(TDB_DATA ଚାବି, TDB_DATA data,
				 ଶୂନ୍ୟ *private_data),
		   ଶୂନ୍ୟ *private_data)
{
	TDB_DATA data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	data.dsize = len;

	ଯଦି ((tdb->transaction == ଖାଲି) && (tdb->map_ptr != ଖାଲି)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		ଯଦି (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			ଫେରନ୍ତୁ -1;
		}
		data.dptr = offset + (unsigned ଅକ୍ଷର *)tdb->map_ptr;
		ଫେରନ୍ତୁ parser(ଚାବି, data, private_data);
	}

	ଯଦି (!(data.dptr = tdb_alloc_read(tdb, offset, len))) {
		ଫେରନ୍ତୁ -1;
	}

	result = parser(ଚାବି, data, private_data);
	ମୁକ୍ତ(data.dptr);
	ଫେରନ୍ତୁ result;
}

/* read/write a record */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_rec_read(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ସଂରଚନା list_struct *rec)
{
	ଯଦି (tdb->methods->tdb_read(tdb, offset, rec, ଆକାର(*rec),DOCONV()) == -1)
		ଫେରନ୍ତୁ -1;
	ଯଦି (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ଫେରନ୍ତୁ tdb->methods->tdb_oob(tdb, rec->next+ଆକାର(*rec), 0);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_rec_write(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ସଂରଚନା list_struct *rec)
{
	ସଂରଚନା list_struct r = *rec;
	ଫେରନ୍ତୁ tdb->methods->tdb_write(tdb, offset, CONVERT(r), ଆକାର(r));
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା tdb_methods io_methods = {
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
ଶୂନ୍ୟ tdb_io_init(ସଂରଚନା tdb_context *tdb)
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

ସଂରଚନା tdb_transaction_el {
	ସଂରଚନା tdb_transaction_el *next, *prev;
	tdb_off_t offset;
	tdb_len_t length;
	unsigned ଅକ୍ଷର *data;
};

/*
  hold the context of any current transaction
*/
ସଂରଚନା tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ଅଚଳ ସଂରଚନା tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ସଂରଚନା tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting;

	/* old file size before transaction */
	tdb_len_t old_map_size;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_read(ସଂରଚନା tdb_context *tdb, tdb_off_t off, ଶୂନ୍ୟ *buf,
			    tdb_len_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv)
{
	ସଂରଚନା tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	ପାଇଁ (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		ଯଦି (off+len <= el->offset) {
			ଜାରି;
		}
		ଯଦି (off >= el->offset + el->length) {
			ଜାରି;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		ଯଦି (off < el->offset) {
			partial = el->offset - off;
			ଯଦି (transaction_read(tdb, off, buf, partial, cv) != 0) {
				ଯାଅ fail;
			}
			len -= partial;
			off += partial;
			buf = (ଶୂନ୍ୟ *)(partial + (ଅକ୍ଷର *)buf);
		}
		ଯଦି (off + len <= el->offset + el->length) {
			partial = len;
		} ଅନ୍ୟଥା {
			partial = el->offset + el->length - off;
		}
		memcpy(buf, el->data + (off - el->offset), partial);
		ଯଦି (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (ଶୂନ୍ୟ *)(partial + (ଅକ୍ଷର *)buf);

		ଯଦି (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			ଯାଅ fail;
		}

		ଫେରନ୍ତୁ 0;
	}

	/* its not in the transaction elements - do a real read */
	ଫେରନ୍ତୁ tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ଫେରନ୍ତୁ -1;
}


/*
  write while in a transaction
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_write(ସଂରଚନା tdb_context *tdb, tdb_off_t off,
			     ଅଚଳ ଶୂନ୍ୟ *buf, tdb_len_t len)
{
	ସଂରଚନା tdb_transaction_el *el, *best_el=ଖାଲି;

	ଯଦି (len == 0) {
		ଫେରନ୍ତୁ 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	ଯଦି (len == ଆକାର(tdb_off_t) && off >= FREELIST_ଶୀର୍ଷ &&
	    off < FREELIST_ଶୀର୍ଷ+TDB_HASHTABLE_SIZE(tdb)) {
		u32 chain = (off-FREELIST_ଶୀର୍ଷ) / ଆକାର(tdb_off_t);
		memcpy(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	ପାଇଁ (el=tdb->transaction->elements_last;el;el=el->prev) {
		tdb_len_t partial;

		ଯଦି (best_el == ଖାଲି && off == el->offset+el->length) {
			best_el = el;
		}

		ଯଦି (off+len <= el->offset) {
			ଜାରି;
		}
		ଯଦି (off >= el->offset + el->length) {
			ଜାରି;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		ଯଦି (off < el->offset) {
			partial = el->offset - off;
			ଯଦି (transaction_write(tdb, off, buf, partial) != 0) {
				ଯାଅ fail;
			}
			len -= partial;
			off += partial;
			buf = (ଅଚଳ ଶୂନ୍ୟ *)(partial + (ଅଚଳ ଅକ୍ଷର *)buf);
		}
		ଯଦି (off + len <= el->offset + el->length) {
			partial = len;
		} ଅନ୍ୟଥା {
			partial = el->offset + el->length - off;
		}
		memcpy(el->data + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (ଅଚଳ ଶୂନ୍ୟ *)(partial + (ଅଚଳ ଅକ୍ଷର *)buf);

		ଯଦି (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			ଯାଅ fail;
		}

		ଫେରନ୍ତୁ 0;
	}

	/* see if we can append the new entry to an existing entry */
	ଯଦି (best_el && best_el->offset + best_el->length == off &&
	    (off+len < tdb->transaction->old_map_size ||
	     off > tdb->transaction->old_map_size)) {
		unsigned ଅକ୍ଷର *data = best_el->data;
		el = best_el;
		el->data = (unsigned ଅକ୍ଷର *)realloc(el->data,
						    el->length + len);
		ଯଦି (el->data == ଖାଲି) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->data = data;
			ଫେରନ୍ତୁ -1;
		}
		ଯଦି (buf) {
			memcpy(el->data + el->length, buf, len);
		} ଅନ୍ୟଥା {
			memset(el->data + el->length, TDB_PAD_BYTE, len);
		}
		el->length += len;
		ଫେରନ୍ତୁ 0;
	}

	/* add a new entry at the end of the list */
	el = (ସଂରଚନା tdb_transaction_el *)malloc(ଆକାର(*el));
	ଯଦି (el == ଖାଲି) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ଫେରନ୍ତୁ -1;
	}
	el->next = ଖାଲି;
	el->prev = tdb->transaction->elements_last;
	el->offset = off;
	el->length = len;
	el->data = (unsigned ଅକ୍ଷର *)malloc(len);
	ଯଦି (el->data == ଖାଲି) {
		ମୁକ୍ତ(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ଫେରନ୍ତୁ -1;
	}
	ଯଦି (buf) {
		memcpy(el->data, buf, len);
	} ଅନ୍ୟଥା {
		memset(el->data, TDB_PAD_BYTE, len);
	}
	ଯଦି (el->prev) {
		el->prev->next = el;
	} ଅନ୍ୟଥା {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	ଫେରନ୍ତୁ 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ଫେରନ୍ତୁ -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ସ୍ଥିର ଶୂନ୍ୟ transaction_next_hash_chain(ସଂରଚନା tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ପାଇଁ (;h < tdb->header.hash_size;h++) {
		/* the +1 takes account of the freelist */
		ଯଦି (0 != tdb->transaction->hash_heads[h+1]) {
			ଭାଙ୍ଗ;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_oob(ସଂରଚନା tdb_context *tdb, tdb_off_t len,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା probe EXT2FS_ATTR((unused)))
{
	ଯଦି (len <= tdb->map_size) {
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_expand_file(ସଂରଚନା tdb_context *tdb, tdb_off_t size,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	ଯଦି (transaction_write(tdb, size, ଖାଲି, addition) != 0) {
		ଫେରନ୍ତୁ -1;
	}

	ଫେରନ୍ତୁ 0;
}

/*
  brlock during a transaction - ignore them
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_brlock(ସଂରଚନା tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rw_type EXT2FS_ATTR((unused)),
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lck_type EXT2FS_ATTR((unused)),
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା probe EXT2FS_ATTR((unused)),
			      size_t len EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା tdb_methods transaction_methods = {
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_start(ସଂରଚନା tdb_context *tdb)
{
	/* some sanity checks */
	ଯଦି (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		ଫେରନ୍ତୁ -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	ଯଦି (tdb->transaction != ଖାଲି) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (tdb->ଅଙ୍କ_locks != 0 || tdb->global_lock.ଗଣନା_2) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->travlocks.next != ଖାଲି) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ଫେରନ୍ତୁ -1;
	}

	tdb->transaction = (ସଂରଚନା tdb_transaction *)
		calloc(ଆକାର(ସଂରଚନା tdb_transaction), 1);
	ଯଦି (tdb->transaction == ଖାଲି) {
		tdb->ecode = TDB_ERR_OOM;
		ଫେରନ୍ତୁ -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	ଯଦି (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		ଫେରନ୍ତୁ -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	ଯଦି (tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		ଯାଅ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		calloc(tdb->header.hash_size+1, ଆକାର(u32));
	ଯଦି (tdb->transaction->hash_heads == ଖାଲି) {
		tdb->ecode = TDB_ERR_OOM;
		ଯାଅ fail;
	}
	ଯଦି (tdb->methods->tdb_read(tdb, FREELIST_ଶୀର୍ଷ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_SIZE(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		ଯାଅ fail;
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
	ଯଦି (transaction_write(tdb, FREELIST_ଶୀର୍ଷ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_SIZE(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		ଯାଅ fail;
	}

	ଫେରନ୍ତୁ 0;

fail:
	tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	ଫେରନ୍ତୁ -1;
}


/*
  cancel the current transaction
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_cancel(ସଂରଚନା tdb_context *tdb)
{
	ଯଦି (tdb->transaction == ଖାଲି) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		ଫେରନ୍ତୁ 0;
	}

	tdb->map_size = tdb->transaction->old_map_size;

	/* free all the transaction elements */
	ଯେତେବେଳେ (tdb->transaction->elements) {
		ସଂରଚନା tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->next;
		ମୁକ୍ତ(el->data);
		ମୁକ୍ତ(el);
	}

	/* remove any global lock created during the transaction */
	ଯଦି (tdb->global_lock.ଗଣନା_2 != 0) {
		tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_size);
		tdb->global_lock.ଗଣନା_2 = 0;
	}

	/* remove any locks created during the transaction */
	ଯଦି (tdb->ଅଙ୍କ_locks != 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ପାଇଁ (i=0;i<tdb->ଅଙ୍କ_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_ଶୀର୍ଷ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->ଅଙ୍କ_locks = 0;
		tdb->ଅଙ୍କ_lockrecs = 0;
		SAFE_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_ଶୀର୍ଷ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);

	ଫେରନ୍ତୁ 0;
}

/*
  sync to disk
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_sync(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, tdb_len_t length)
{
	ଯଦି (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		ଫେରନ୍ତୁ -1;
	}
#ଯଦି defined(HAVE_MSYNC) && defined(MS_SYNC)
	ଯଦି (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_size-1);
		ଯଦି (msync(moffset + (ଅକ୍ଷର *)tdb->map_ptr,
			  length + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			ଫେରନ୍ତୁ -1;
		}
	}
#endif
	ଫେରନ୍ତୁ 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ସ୍ଥିର tdb_len_t tdb_recovery_size(ସଂରଚନା tdb_context *tdb)
{
	ସଂରଚନା tdb_transaction_el *el;
	tdb_len_t recovery_size = 0;

	recovery_size = ଆକାର(u32);
	ପାଇଁ (el=tdb->transaction->elements;el;el=el->next) {
		ଯଦି (el->offset >= tdb->transaction->old_map_size) {
			ଜାରି;
		}
		recovery_size += 2*ଆକାର(tdb_off_t) + el->length;
	}

	ଫେରନ୍ତୁ recovery_size;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_recovery_allocate(ସଂରଚନା tdb_context *tdb,
				 tdb_len_t *recovery_size,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_size)
{
	ସଂରଚନା list_struct rec;
	ଅଚଳ ସଂରଚନା tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_head;

	ଯଦି (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		ଫେରନ୍ତୁ -1;
	}

	rec.rec_len = 0;

	ଯଦି (recovery_head != 0 &&
	    methods->tdb_read(tdb, recovery_head, &rec, ଆକାର(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		ଫେରନ୍ତୁ -1;
	}

	*recovery_size = tdb_recovery_size(tdb);

	ଯଦି (recovery_head != 0 && *recovery_size <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_size = rec.rec_len;
		*recovery_offset = recovery_head;
		ଫେରନ୍ତୁ 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	ଯଦି (recovery_head != 0) {
		ଯଦି (tdb_free(tdb, recovery_head, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			ଫେରନ୍ତୁ -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_size = tdb_recovery_size(tdb);

	/* round up to a multiple of page size */
	*recovery_max_size = TDB_ALIGN(ଆକାର(rec) + *recovery_size, tdb->page_size) - ଆକାର(rec);
	*recovery_offset = tdb->map_size;
	recovery_head = *recovery_offset;

	ଯଦି (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
				     (tdb->map_size - tdb->transaction->old_map_size) +
				     ଆକାର(rec) + *recovery_max_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		ଫେରନ୍ତୁ -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_size + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_size = tdb->map_size;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_head);
	ଯଦି (methods->tdb_write(tdb, TDB_RECOVERY_HEAD,
			       &recovery_head, ଆକାର(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		ଫେରନ୍ତୁ -1;
	}

	ଫେରନ୍ତୁ 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା transaction_setup_recovery(ସଂରଚନା tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	ସଂରଚନା tdb_transaction_el *el;
	tdb_len_t recovery_size;
	unsigned ଅକ୍ଷର *data, *p;
	ଅଚଳ ସଂରଚନା tdb_methods *methods = tdb->transaction->io_methods;
	ସଂରଚନା list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_size;
	tdb_off_t old_map_size = tdb->transaction->old_map_size;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	ଯଦି (tdb_recovery_allocate(tdb, &recovery_size,
				  &recovery_offset, &recovery_max_size) == -1) {
		ଫେରନ୍ତୁ -1;
	}

	data = (unsigned ଅକ୍ଷର *)malloc(recovery_size + ଆକାର(*rec));
	ଯଦି (data == ଖାଲି) {
		tdb->ecode = TDB_ERR_OOM;
		ଫେରନ୍ତୁ -1;
	}

	rec = (ସଂରଚନା list_struct *)data;
	memset(rec, 0, ଆକାର(*rec));

	rec->magic    = 0;
	rec->data_len = recovery_size;
	rec->rec_len  = recovery_max_size;
	rec->ଚାବି_len  = old_map_size;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = data + ଆକାର(*rec);
	ପାଇଁ (el=tdb->transaction->elements;el;el=el->next) {
		ଯଦି (el->offset >= old_map_size) {
			ଜାରି;
		}
		ଯଦି (el->offset + el->length > tdb->transaction->old_map_size) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			ମୁକ୍ତ(data);
			tdb->ecode = TDB_ERR_CORRUPT;
			ଫେରନ୍ତୁ -1;
		}
		memcpy(p, &el->offset, 4);
		memcpy(p+4, &el->length, 4);
		ଯଦି (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		ଯଦି (methods->tdb_read(tdb, el->offset, p + 8, el->length, 0) != 0) {
			ମୁକ୍ତ(data);
			tdb->ecode = TDB_ERR_IO;
			ଫେରନ୍ତୁ -1;
		}
		p += 8 + el->length;
	}

	/* and the tailer */
	tailer = ଆକାର(*rec) + recovery_max_size;
	memcpy(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	ଯଦି (methods->tdb_write(tdb, recovery_offset, data, ଆକାର(*rec) + recovery_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		ମୁକ୍ତ(data);
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	ଯଦି (transaction_sync(tdb, recovery_offset, ଆକାର(*rec) + recovery_size) == -1) {
		ମୁକ୍ତ(data);
		ଫେରନ୍ତୁ -1;
	}

	ମୁକ୍ତ(data);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + offsetof(ସଂରଚନା list_struct, magic);

	ଯଦି (methods->tdb_write(tdb, *magic_offset, &magic, ଆକାର(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	/* ensure the recovery magic marker is on disk */
	ଯଦି (transaction_sync(tdb, *magic_offset, ଆକାର(magic)) == -1) {
		ଫେରନ୍ତୁ -1;
	}

	ଫେରନ୍ତୁ 0;
}

/*
  commit the current transaction
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_commit(ସଂରଚନା tdb_context *tdb)
{
	ଅଚଳ ସଂରଚନା tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	ଯଦି (tdb->transaction == ଖାଲି) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		ଫେରନ୍ତୁ 0;
	}

	/* check for a null transaction */
	ଯଦି (tdb->transaction->elements == ଖାଲି) {
		tdb_transaction_cancel(tdb);
		ଫେରନ୍ତୁ 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	ଯଦି (tdb->ଅଙ୍କ_locks || tdb->global_lock.ଗଣନା_2) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		ଫେରନ୍ତୁ -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	ଯଦି (tdb_brlock_upgrade(tdb, FREELIST_ଶୀର୍ଷ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ଫେରନ୍ତୁ -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	ଯଦି (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		ଯଦି (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ଫେରନ୍ତୁ -1;
		}
	}

	/* expand the file to the new size if needed */
	ଯଦି (tdb->map_size != tdb->transaction->old_map_size) {
		ଯଦି (methods->tdb_expand_file(tdb, tdb->transaction->old_map_size,
					     tdb->map_size -
					     tdb->transaction->old_map_size) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ଫେରନ୍ତୁ -1;
		}
		tdb->map_size = tdb->transaction->old_map_size;
		methods->tdb_oob(tdb, tdb->map_size + 1, 1);
	}

	/* perform all the writes */
	ଯେତେବେଳେ (tdb->transaction->elements) {
		ସଂରଚନା tdb_transaction_el *el = tdb->transaction->elements;

		ଯଦି (methods->tdb_write(tdb, el->offset, el->data, el->length) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			ଫେରନ୍ତୁ -1;
		}
		tdb->transaction->elements = el->next;
		ମୁକ୍ତ(el->data);
		ମୁକ୍ତ(el);
	}

	ଯଦି (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		ଯଦି (transaction_sync(tdb, 0, tdb->map_size) == -1) {
			ଫେରନ୍ତୁ -1;
		}

		/* remove the recovery marker */
		ଯଦି (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			ଫେରନ୍ତୁ -1;
		}

		/* ensure the recovery marker has been removed on disk */
		ଯଦି (transaction_sync(tdb, magic_offset, 4) == -1) {
			ଫେରନ୍ତୁ -1;
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
	utime(tdb->name, ଖାଲି);
#endif

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	ଫେରନ୍ତୁ 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_transaction_recover(ସଂରଚନା tdb_context *tdb)
{
	tdb_off_t recovery_head, recovery_eof;
	unsigned ଅକ୍ଷର *data, *p;
	u32 zero = 0;
	ସଂରଚନା list_struct rec;

	/* find the recovery area */
	ଯଦି (tdb_ofs_read(tdb, TDB_RECOVERY_HEAD, &recovery_head) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (recovery_head == 0) {
		/* we have never allocated a recovery record */
		ଫେରନ୍ତୁ 0;
	}

	/* read the recovery record */
	ଯଦି (tdb->methods->tdb_read(tdb, recovery_head, &rec,
				   ଆକାର(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		ଫେରନ୍ତୁ -1;
	}

	recovery_eof = rec.ଚାବି_len;

	data = (unsigned ଅକ୍ଷର *)malloc(rec.data_len);
	ଯଦି (data == ଖାଲି) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		ଫେରନ୍ତୁ -1;
	}

	/* read the full recovery data */
	ଯଦି (tdb->methods->tdb_read(tdb, recovery_head + ଆକାର(rec), data,
				   rec.data_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		ମୁକ୍ତ(data);
		ଫେରନ୍ତୁ -1;
	}

	/* recover the file data */
	p = data;
	ଯେତେବେଳେ (p+8 < data + rec.data_len) {
		u32 ofs, len;
		ଯଦି (DOCONV()) {
			tdb_convert(p, 8);
		}
		memcpy(&ofs, p, 4);
		memcpy(&len, p+4, 4);

		ଯଦି (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			ମୁକ୍ତ(data);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			ଫେରନ୍ତୁ -1;
		}
		p += 8 + len;
	}

	ମୁକ୍ତ(data);

	ଯଦି (transaction_sync(tdb, 0, tdb->map_size) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	ଯଦି (recovery_eof <= recovery_head) {
		ଯଦି (tdb_ofs_write(tdb, TDB_RECOVERY_HEAD, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			ଫେରନ୍ତୁ -1;
		}
	}

	/* remove the recovery magic */
	ଯଦି (tdb_ofs_write(tdb, recovery_head + offsetof(ସଂରଚନା list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	ଯଦି (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}
	tdb->map_size = recovery_eof;
	tdb_mmap(tdb);

	ଯଦି (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ଫେରନ୍ତୁ -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	ଫେରନ୍ତୁ 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_rec_free_read(ସଂରଚନା tdb_context *tdb, tdb_off_t off, ସଂରଚନା list_struct *rec)
{
	ଯଦି (tdb->methods->tdb_read(tdb, off, rec, ଆକାର(*rec),DOCONV()) == -1)
		ଫେରନ୍ତୁ -1;

	ଯଦି (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		ଯଦି (tdb->methods->tdb_write(tdb, off, rec, ଆକାର(*rec)) == -1)
			ଫେରନ୍ତୁ -1;
	}

	ଯଦି (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ଯଦି (tdb->methods->tdb_oob(tdb, rec->next+ଆକାର(*rec), 0) != 0)
		ଫେରନ୍ତୁ -1;
	ଫେରନ୍ତୁ 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_from_freelist(ସଂରଚନା tdb_context *tdb, tdb_off_t off, tdb_off_t next)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_ଶୀର୍ଷ;
	ଯେତେବେଳେ (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		ଯଦି (i == off) {
			/* We've found it! */
			ଫେରନ୍ତୁ tdb_ofs_write(tdb, last_ptr, &next);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_tailer(ସଂରଚନା tdb_context *tdb, tdb_off_t offset,
			 ଅଚଳ ସଂରଚନା list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = ଆକାର(*rec) + rec->rec_len;
	ଫେରନ୍ତୁ tdb_ofs_write(tdb, offset + totalsize - ଆକାର(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_free(ସଂରଚନା tdb_context *tdb, tdb_off_t offset, ସଂରଚନା list_struct *rec)
{
	tdb_off_t ଡାହାଣ, ବାମ;

	/* Allocation and tailer lock */
	ଯଦି (tdb_lock(tdb, -1, F_WRLCK) != 0)
		ଫେରନ୍ତୁ -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	ଯଦି (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		ଯାଅ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	ଡାହାଣ = offset + ଆକାର(*rec) + rec->rec_len;
	ଯଦି (ଡାହାଣ + ଆକାର(*rec) <= tdb->map_size) {
		ସଂରଚନା list_struct r;

		ଯଦି (tdb->methods->tdb_read(tdb, ଡାହାଣ, &r, ଆକାର(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", ଡାହାଣ));
			ଯାଅ ବାମ;
		}

		/* If it's free, expand to include it. */
		ଯଦି (r.magic == TDB_FREE_MAGIC) {
			ଯଦି (remove_from_freelist(tdb, ଡାହାଣ, r.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", ଡାହାଣ));
				ଯାଅ ବାମ;
			}
			rec->rec_len += ଆକାର(r) + r.rec_len;
		}
	}

ବାମ:
	/* Look left */
	ବାମ = offset - ଆକାର(tdb_off_t);
	ଯଦି (ବାମ > TDB_DATA_START(tdb->header.hash_size)) {
		ସଂରଚନା list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		ଯଦି (tdb_ofs_read(tdb, ବାମ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ବାମ));
			ଯାଅ update;
		}

		/* it could be uninitialised data */
		ଯଦି (leftsize == 0 || leftsize == TDB_PAD_U32) {
			ଯାଅ update;
		}

		ବାମ = offset - leftsize;

		/* Now read in record */
		ଯଦି (tdb->methods->tdb_read(tdb, ବାମ, &l, ଆକାର(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ବାମ, leftsize));
			ଯାଅ update;
		}

		/* If it's free, expand to include it. */
		ଯଦି (l.magic == TDB_FREE_MAGIC) {
			ଯଦି (remove_from_freelist(tdb, ବାମ, l.next) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ବାମ));
				ଯାଅ update;
			} ଅନ୍ୟଥା {
				offset = ବାମ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	ଯଦି (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		ଯାଅ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	ଯଦି (tdb_ofs_read(tdb, FREELIST_ଶୀର୍ଷ, &rec->next) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_ଶୀର୍ଷ, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		ଯାଅ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	ଫେରନ୍ତୁ 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ଫେରନ୍ତୁ -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ସ୍ଥିର tdb_off_t tdb_allocate_ofs(ସଂରଚନା tdb_context *tdb, tdb_len_t length, tdb_off_t rec_ptr,
				ସଂରଚନା list_struct *rec, tdb_off_t last_ptr)
{
	ସଂରଚନା list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', ଆକାର(newrec));

	/* found it - now possibly split it up  */
	ଯଦି (rec->rec_len > length + MIN_REC_SIZE) {
		/* Length of left piece */
		length = TDB_ALIGN(length, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (ଆକାର(*rec) + length);
		newrec_ptr = rec_ptr + ଆକାର(*rec) + length;

		/* And left record is shortened */
		rec->rec_len = length;
	} ଅନ୍ୟଥା {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	ଯଦି (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1) {
		ଫେରନ୍ତୁ 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	ଯଦି (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		ଫେରନ୍ତୁ 0;
	}

	/* Did we create new block? */
	ଯଦି (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		ଯଦି (update_tailer(tdb, rec_ptr, rec) == -1) {
			ଫେରନ୍ତୁ 0;
		}

		/* Free new record */
		ଯଦି (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			ଫେରନ୍ତୁ 0;
		}
	}

	/* all done - return the new record offset */
	ଫେରନ୍ତୁ rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ସଂରଚନା tdb_context *tdb, tdb_len_t length, ସଂରଚନା list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	ସଂରଚନା {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	ଯଦି (tdb_lock(tdb, -1, F_WRLCK) == -1)
		ଫେରନ୍ତୁ 0;

	/* Extra bytes required for tailer */
	length += ଆକାର(tdb_off_t);

 again:
	last_ptr = FREELIST_ଶୀର୍ଷ;

	/* read in the freelist top */
	ଯଦି (tdb_ofs_read(tdb, FREELIST_ଶୀର୍ଷ, &rec_ptr) == -1)
		ଯାଅ fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	ଯେତେବେଳେ (rec_ptr) {
		ଯଦି (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			ଯାଅ fail;
		}

		ଯଦି (rec->rec_len >= length) {
			ଯଦି (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				ଯଦି (bestfit.rec_len < 2*length) {
					ଭାଙ୍ଗ;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->next;
	}

	ଯଦି (bestfit.rec_ptr != 0) {
		ଯଦି (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			ଯାଅ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, length, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		ଫେରନ୍ତୁ newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	ଯଦି (tdb_expand(tdb, length + ଆକାର(*rec)) == 0)
		ଯାଅ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ଫେରନ୍ତୁ 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_ଭର୍ତ୍ତି(ସଂରଚନା tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_DATA ଚାବି, data;

	memset(&data, '\0', ଆକାର(data));
	ଚାବି.dptr = (unsigned ଅକ୍ଷର *)&rec_ptr;
	ଚାବି.dsize = ଆକାର(rec_ptr);
	ଫେରନ୍ତୁ tdb_store(mem_tdb, ଚାବି, data, TDB_ଭର୍ତ୍ତି);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_validate_freelist(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pnum_entries)
{
	ସଂରଚନା tdb_context *mem_tdb = ଖାଲି;
	ସଂରଚନା list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_size,
				TDB_INTERNAL, O_RDWR, 0600);
	ଯଦି (!mem_tdb) {
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		ଫେରନ୍ତୁ 0;
	}

	last_ptr = FREELIST_ଶୀର୍ଷ;

	/* Store the FREELIST_TOP record. */
	ଯଦି (seen_ଭର୍ତ୍ତି(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		ଯାଅ fail;
	}

	/* read in the freelist top */
	ଯଦି (tdb_ofs_read(tdb, FREELIST_ଶୀର୍ଷ, &rec_ptr) == -1) {
		ଯାଅ fail;
	}

	ଯେତେବେଳେ (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		ଯଦି (seen_ଭର୍ତ୍ତି(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			ଯାଅ fail;
		}

		ଯଦି (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			ଯାଅ fail;
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
	ଫେରନ୍ତୁ ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_next_lock(ସଂରଚନା tdb_context *tdb, ସଂରଚନା tdb_traverse_lock *tlock,
			 ସଂରଚନା list_struct *rec)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା want_next = (tlock->off != 0);

	/* Lock each chain from the start one. */
	ପାଇଁ (; tlock->hash < tdb->header.hash_size; tlock->hash++) {
		ଯଦି (!tlock->off && tlock->hash != 0) {
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
			ଯଦି (tlock->hash == tdb->header.hash_size) {
				ଜାରି;
			}
		}

		ଯଦି (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			ଫେରନ୍ତୁ -1;

		/* No previous record?  Start at top of chain. */
		ଯଦି (!tlock->off) {
			ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(tlock->hash),
				     &tlock->off) == -1)
				ଯାଅ fail;
		} ଅନ୍ୟଥା {
			/* Otherwise unlock the previous record. */
			ଯଦି (tdb_unlock_record(tdb, tlock->off) != 0)
				ଯାଅ fail;
		}

		ଯଦି (want_next) {
			/* We have offset of old record: grab next */
			ଯଦି (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ଯାଅ fail;
			tlock->off = rec->next;
		}

		/* Iterate through chain */
		ଯେତେବେଳେ( tlock->off) {
			tdb_off_t current;
			ଯଦି (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ଯାଅ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			ଯଦି (tlock->off == rec->next) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				ଯାଅ fail;
			}

			ଯଦି (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				ଯଦି (tdb_lock_record(tdb, tlock->off) != 0)
					ଯାଅ fail;
				ଫେରନ୍ତୁ tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->next;
			ଯଦି (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				ଯାଅ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_next = 0;
	}
	/* We finished iteration without finding anything */
	ଫେରନ୍ତୁ TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	ଯଦି (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	ଫେରନ୍ତୁ -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_traverse_internal(ସଂରଚନା tdb_context *tdb,
				 tdb_traverse_func fn, ଶୂନ୍ୟ *private_data,
				 ସଂରଚନା tdb_traverse_lock *tl)
{
	TDB_DATA ଚାବି, dbuf;
	ସଂରଚନା list_struct rec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, ଗଣନା_2 = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->next = tdb->travlocks.next;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.next = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	ଯେତେବେଳେ ((ret = tdb_next_lock(tdb, tl, &rec)) > 0) {
		ଗଣନା_2++;
		/* now read the full record */
		ଚାବି.dptr = tdb_alloc_read(tdb, tl->off + ଆକାର(rec),
					  rec.ଚାବି_len + rec.data_len);
		ଯଦି (!ଚାବି.dptr) {
			ret = -1;
			ଯଦି (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				ଯାଅ out;
			ଯଦି (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			ଯାଅ out;
		}
		ଚାବି.dsize = rec.ଚାବି_len;
		dbuf.dptr = ଚାବି.dptr + rec.ଚାବି_len;
		dbuf.dsize = rec.data_len;

		/* Drop chain lock, call out */
		ଯଦି (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(ଚାବି.dptr);
			ଯାଅ out;
		}
		ଯଦି (fn && fn(tdb, ଚାବି, dbuf, private_data)) {
			/* They want us to terminate traversal */
			ret = ଗଣନା_2;
			ଯଦି (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(ଚାବି.dptr);
			ଯାଅ out;
		}
		SAFE_FREE(ଚାବି.dptr);
	}
out:
	tdb->travlocks.next = tl->next;
	ଯଦି (ret < 0)
		ଫେରନ୍ତୁ -1;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ ଗଣନା_2;
}


/*
  a write style traverse - temporarily marks the db read only
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_traverse_read(ସଂରଚନା tdb_context *tdb,
		      tdb_traverse_func fn, ଶୂନ୍ୟ *private_data)
{
	ସଂରଚନା tdb_traverse_lock tl = { ଖାଲି, 0, 0, F_RDLCK };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	ଯଦି (tdb_transaction_lock(tdb, F_RDLCK)) {
		ଫେରନ୍ତୁ -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	ଫେରନ୍ତୁ ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_traverse(ସଂରଚନା tdb_context *tdb,
		 tdb_traverse_func fn, ଶୂନ୍ୟ *private_data)
{
	ସଂରଚନା tdb_traverse_lock tl = { ଖାଲି, 0, 0, F_WRLCK };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (tdb->read_only || tdb->traverse_read) {
		ଫେରନ୍ତୁ tdb_traverse_read(tdb, fn, private_data);
	}

	ଯଦି (tdb_transaction_lock(tdb, F_WRLCK)) {
		ଫେରନ୍ତୁ -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_data, &tl);

	tdb_transaction_unlock(tdb);

	ଫେରନ୍ତୁ ret;
}


/* find the first entry in the database and return its key */
TDB_DATA tdb_firstkey(ସଂରଚନା tdb_context *tdb)
{
	TDB_DATA ଚାବି;
	ସଂରଚନା list_struct rec;

	/* release any old lock */
	ଯଦି (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		ଫେରନ୍ତୁ tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	ଯଦି (tdb_next_lock(tdb, &tdb->travlocks, &rec) <= 0)
		ଫେରନ୍ତୁ tdb_null;
	/* now read the key */
	ଚାବି.dsize = rec.ଚାବି_len;
	ଚାବି.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+ଆକାର(rec),ଚାବି.dsize);

	/* Unlock the hash chain of the record we just read. */
	ଯଦି (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	ଫେରନ୍ତୁ ଚାବି;
}

/* find the next entry in the database, returning its key */
TDB_DATA tdb_nextkey(ସଂରଚନା tdb_context *tdb, TDB_DATA oldkey)
{
	u32 oldhash;
	TDB_DATA ଚାବି = tdb_null;
	ସଂରଚନା list_struct rec;
	unsigned ଅକ୍ଷର *k = ଖାଲି;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	ଯଦି (tdb->travlocks.off) {
		ଯଦି (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			ଫେରନ୍ତୁ tdb_null;
		ଯଦି (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+ଆକାର(rec),
					    rec.ଚାବି_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			ଯଦି (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				ଫେରନ୍ତୁ tdb_null;
			}
			ଯଦି (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				ଫେରନ୍ତୁ tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	ଯଦି (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		ଯଦି (!tdb->travlocks.off)
			ଫେରନ୍ତୁ tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		ଯଦି (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			ଫେରନ୍ତୁ tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	ଯଦି (tdb_next_lock(tdb, &tdb->travlocks, &rec) > 0) {
		ଚାବି.dsize = rec.ଚାବି_len;
		ଚାବି.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+ଆକାର(rec),
					  ଚାବି.dsize);
		/* Unlock the chain of this new record */
		ଯଦି (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	ଯଦି (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	ଫେରନ୍ତୁ ଚାବି;
}

/* file: dump.c */

ସ୍ଥିର tdb_off_t tdb_dump_record(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash,
				 tdb_off_t offset)
{
	ସଂରଚନା list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	ଯଦି (tdb->methods->tdb_read(tdb, offset, (ଅକ୍ଷର *)&rec,
				   ଆକାର(rec), DOCONV()) == -1) {
		printf("ERROR: failed to read record at %u\n", offset);
		ଫେରନ୍ତୁ 0;
	}

	printf(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.next, rec.rec_len, rec.ଚାବି_len, rec.data_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + ଆକାର(rec) + rec.rec_len - ଆକାର(tdb_off_t);

	ଯଦି (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		printf("ERROR: failed to read tailer at %u\n", tailer_ofs);
		ଫେରନ୍ତୁ rec.next;
	}

	ଯଦି (tailer != rec.rec_len + ଆକାର(rec)) {
		printf("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tailer, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(rec.rec_len + ଆକାର(rec)));
	}
	ଫେରନ୍ତୁ rec.next;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_dump_chain(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
	tdb_off_t rec_ptr, ଶୀର୍ଷ;

	ଶୀର୍ଷ = TDB_HASH_ଶୀର୍ଷ(i);

	ଯଦି (tdb_lock(tdb, i, F_WRLCK) != 0)
		ଫେରନ୍ତୁ -1;

	ଯଦି (tdb_ofs_read(tdb, ଶୀର୍ଷ, &rec_ptr) == -1)
		ଫେରନ୍ତୁ tdb_unlock(tdb, i, F_WRLCK);

	ଯଦି (rec_ptr)
		printf("hash=%d\n", i);

	ଯେତେବେଳେ (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	ଫେରନ୍ତୁ tdb_unlock(tdb, i, F_WRLCK);
}

ଶୂନ୍ୟ tdb_dump_all(ସଂରଚନା tdb_context *tdb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tdb->header.hash_size; i++) {
		tdb_dump_chain(tdb, i);
	}
	printf("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_printfreelist(ସଂରଚନା tdb_context *tdb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	long total_free = 0;
	tdb_off_t offset, rec_ptr;
	ସଂରଚନା list_struct rec;

	ଯଦି ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		ଫେରନ୍ତୁ ret;

	offset = FREELIST_ଶୀର୍ଷ;

	/* read in the freelist top */
	ଯଦି (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ଫେରନ୍ତୁ 0;
	}

	printf("freelist top=[0x%08x]\n", rec_ptr );
	ଯେତେବେଳେ (rec_ptr) {
		ଯଦି (tdb->methods->tdb_read(tdb, rec_ptr, (ଅକ୍ଷର *)&rec,
					   ଆକାର(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			ଫେରନ୍ତୁ -1;
		}

		ଯଦି (rec.magic != TDB_FREE_MAGIC) {
			printf("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			ଫେରନ୍ତୁ -1;
		}

		printf("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		total_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.next;
	}
	printf("total rec_len = [0x%08x (%d)]\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total_free,
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total_free);

	ଫେରନ୍ତୁ tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ଶୂନ୍ୟ tdb_increment_seqnum_nonblock(ସଂରଚନା tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ଯଦି (!(tdb->flags & TDB_SEQNUM)) {
		ଫେରନ୍ତୁ;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	ଯଦି (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ଫେରନ୍ତୁ;
	seqnum++;
	(ଶୂନ୍ୟ) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ସ୍ଥିର ଶୂନ୍ୟ tdb_increment_seqnum(ସଂରଚନା tdb_context *tdb)
{
	ଯଦି (!(tdb->flags & TDB_SEQNUM)) {
		ଫେରନ୍ତୁ;
	}

	ଯଦି (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		ଫେରନ୍ତୁ;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ଚାବି_compare(TDB_DATA ଚାବି, TDB_DATA data,
			   ଶୂନ୍ୟ *private_data EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ memcmp(data.dptr, ଚାବି.dptr, data.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ସ୍ଥିର tdb_off_t tdb_find(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, u32 hash,
			ସଂରଚନା list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(hash), &rec_ptr) == -1)
		ଫେରନ୍ତୁ 0;

	/* keep looking until we find the right record */
	ଯେତେବେଳେ (rec_ptr) {
		ଯଦି (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ଫେରନ୍ତୁ 0;

		ଯଦି (!TDB_DEAD(r) && hash==r->full_hash
		    && ଚାବି.dsize==r->ଚାବି_len
		    && tdb_parse_data(tdb, ଚାବି, rec_ptr + ଆକାର(*r),
				      r->ଚାବି_len, tdb_ଚାବି_compare,
				      ଖାଲି) == 0) {
			ଫେରନ୍ତୁ rec_ptr;
		}
		rec_ptr = r->next;
	}
	ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, u32 hash, ପୂର୍ଣ୍ଣସଂଖ୍ୟା locktype,
			   ସଂରଚନା list_struct *rec)
{
	u32 rec_ptr;

	ଯଦି (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		ଫେରନ୍ତୁ 0;
	ଯଦି (!(rec_ptr = tdb_find(tdb, ଚାବି, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	ଫେରନ୍ତୁ rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_update_hash(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, u32 hash, TDB_DATA dbuf)
{
	ସଂରଚନା list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	ଯଦି (!(rec_ptr = tdb_find(tdb, ଚାବି, hash, &rec)))
		ଫେରନ୍ତୁ -1;

	/* must be long enough key, data and tailer */
	ଯଦି (rec.rec_len < ଚାବି.dsize + dbuf.dsize + ଆକାର(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (tdb->methods->tdb_write(tdb, rec_ptr + ଆକାର(rec) + rec.ଚାବି_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		ଫେରନ୍ତୁ -1;

	ଯଦି (dbuf.dsize != rec.data_len) {
		/* update size */
		rec.data_len = dbuf.dsize;
		ଫେରନ୍ତୁ tdb_rec_write(tdb, rec_ptr, &rec);
	}

	ଫେରନ୍ତୁ 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_DATA tdb_fetch(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	tdb_off_t rec_ptr;
	ସଂରଚନା list_struct rec;
	TDB_DATA ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ଚାବି);
	ଯଦି (!(rec_ptr = tdb_find_lock_hash(tdb,ଚାବି,hash,F_RDLCK,&rec)))
		ଫେରନ୍ତୁ tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + ଆକାର(rec) + rec.ଚାବି_len,
				  rec.data_len);
	ret.dsize = rec.data_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ଫେରନ୍ତୁ ret;
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_parse_record(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*parser)(TDB_DATA ଚାବି, TDB_DATA data,
				   ଶୂନ୍ୟ *private_data),
		     ଶୂନ୍ୟ *private_data)
{
	tdb_off_t rec_ptr;
	ସଂରଚନା list_struct rec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ଚାବି);

	ଯଦି (!(rec_ptr = tdb_find_lock_hash(tdb,ଚାବି,hash,F_RDLCK,&rec))) {
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_data(tdb, ଚାବି, rec_ptr + ଆକାର(rec) + rec.ଚାବି_len,
			     rec.data_len, parser, private_data);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	ଫେରନ୍ତୁ ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_exists_hash(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, u32 hash)
{
	ସଂରଚନା list_struct rec;

	ଯଦି (tdb_find_lock_hash(tdb, ଚାବି, hash, F_RDLCK, &rec) == 0)
		ଫେରନ୍ତୁ 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_exists(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	u32 hash = tdb->hash_fn(&ଚାବି);
	ଫେରନ୍ତୁ tdb_exists_hash(tdb, ଚାବି, hash);
}

/* actually delete an entry in the database given the offset */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_do_delete(ସଂରଚନା tdb_context *tdb, tdb_off_t rec_ptr, ସଂରଚନା list_struct*rec)
{
	tdb_off_t last_ptr, i;
	ସଂରଚନା list_struct lastrec;

	ଯଦି (tdb->read_only || tdb->traverse_read) ଫେରନ୍ତୁ -1;

	ଯଦି (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		ଫେରନ୍ତୁ tdb_rec_write(tdb, rec_ptr, rec);
	}
	ଯଦି (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		ଫେରନ୍ତୁ -1;

	/* find previous record in hash chain */
	ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(rec->full_hash), &i) == -1)
		ଫେରନ୍ତୁ -1;
	ପାଇଁ (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.next)
		ଯଦି (tdb_rec_read(tdb, i, &lastrec) == -1)
			ଫେରନ୍ତୁ -1;

	/* unlink it: next ptr is at start of record. */
	ଯଦି (last_ptr == 0)
		last_ptr = TDB_HASH_ଶୀର୍ଷ(rec->full_hash);
	ଯଦି (tdb_ofs_write(tdb, last_ptr, &rec->next) == -1)
		ଫେରନ୍ତୁ -1;

	/* recover the space */
	ଯଦି (tdb_free(tdb, rec_ptr, rec) == -1)
		ଫେରନ୍ତୁ -1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_ଗଣନା_dead(ସଂରଚନା tdb_context *tdb, u32 hash)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	tdb_off_t rec_ptr;
	ସଂରଚନା list_struct rec;

	/* read in the hash top */
	ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(hash), &rec_ptr) == -1)
		ଫେରନ୍ତୁ 0;

	ଯେତେବେଳେ (rec_ptr) {
		ଯଦି (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			ଫେରନ୍ତୁ 0;

		ଯଦି (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.next;
	}
	ଫେରନ୍ତୁ res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_purge_dead(ସଂରଚନା tdb_context *tdb, u32 hash)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = -1;
	ସଂରଚନା list_struct rec;
	tdb_off_t rec_ptr;

	ଯଦି (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		ଫେରନ୍ତୁ -1;
	}

	/* read in the hash top */
	ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(hash), &rec_ptr) == -1)
		ଯାଅ fail;

	ଯେତେବେଳେ (rec_ptr) {
		tdb_off_t next;

		ଯଦି (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			ଯାଅ fail;
		}

		next = rec.next;

		ଯଦି (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			ଯାଅ fail;
		}
		rec_ptr = next;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ଫେରନ୍ତୁ res;
}

/* delete an entry in the database given a key */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_delete_hash(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, u32 hash)
{
	tdb_off_t rec_ptr;
	ସଂରଚନା list_struct rec;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		ଯଦି (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			ଫେରନ୍ତୁ -1;

		ଯଦି (tdb_ଗଣନା_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		ଯଦି (!(rec_ptr = tdb_find(tdb, ଚାବି, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			ଫେରନ୍ତୁ -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	ଅନ୍ୟଥା {
		ଯଦି (!(rec_ptr = tdb_find_lock_hash(tdb, ଚାବି, hash, F_WRLCK,
						   &rec)))
			ଫେରନ୍ତୁ -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	ଯଦି (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ଯଦି (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_delete(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି)
{
	u32 hash = tdb->hash_fn(&ଚାବି);
	ଫେରନ୍ତୁ tdb_delete_hash(tdb, ଚାବି, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ସ୍ଥିର tdb_off_t tdb_find_dead(ସଂରଚନା tdb_context *tdb, u32 hash,
			       ସଂରଚନା list_struct *r, tdb_len_t length)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(hash), &rec_ptr) == -1)
		ଫେରନ୍ତୁ 0;

	/* keep looking until we find the right record */
	ଯେତେବେଳେ (rec_ptr) {
		ଯଦି (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ଫେରନ୍ତୁ 0;

		ଯଦି (TDB_DEAD(r) && r->rec_len >= length) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			ଫେରନ୍ତୁ rec_ptr;
		}
		rec_ptr = r->next;
	}
	ଫେରନ୍ତୁ 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_store(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag)
{
	ସଂରଚନା list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	ଅକ୍ଷର *p = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	ଯଦି (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ଫେରନ୍ତୁ -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ଚାବି);
	ଯଦି (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ଫେରନ୍ତୁ -1;

	/* check for it existing, on insert. */
	ଯଦି (flag == TDB_ଭର୍ତ୍ତି) {
		ଯଦି (tdb_exists_hash(tdb, ଚାବି, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			ଯାଅ fail;
		}
	} ଅନ୍ୟଥା {
		/* first try in-place update, on modify or replace. */
		ଯଦି (tdb_update_hash(tdb, ଚାବି, hash, dbuf) == 0) {
			ଯାଅ done;
		}
		ଯଦି (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			ଯାଅ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	ଯଦି (flag != TDB_ଭର୍ତ୍ତି)
		tdb_delete_hash(tdb, ଚାବି, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	ଯଦି (!(p = (ଅକ୍ଷର *)malloc(ଚାବି.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		ଯାଅ fail;
	}

	memcpy(p, ଚାବି.dptr, ଚାବି.dsize);
	ଯଦି (dbuf.dsize)
		memcpy(p+ଚାବି.dsize, dbuf.dptr, dbuf.dsize);

	ଯଦି (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			ଚାବି.dsize + dbuf.dsize + ଆକାର(tdb_off_t));

		ଯଦି (rec_ptr != 0) {
			rec.ଚାବି_len = ଚାବି.dsize;
			rec.data_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			ଯଦି (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + ଆକାର(rec),
				    p, ଚାବି.dsize + dbuf.dsize) == -1) {
				ଯାଅ fail;
			}
			ଯାଅ done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	ଯଦି (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		ଯାଅ fail;
	}

	ଯଦି ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ଯାଅ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, ଚାବି.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	ଯଦି (rec_ptr == 0) {
		ଯାଅ fail;
	}

	/* Read hash top into next ptr */
	ଯଦି (tdb_ofs_read(tdb, TDB_HASH_ଶୀର୍ଷ(hash), &rec.next) == -1)
		ଯାଅ fail;

	rec.ଚାବି_len = ଚାବି.dsize;
	rec.data_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	ଯଦି (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+ଆକାର(rec), p, ଚାବି.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_ଶୀର୍ଷ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		ଯାଅ fail;
	}

 done:
	ret = 0;
 fail:
	ଯଦି (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	ଫେରନ୍ତୁ ret;
}


/* Append to an entry. Create if not exist. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_append(ସଂରଚନା tdb_context *tdb, TDB_DATA ଚାବି, TDB_DATA new_dbuf)
{
	u32 hash;
	TDB_DATA dbuf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ଚାବି);
	ଯଦି (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		ଫେରନ୍ତୁ -1;

	dbuf = tdb_fetch(tdb, ଚାବି);

	ଯଦି (dbuf.dptr == ଖାଲି) {
		dbuf.dptr = (unsigned ଅକ୍ଷର *)malloc(new_dbuf.dsize);
	} ଅନ୍ୟଥା {
		unsigned ଅକ୍ଷର *new_dptr = (unsigned ଅକ୍ଷର *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		ଯଦି (new_dptr == ଖାଲି) {
			ମୁକ୍ତ(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	ଯଦି (dbuf.dptr == ଖାଲି) {
		tdb->ecode = TDB_ERR_OOM;
		ଯାଅ failed;
	}

	memcpy(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, ଚାବି, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	ଫେରନ୍ତୁ ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ଅଚଳ ଅକ୍ଷର *tdb_name(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_fd(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->log.log_fn;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_get_seqnum(ସଂରଚନା tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ଯଦି (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ seqnum;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_hash_size(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->header.hash_size;
}

size_t tdb_map_size(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->map_size;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_get_flags(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
ଶୂନ୍ୟ tdb_enable_seqnum(ସଂରଚନା tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ସ୍ଥିର ସଂରଚନା tdb_context *tdbs = ଖାଲି;


/* This is from a hash algorithm suggested by Rogier Wolff */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_tdb_hash(TDB_DATA *ଚାବି)
{
	u32 ମୂଲ୍ୟ;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	ପାଇଁ (ମୂଲ୍ୟ = 0, i=0; i < ଚାବି->dsize; i++)
		ମୂଲ୍ୟ = ମୂଲ୍ୟ * 256 + ଚାବି->dptr[i] + (ମୂଲ୍ୟ >> 24) * 241;

	ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


/* initialise a new database with a specified hash size */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_new_database(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_size)
{
	ସଂରଚନା tdb_header *newdb;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	size = ଆକାର(ସଂରଚନା tdb_header) + (hash_size+1)*ଆକାର(tdb_off_t);
	ଯଦି (!(newdb = (ସଂରଚନା tdb_header *)calloc(size, 1)))
		ଫେରନ୍ତୁ TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_size = hash_size;
	ଯଦି (tdb->flags & TDB_INTERNAL) {
		tdb->map_size = size;
		tdb->map_ptr = (ଅକ୍ଷର *)newdb;
		memcpy(&tdb->header, newdb, ଆକାର(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (lseek(tdb->fd, 0, SEEK_SET) == -1)
		ଯାଅ fail;

	ଯଦି (ftruncate(tdb->fd, 0) == -1)
		ଯାଅ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	memcpy(&tdb->header, newdb, ଆକାର(tdb->header));
	/* Don't endian-convert the magic food! */
	memcpy(newdb->magic_food, TDB_MAGIC_FOOD, strlen(TDB_MAGIC_FOOD)+1);
	ଯଦି (ଲିଖ(tdb->fd, newdb, size) != size) {
		ret = -1;
	} ଅନ୍ୟଥା {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	ଫେରନ୍ତୁ ret;
}



ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_already_open(dev_t device,
			    ino_t ino)
{
	ସଂରଚନା tdb_context *i;

	ପାଇଁ (i = tdbs; i; i = i->next) {
		ଯଦି (i->device == device && i->inode == ino) {
			ଫେରନ୍ତୁ 1;
		}
	}

	ଫେରନ୍ତୁ 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ସଂରଚନା tdb_context *tdb_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_flags,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags, mode_t mode)
{
	ଫେରନ୍ତୁ tdb_open_ex(name, hash_size, tdb_flags, open_flags, mode, ଖାଲି, ଖାଲି);
}

/* a default logging function */
ସ୍ଥିର ଶୂନ୍ୟ null_log_fn(ସଂରଚନା tdb_context *tdb, ଗଣନା tdb_debug_level level, ଅଚଳ ଅକ୍ଷର *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ସ୍ଥିର ଶୂନ୍ୟ null_log_fn(ସଂରଚନା tdb_context *tdb EXT2FS_ATTR((unused)),
			ଗଣନା tdb_debug_level level EXT2FS_ATTR((unused)),
			ଅଚଳ ଅକ୍ଷର *fmt EXT2FS_ATTR((unused)), ...)
{
}


ସଂରଚନା tdb_context *tdb_open_ex(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_flags,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags, mode_t mode,
				ଅଚଳ ସଂରଚନା tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ସଂରଚନା tdb_context *tdb;
	ସଂରଚନା stat st;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rev = 0, locked = 0;
	unsigned ଅକ୍ଷର *vp;
	u32 vertest;

	ଯଦି (!(tdb = (ସଂରଚନା tdb_context *)calloc(1, ଆକାର *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		ଯାଅ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ଖାଲି;
	tdb->map_ptr = ଖାଲି;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	ଯଦି (log_ctx) {
		tdb->log = *log_ctx;
	} ଅନ୍ୟଥା {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = ଖାଲି;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_size = sysconf(_SC_PAGESIZE);
	ଯଦି (tdb->page_size <= 0) {
		tdb->page_size = 0x2000;
	}

	ଯଦି ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		ଯାଅ fail;
	}

	ଯଦି (hash_size == 0)
		hash_size = DEFAULT_HASH_SIZE;
	ଯଦି ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	ଯଦି (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		ଯଦି (tdb_new_database(tdb, hash_size) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			ଯାଅ fail;
		}
		ଯାଅ internal;
	}

	ଯଦି ((tdb->fd = open(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		ଯାଅ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	ଯଦି (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		ଯାଅ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	ଯଦି ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		ଯଦି (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			ଯାଅ fail; /* errno set by ftruncate */
		}
	}

	ଯଦି (ପଠ(tdb->fd, &tdb->header, ଆକାର(tdb->header)) != ଆକାର(tdb->header)
	    || memcmp(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      ଆକାର(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		ଯଦି (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_size) == -1) {
			errno = EIO; /* ie bad format or something */
			ଯାଅ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (unsigned ଅକ୍ଷର *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	ଯଦି (!rev)
		tdb->flags &= ~TDB_CONVERT;
	ଅନ୍ୟଥା {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, ଆକାର(tdb->header));
	}
	ଯଦି (fstat(tdb->fd, &st) == -1)
		ଯାଅ fail;

	ଯଦି (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		ଯାଅ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	ଯଦି (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)st.st_dev, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)st.st_ino));
		errno = EBUSY;
		ଯାଅ fail;
	}

	ଯଦି (!(tdb->name = (ଅକ୍ଷର *)strdup(name))) {
		errno = ENOMEM;
		ଯାଅ fail;
	}

	tdb->map_size = st.st_size;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	ଯଦି (locked) {
		ଯଦି (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			ଯାଅ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	ଯଦି (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		ଯଦି (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			ଯାଅ fail;
	}

	/* if needed, run recovery */
	ଯଦି (tdb_transaction_recover(tdb) == -1) {
		ଯାଅ fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	ଯଦି (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		ଯାଅ fail;
	tdb->next = tdbs;
	tdbs = tdb;
	ଫେରନ୍ତୁ tdb;

 fail:
	{ ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_errno = errno;

	ଯଦି (!tdb)
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (tdb->map_ptr) {
		ଯଦି (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		ଅନ୍ୟଥା
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	ଯଦି (tdb->fd != -1)
		ଯଦି (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	ଫେରନ୍ତୁ ଖାଲି;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

ଶୂନ୍ୟ tdb_set_max_dead(ସଂରଚନା tdb_context *tdb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_close(ସଂରଚନା tdb_context *tdb)
{
	ସଂରଚନା tdb_context **i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	ଯଦି (tdb->map_ptr) {
		ଯଦି (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		ଅନ୍ୟଥା
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	ଯଦି (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	ପାଇଁ (i = &tdbs; *i; i = &(*i)->next) {
		ଯଦି (*i == tdb) {
			*i = tdb->next;
			ଭାଙ୍ଗ;
		}
	}

	memset(tdb, 0, ଆକାର(*tdb));
	SAFE_FREE(tdb);

	ଫେରନ୍ତୁ ret;
}

/* register a logging function */
ଶୂନ୍ୟ tdb_set_logging_function(ସଂରଚନା tdb_context *tdb,
                              ଅଚଳ ସଂରଚନା tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

ଶୂନ୍ୟ *tdb_get_logging_private(ସଂରଚନା tdb_context *tdb)
{
	ଫେରନ୍ତୁ tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_reopen(ସଂରଚନା tdb_context *tdb)
{
	ସଂରଚନା stat st;

	ଯଦି (tdb->flags & TDB_INTERNAL) {
		ଫେରନ୍ତୁ 0; /* Nothing to do. */
	}

	ଯଦି (tdb->ଅଙ୍କ_locks != 0 || tdb->global_lock.ଗଣନା_2) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		ଯାଅ fail;
	}

	ଯଦି (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		ଯାଅ fail;
	}

	ଯଦି (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		ଯାଅ fail;
	}
	ଯଦି (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	ଯଦି (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		ଯାଅ fail;
	}
	ଯଦି ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		ଯାଅ fail;
	}
	ଯଦି (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		ଯାଅ fail;
	}
	ଯଦି (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		ଯାଅ fail;
	}
	tdb_mmap(tdb);

	ଫେରନ୍ତୁ 0;

fail:
	tdb_close(tdb);
	ଫେରନ୍ତୁ -1;
}

/* reopen all tdb's */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_reopen_all(ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_longlived)
{
	ସଂରଚନା tdb_context *tdb;

	ପାଇଁ (tdb=tdbs; tdb; tdb = tdb->next) {
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
		ଯଦି (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		ଯଦି (tdb_reopen(tdb) != 0)
			ଫେରନ୍ତୁ -1;
	}

	ଫେରନ୍ତୁ 0;
}

/**
 * Flush a database file from the page cache.
 **/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା tdb_flush(ସଂରଚନା tdb_context *tdb)
{
	ଯଦି (tdb->fd != -1)
		ଫେରନ୍ତୁ fsync(tdb->fd);
	ଫେରନ୍ତୁ 0;
}
