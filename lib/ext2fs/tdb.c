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

#جے_تعریف_شدہ CONFIG_STAND_ALONE
#تعریف_کرو HAVE_MMAP
#تعریف_کرو HAVE_STRDUP
#تعریف_کرو HAVE_SYS_MMAN_H
#تعریف_کرو HAVE_UTIME_H
#تعریف_کرو HAVE_UTIME
#جے_انت
#جے_بنا_تعریف __FreeBSD__
#تعریف_کرو XOPEN_سروت 600
#جے_انت

#ضمن "config.h"
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <stdarg.h>
#ضمن <stddef.h>
#ضمن <errno.h>
#ضمن <string.h>
#جے_تعریف_شدہ HAVE_SYS_SELECT_H
#ضمن <sys/select.h>
#جے_انت
#ضمن <sys/time.h>
#ضمن <sys/types.h>
#ضمن <time.h>
#جے_تعریف_شدہ HAVE_UTIME_H
#ضمن <utime.h>
#جے_انت
#ضمن <sys/stat.h>
#ضمن <sys/file.h>
#ضمن <fcntl.h>

#جے_تعریف_شدہ HAVE_SYS_MMAN_H
#ضمن <sys/mman.h>
#جے_انت

#جے_تعریف_شدہ __GNUC__
#تعریف_کرو EXT2FS_ATTR(x) __attribute__(x)
#وإلا
#تعریف_کرو EXT2FS_ATTR(x)
#جے_انت

#جے_بنا_تعریف MAP_FILE
#تعریف_کرو MAP_FILE 0
#جے_انت

#جے_بنا_تعریف MAP_FAILED
#تعریف_کرو MAP_FAILED ((فراغ *)-1)
#جے_انت

#جے_بنا_تعریف HAVE_STRDUP
#تعریف_کرو strdup rep_strdup
ساكن حرف *rep_strdup(ثابت حرف *s)
{
	حرف *ret;
	صحيح لمائی;

	إذا (!s)
		أرجع عدم;
	لمائی = لڑی_لمبائی(s);
	ret = حافظہ_دیو(لمائی + 1);
	إذا (ret) {
		strncpy(ret, s, لمائی);
		ret[لمائی] = '\0';
	}
	أرجع ret;
}
#جے_انت

#جے_بنا_تعریف PRINTF_ATTRIBUTE
#إذا (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وإلا
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2)
#جے_انت
#جے_انت

#إذا defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
تعريفنوع صحيح bool;
#جے_انت

#ضمن "tdb.h"

ساكن TDB_انکڑے tdb_null;

#جے_بنا_تعریف u32
#تعریف_کرو u32 بنانشانی
#جے_انت

تعريفنوع u32 tdb_len_t;
تعريفنوع u32 tdb_off_t;

#جے_بنا_تعریف offsetof
#تعریف_کرو offsetof(t,f) ((بنانشانی صحيح)&((t *)0)->f)
#جے_انت

#تعریف_کرو TDB_MAGIC_FOOD "TDB file\n"
#تعریف_کرو TDB_نسخہ (0x26011967 + 6)
#تعریف_کرو TDB_MAGIC (0x26011999U)
#تعریف_کرو TDB_FREE_MAGIC (~TDB_MAGIC)
#تعریف_کرو TDB_DEAD_MAGIC (0xFEE1DEAD)
#تعریف_کرو TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#تعریف_کرو TDB_ALIGNMENT 4
#تعریف_کرو MIN_REC_ناپ (2*حجم(بنية list_struct) + TDB_ALIGNMENT)
#تعریف_کرو DEFAULT_HASH_ناپ 131
#تعریف_کرو FREELIST_چوٹی (حجم(بنية tdb_سرخی))
#تعریف_کرو TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#تعریف_کرو TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#تعریف_کرو TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#تعریف_کرو TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#تعریف_کرو TDB_HASH_چوٹی(hash) (FREELIST_چوٹی + (بالٹی(hash)+1)*حجم(tdb_off_t))
#تعریف_کرو TDB_HASHTABLE_ناپ(tdb) ((tdb->سرخی.hash_ناپ+1)*حجم(tdb_off_t))
#تعریف_کرو TDB_انکڑے_شروع(hash_ناپ) TDB_HASH_چوٹی(hash_ناپ-1)
#تعریف_کرو TDB_RECOVERY_سر offsetof(بنية tdb_سرخی, recovery_شروع)
#تعریف_کرو TDB_SEQNUM_OFS    offsetof(بنية tdb_سرخی, sequence_نمبر)
#تعریف_کرو TDB_PAD_BYTE 0x42
#تعریف_کرو TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#تعریف_کرو TDB_LOG(x) tdb->لوگارتھم.log_fn x

/* lock offsets */
#تعریف_کرو GLOBAL_جندرا      0
#تعریف_کرو سرگرم_جندرا      4
#تعریف_کرو TRANSACTION_جندرا 8

/* free memory if the pointer is valid and zero the pointer */
#جے_بنا_تعریف محفوظ_FREE
#تعریف_کرو محفوظ_FREE(x) افعل { إذا ((x) != عدم) {حرر(x); (x)=عدم;} } طالما(0)
#جے_انت

#تعریف_کرو بالٹی(hash) ((hash) % tdb->سرخی.hash_ناپ)

#تعریف_کرو DOCONV() (tdb->flags & TDB_CONVERT)
#تعریف_کرو CONVERT(x) (DOCONV() ? tdb_convert(&x, حجم(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
بنية list_struct {
	tdb_off_t اگلا; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t کلید_len; /* byte length of key */
	tdb_len_t انکڑے_len; /* byte length of data */
	u32 بھریا_hash; /* the full 32 bit hash of the key */
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
بنية tdb_سرخی {
	حرف magic_food[32]; /* for /etc/magic */
	u32 نسخہ; /* version of the code */
	u32 hash_ناپ; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_شروع; /* offset of transaction recovery region */
	tdb_off_t sequence_نمبر; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

بنية tdb_جندرا_type {
	صحيح list;
	u32 گنتی;
	u32 ltype;
};

بنية tdb_traverse_جندرا {
	بنية tdb_traverse_جندرا *اگلا;
	u32 off;
	u32 hash;
	صحيح جندرا_rw;
};


بنية tdb_methods {
	صحيح (*tdb_read)(بنية tdb_context *, tdb_off_t , فراغ *, tdb_len_t , صحيح );
	صحيح (*tdb_write)(بنية tdb_context *, tdb_off_t, ثابت فراغ *, tdb_len_t);
	فراغ (*اگلا_hash_chain)(بنية tdb_context *, u32 *);
	صحيح (*tdb_oob)(بنية tdb_context *, tdb_off_t , صحيح );
	صحيح (*tdb_expand_file)(بنية tdb_context *, tdb_off_t , tdb_off_t );
	صحيح (*tdb_brlock)(بنية tdb_context *, tdb_off_t , صحيح, صحيح, صحيح, size_t);
};

بنية tdb_context {
	حرف *name; /* the name of the database */
	فراغ *map_ptr; /* where it is currently mapped */
	صحيح fd; /* open file descriptor for the database */
	tdb_len_t map_ناپ; /* how much space has been mapped */
	صحيح read_only; /* opened read-only */
	صحيح traverse_read; /* read-only traversal */
	بنية tdb_جندرا_type global_جندرا;
	صحيح عدد_lockrecs;
	بنية tdb_جندرا_type *lockrecs; /* only real locks, all with count>0 */
	تعداد TDB_ERROR ecode; /* error code for last tdb error */
	بنية tdb_سرخی سرخی; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	بنية tdb_traverse_جندرا travlocks; /* current traversal locks */
	بنية tdb_context *اگلا; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	بنية tdb_logging_context لوگارتھم;
	بنانشانی صحيح (*hash_fn)(TDB_انکڑے *کلید);
	صحيح open_flags; /* flags used in the open - needed by reopen */
	بنانشانی صحيح عدد_locks; /* number of chain locks held */
	ثابت بنية tdb_methods *methods;
	بنية tdb_transaction *transaction;
	صحيح ورقہ_ناپ;
	صحيح max_dead_ریکارڈاں;
	bool have_transaction_جندرا;
	tdb_len_t real_map_ناپ; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ساكن صحيح tdb_munmap(بنية tdb_context *tdb);
ساكن فراغ tdb_mmap(بنية tdb_context *tdb);
ساكن صحيح tdb_جندرا(بنية tdb_context *tdb, صحيح list, صحيح ltype);
صحيح tdb_جندرا_nonblock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
ساكن صحيح tdb_unlock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
ساكن صحيح tdb_brlock(بنية tdb_context *tdb, tdb_off_t وتھ, صحيح rw_type, صحيح lck_type, صحيح probe, size_t len);
ساكن صحيح tdb_transaction_جندرا(بنية tdb_context *tdb, صحيح ltype);
ساكن صحيح tdb_transaction_unlock(بنية tdb_context *tdb);
ساكن صحيح tdb_brlock_upgrade(بنية tdb_context *tdb, tdb_off_t وتھ, size_t len);
ساكن صحيح tdb_write_جندرا_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_write_unlock_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_off_t *d);
ساكن صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_off_t *d);
ساكن فراغ *tdb_convert(فراغ *buf, u32 ناپ);
ساكن صحيح tdb_free(بنية tdb_context *tdb, tdb_off_t وتھ, بنية list_struct *rec);
ساكن tdb_off_t tdb_allocate(بنية tdb_context *tdb, tdb_len_t لمائی, بنية list_struct *rec);
ساكن صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_off_t *d);
ساكن صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_off_t *d);
ساكن صحيح tdb_جندرا_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_unlock_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_rec_read(بنية tdb_context *tdb, tdb_off_t وتھ, بنية list_struct *rec);
ساكن صحيح tdb_rec_write(بنية tdb_context *tdb, tdb_off_t وتھ, بنية list_struct *rec);
ساكن صحيح tdb_do_delete(بنية tdb_context *tdb, tdb_off_t rec_ptr, بنية list_struct *rec);
ساكن بنانشانی حرف *tdb_alloc_read(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_len_t len);
ساكن صحيح tdb_parse_انکڑے(بنية tdb_context *tdb, TDB_انکڑے کلید,
		   tdb_off_t وتھ, tdb_len_t len,
		   صحيح (*parser)(TDB_انکڑے کلید, TDB_انکڑے انکڑے,
				 فراغ *نجی_انکڑے),
		   فراغ *نجی_انکڑے);
ساكن tdb_off_t tdb_لبھو_جندرا_hash(بنية tdb_context *tdb, TDB_انکڑے کلید, u32 hash, صحيح locktype,
			   بنية list_struct *rec);
ساكن فراغ tdb_io_init(بنية tdb_context *tdb);
ساكن صحيح tdb_expand(بنية tdb_context *tdb, tdb_off_t ناپ);
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
	     {TDB_ERR_جندرا, "Locking error"},
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

#تعریف_کرو TDB_MARK_جندرا 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
صحيح tdb_brlock(بنية tdb_context *tdb, tdb_off_t وتھ,
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
	fl.l_شروع = وتھ;
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
			tdb->ecode = TDB_ERR_جندرا;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, وتھ, rw_type, lck_type, (صحيح)len));
		}
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}
	أرجع 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
صحيح tdb_brlock_upgrade(بنية tdb_context *tdb, tdb_off_t وتھ, size_t len)
{
	صحيح گنتی = 1000;
	طالما (گنتی--) {
		بنية timeval tv;
		إذا (tdb_brlock(tdb, وتھ, F_WRLCK, F_SETLKW, 1, len) == 0) {
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
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", وتھ));
	أرجع -1;
}


/* lock a list in the database. list -1 is the alloc list */
ساكن صحيح tdb_جندرا_2(بنية tdb_context *tdb, صحيح list, صحيح ltype, صحيح op)
{
	بنية tdb_جندرا_type *new_lck;
	صحيح i;
	bool mark_جندرا = ((ltype & TDB_MARK_جندرا) == TDB_MARK_جندرا);

	ltype &= ~TDB_MARK_جندرا;

	/* a global lock allows us to avoid per chain locks */
	إذا (tdb->global_جندرا.گنتی &&
	    ((u32)ltype == tdb->global_جندرا.ltype || ltype == F_RDLCK)) {
		أرجع 0;
	}

	إذا (tdb->global_جندرا.گنتی) {
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}

	إذا (list < -1 || list >= (صحيح)tdb->سرخی.hash_ناپ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		أرجع -1;
	}
	إذا (tdb->flags & TDB_NOLOCK)
		أرجع 0;

	لكل (i=0; i<tdb->عدد_lockrecs; i++) {
		إذا (tdb->lockrecs[i].list == list) {
			إذا (tdb->lockrecs[i].گنتی == 0) {
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
			tdb->lockrecs[i].گنتی++;
			أرجع 0;
		}
	}

	new_lck = (بنية tdb_جندرا_type *)realloc(
		tdb->lockrecs,
		حجم(*tdb->lockrecs) * (tdb->عدد_lockrecs+1));
	إذا (new_lck == عدم) {
		errno = ENOMEM;
		أرجع -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	إذا (!mark_جندرا &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_چوٹی+4*list, ltype, op,
				     0, 1)) {
		أرجع -1;
	}

	tdb->عدد_locks++;

	tdb->lockrecs[tdb->عدد_lockrecs].list = list;
	tdb->lockrecs[tdb->عدد_lockrecs].گنتی = 1;
	tdb->lockrecs[tdb->عدد_lockrecs].ltype = ltype;
	tdb->عدد_lockrecs += 1;

	أرجع 0;
}

/* lock a list in the database. list -1 is the alloc list */
صحيح tdb_جندرا(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	صحيح ret;
	ret = tdb_جندرا_2(tdb, list, ltype, F_SETLKW);
	إذا (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	أرجع ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
صحيح tdb_جندرا_nonblock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	أرجع tdb_جندرا_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
صحيح tdb_unlock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	صحيح ret = -1;
	صحيح i;
	بنية tdb_جندرا_type *lck = عدم;
	bool mark_جندرا = ((ltype & TDB_MARK_جندرا) == TDB_MARK_جندرا);

	ltype &= ~TDB_MARK_جندرا;

	/* a global lock allows us to avoid per chain locks */
	إذا (tdb->global_جندرا.گنتی &&
	    ((u32)ltype == tdb->global_جندرا.ltype || ltype == F_RDLCK)) {
		أرجع 0;
	}

	إذا (tdb->global_جندرا.گنتی) {
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}

	إذا (tdb->flags & TDB_NOLOCK)
		أرجع 0;

	/* Sanity checks */
	إذا (list < -1 || list >= (صحيح)tdb->سرخی.hash_ناپ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->سرخی.hash_ناپ));
		أرجع ret;
	}

	لكل (i=0; i<tdb->عدد_lockrecs; i++) {
		إذا (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			اكسر;
		}
	}

	إذا ((lck == عدم) || (lck->گنتی == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		أرجع -1;
	}

	إذا (lck->گنتی > 1) {
		lck->گنتی--;
		أرجع 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	إذا (mark_جندرا) {
		ret = 0;
	} وإلا {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_چوٹی+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->عدد_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	إذا (tdb->عدد_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->عدد_lockrecs-1];
	}
	tdb->عدد_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	إذا (tdb->عدد_lockrecs == 0) {
		محفوظ_FREE(tdb->lockrecs);
	}

	إذا (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	أرجع ret;
}

/*
  get the transaction lock
 */
صحيح tdb_transaction_جندرا(بنية tdb_context *tdb, صحيح ltype)
{
	إذا (tdb->have_transaction_جندرا || tdb->global_جندرا.گنتی) {
		أرجع 0;
	}
	إذا (tdb->methods->tdb_brlock(tdb, TRANSACTION_جندرا, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_جندرا;
		أرجع -1;
	}
	tdb->have_transaction_جندرا = 1;
	أرجع 0;
}

/*
  release the transaction lock
 */
صحيح tdb_transaction_unlock(بنية tdb_context *tdb)
{
	صحيح ret;
	إذا (!tdb->have_transaction_جندرا) {
		أرجع 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_جندرا, F_UNLCK, F_SETLKW, 0, 1);
	إذا (ret == 0) {
		tdb->have_transaction_جندرا = 0;
	}
	أرجع ret;
}




/* lock/unlock entire database */
ساكن صحيح _tdb_lockall(بنية tdb_context *tdb, صحيح ltype, صحيح op)
{
	bool mark_جندرا = ((ltype & TDB_MARK_جندرا) == TDB_MARK_جندرا);

	ltype &= ~TDB_MARK_جندرا;

	/* There are no locks on read-only dbs */
	إذا (tdb->read_only || tdb->traverse_read)
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);

	إذا (tdb->global_جندرا.گنتی && tdb->global_جندرا.ltype == (u32)ltype) {
		tdb->global_جندرا.گنتی++;
		أرجع 0;
	}

	إذا (tdb->global_جندرا.گنتی) {
		/* a global lock of a different type exists */
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}

	إذا (tdb->عدد_locks != 0) {
		/* can't combine global and chain locks */
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}

	إذا (!mark_جندرا &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_چوٹی, ltype, op,
				     0, 4*tdb->سرخی.hash_ناپ)) {
		إذا (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		أرجع -1;
	}

	tdb->global_جندرا.گنتی = 1;
	tdb->global_جندرا.ltype = ltype;

	أرجع 0;
}



/* unlock entire db */
ساكن صحيح _tdb_unlockall(بنية tdb_context *tdb, صحيح ltype)
{
	bool mark_جندرا = ((ltype & TDB_MARK_جندرا) == TDB_MARK_جندرا);

	ltype &= ~TDB_MARK_جندرا;

	/* There are no locks on read-only dbs */
	إذا (tdb->read_only || tdb->traverse_read) {
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}

	إذا (tdb->global_جندرا.ltype != (u32)ltype ||
	    tdb->global_جندرا.گنتی == 0) {
		أرجع TDB_ERRCODE(TDB_ERR_جندرا, -1);
	}

	إذا (tdb->global_جندرا.گنتی > 1) {
		tdb->global_جندرا.گنتی--;
		أرجع 0;
	}

	إذا (!mark_جندرا &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW,
				     0, 4*tdb->سرخی.hash_ناپ)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		أرجع -1;
	}

	tdb->global_جندرا.گنتی = 0;
	tdb->global_جندرا.ltype = 0;

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
	أرجع _tdb_lockall(tdb, F_WRLCK | TDB_MARK_جندرا, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
صحيح tdb_lockall_unmark(بنية tdb_context *tdb)
{
	أرجع _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_جندرا);
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
صحيح tdb_chainlock(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_جندرا(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
صحيح tdb_chainlock_nonblock(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_جندرا_nonblock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
صحيح tdb_chainlock_mark(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_جندرا(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_جندرا);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
صحيح tdb_chainlock_unmark(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_unlock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_جندرا);
}

صحيح tdb_chainunlock(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_unlock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK);
}

صحيح tdb_chainlock_read(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_جندرا(tdb, بالٹی(tdb->hash_fn(&کلید)), F_RDLCK);
}

صحيح tdb_chainunlock_read(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	أرجع tdb_unlock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_RDLCK);
}



/* record lock stops delete underneath */
صحيح tdb_جندرا_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off)
{
	أرجع off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحيح tdb_write_جندرا_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off)
{
	بنية tdb_traverse_جندرا *i;
	لكل (i = &tdb->travlocks; i; i = i->اگلا)
		إذا (i->off == off)
			أرجع -1;
	أرجع tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحيح tdb_write_unlock_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off)
{
	أرجع tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
صحيح tdb_unlock_ریکارڈ(بنية tdb_context *tdb, tdb_off_t off)
{
	بنية tdb_traverse_جندرا *i;
	u32 گنتی = 0;

	إذا (off == 0)
		أرجع 0;
	لكل (i = &tdb->travlocks; i; i = i->اگلا)
		إذا (i->off == off)
			گنتی++;
	أرجع (گنتی == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
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
	إذا (len <= tdb->map_ناپ)
		أرجع 0;
	إذا (tdb->flags & TDB_اندرونی) {
		إذا (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (صحيح)len, (صحيح)tdb->map_ناپ));
		}
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	إذا (fstat(tdb->fd, &st) == -1) {
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	إذا (st.st_ناپ < (off_t)len) {
		إذا (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (صحيح)len, (صحيح)st.st_ناپ));
		}
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	إذا (tdb_munmap(tdb) == -1)
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_ناپ = st.st_ناپ;
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
		حافظہ_نقل(off + (حرف *)tdb->map_ptr, buf, len);
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
فراغ *tdb_convert(فراغ *buf, u32 ناپ)
{
	u32 i, *p = (u32 *)buf;
	لكل (i = 0; i < ناپ / 4; i++)
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
		حافظہ_نقل(buf, off + (حرف *)tdb->map_ptr, len);
	} وإلا {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		إذا (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (صحيح)off, (صحيح)len, (صحيح)ret, strerror(errno),
				 (صحيح)tdb->map_ناپ));
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
ساكن فراغ tdb_اگلا_hash_chain(بنية tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	إذا (tdb->map_ptr) {
		لكل (;h < tdb->سرخی.hash_ناپ;h++) {
			إذا (0 != *(u32 *)(TDB_HASH_چوٹی(h) + (بنانشانی حرف *)tdb->map_ptr)) {
				اكسر;
			}
		}
	} وإلا {
		u32 off=0;
		لكل (;h < tdb->سرخی.hash_ناپ;h++) {
			إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(h), &off) != 0 || off != 0) {
				اكسر;
			}
		}
	}
	(*chain) = h;
}


صحيح tdb_munmap(بنية tdb_context *tdb)
{
	إذا (tdb->flags & TDB_اندرونی)
		أرجع 0;

#جے_تعریف_شدہ HAVE_MMAP
	إذا (tdb->map_ptr) {
		صحيح ret = munmap(tdb->map_ptr, tdb->real_map_ناپ);
		إذا (ret != 0)
			أرجع ret;
		tdb->real_map_ناپ = 0;
	}
#جے_انت
	tdb->map_ptr = عدم;
	أرجع 0;
}

فراغ tdb_mmap(بنية tdb_context *tdb)
{
	إذا (tdb->flags & TDB_اندرونی)
		أرجع;

#جے_تعریف_شدہ HAVE_MMAP
	إذا (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(عدم, tdb->map_ناپ,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		إذا (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_ناپ = 0;
			tdb->map_ptr = عدم;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_ناپ, strerror(errno)));
		}
		tdb->real_map_ناپ = tdb->map_ناپ;
	} وإلا {
		tdb->map_ptr = عدم;
	}
#وإلا
	tdb->map_ptr = عدم;
#جے_انت
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ساكن صحيح tdb_expand_file(بنية tdb_context *tdb, tdb_off_t ناپ, tdb_off_t addition)
{
	حرف buf[1024];

	إذا (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		أرجع -1;
	}

	إذا (ftruncate(tdb->fd, ناپ+addition) == -1) {
		حرف b = 0;
		إذا (pwrite(tdb->fd,  &b, 1, (ناپ+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 ناپ+addition, strerror(errno)));
			أرجع -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, حجم(buf));
	طالما (addition) {
		صحيح n = addition>حجم(buf)?حجم(buf):addition;
		صحيح ret = pwrite(tdb->fd, buf, n, ناپ);
		إذا (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			أرجع -1;
		}
		addition -= n;
		ناپ += n;
	}
	أرجع 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
صحيح tdb_expand(بنية tdb_context *tdb, tdb_off_t ناپ)
{
	بنية list_struct rec;
	tdb_off_t وتھ;

	إذا (tdb_جندرا(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		أرجع -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_ناپ + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	ناپ = TDB_ALIGN(tdb->map_ناپ + ناپ*10, tdb->ورقہ_ناپ) - tdb->map_ناپ;

	إذا (!(tdb->flags & TDB_اندرونی))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	إذا (!(tdb->flags & TDB_اندرونی)) {
		إذا (tdb->methods->tdb_expand_file(tdb, tdb->map_ناپ, ناپ) != 0)
			اذهب fail;
	}

	tdb->map_ناپ += ناپ;

	إذا (tdb->flags & TDB_اندرونی) {
		حرف *new_map_ptr = (حرف *)realloc(tdb->map_ptr,
						    tdb->map_ناپ);
		إذا (!new_map_ptr) {
			tdb->map_ناپ -= ناپ;
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
	rec.rec_len = ناپ - حجم(rec);

	/* link it into the free list */
	وتھ = tdb->map_ناپ - ناپ;
	إذا (tdb_free(tdb, وتھ, &rec) == -1)
		اذهب fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع -1;
}

/* read/write a tdb_off_t */
صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_off_t *d)
{
	أرجع tdb->methods->tdb_read(tdb, وتھ, (حرف*)d, حجم(*d), DOCONV());
}

صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_off_t *d)
{
	tdb_off_t off = *d;
	أرجع tdb->methods->tdb_write(tdb, وتھ, CONVERT(off), حجم(*d));
}


/* read a lump of data, allocating the space for it */
بنانشانی حرف *tdb_alloc_read(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_len_t len)
{
	بنانشانی حرف *buf;

	/* some systems don't like zero length malloc */
	إذا (len == 0) {
		len = 1;
	}

	إذا (!(buf = (بنانشانی حرف *)حافظہ_دیو(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		أرجع TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	إذا (tdb->methods->tdb_read(tdb, وتھ, buf, len, 0) == -1) {
		محفوظ_FREE(buf);
		أرجع عدم;
	}
	أرجع buf;
}

/* Give a piece of tdb data to a parser */

صحيح tdb_parse_انکڑے(بنية tdb_context *tdb, TDB_انکڑے کلید,
		   tdb_off_t وتھ, tdb_len_t len,
		   صحيح (*parser)(TDB_انکڑے کلید, TDB_انکڑے انکڑے,
				 فراغ *نجی_انکڑے),
		   فراغ *نجی_انکڑے)
{
	TDB_انکڑے انکڑے;
	صحيح نتیجہ;

	انکڑے.dsize = len;

	إذا ((tdb->transaction == عدم) && (tdb->map_ptr != عدم)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		إذا (tdb->methods->tdb_oob(tdb, وتھ+len, 0) != 0) {
			أرجع -1;
		}
		انکڑے.dptr = وتھ + (بنانشانی حرف *)tdb->map_ptr;
		أرجع parser(کلید, انکڑے, نجی_انکڑے);
	}

	إذا (!(انکڑے.dptr = tdb_alloc_read(tdb, وتھ, len))) {
		أرجع -1;
	}

	نتیجہ = parser(کلید, انکڑے, نجی_انکڑے);
	حرر(انکڑے.dptr);
	أرجع نتیجہ;
}

/* read/write a record */
صحيح tdb_rec_read(بنية tdb_context *tdb, tdb_off_t وتھ, بنية list_struct *rec)
{
	إذا (tdb->methods->tdb_read(tdb, وتھ, rec, حجم(*rec),DOCONV()) == -1)
		أرجع -1;
	إذا (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, وتھ));
		أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	أرجع tdb->methods->tdb_oob(tdb, rec->اگلا+حجم(*rec), 0);
}

صحيح tdb_rec_write(بنية tdb_context *tdb, tdb_off_t وتھ, بنية list_struct *rec)
{
	بنية list_struct r = *rec;
	أرجع tdb->methods->tdb_write(tdb, وتھ, CONVERT(r), حجم(r));
}

ساكن ثابت بنية tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_اگلا_hash_chain,
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
	بنية tdb_transaction_el *اگلا, *prev;
	tdb_off_t وتھ;
	tdb_len_t لمائی;
	بنانشانی حرف *انکڑے;
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
	بنية tdb_transaction_el *elements, *elements_آخری;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	صحيح transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	صحيح nesting;

	/* old file size before transaction */
	tdb_len_t old_map_ناپ;
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
	لكل (el=tdb->transaction->elements_آخری;el;el=el->prev) {
		tdb_len_t partial;

		إذا (off+len <= el->وتھ) {
			استمر;
		}
		إذا (off >= el->وتھ + el->لمائی) {
			استمر;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		إذا (off < el->وتھ) {
			partial = el->وتھ - off;
			إذا (transaction_read(tdb, off, buf, partial, cv) != 0) {
				اذهب fail;
			}
			len -= partial;
			off += partial;
			buf = (فراغ *)(partial + (حرف *)buf);
		}
		إذا (off + len <= el->وتھ + el->لمائی) {
			partial = len;
		} وإلا {
			partial = el->وتھ + el->لمائی - off;
		}
		حافظہ_نقل(buf, el->انکڑے + (off - el->وتھ), partial);
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
	بنية tdb_transaction_el *el, *بہترین_el=عدم;

	إذا (len == 0) {
		أرجع 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	إذا (len == حجم(tdb_off_t) && off >= FREELIST_چوٹی &&
	    off < FREELIST_چوٹی+TDB_HASHTABLE_ناپ(tdb)) {
		u32 chain = (off-FREELIST_چوٹی) / حجم(tdb_off_t);
		حافظہ_نقل(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	لكل (el=tdb->transaction->elements_آخری;el;el=el->prev) {
		tdb_len_t partial;

		إذا (بہترین_el == عدم && off == el->وتھ+el->لمائی) {
			بہترین_el = el;
		}

		إذا (off+len <= el->وتھ) {
			استمر;
		}
		إذا (off >= el->وتھ + el->لمائی) {
			استمر;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		إذا (off < el->وتھ) {
			partial = el->وتھ - off;
			إذا (transaction_write(tdb, off, buf, partial) != 0) {
				اذهب fail;
			}
			len -= partial;
			off += partial;
			buf = (ثابت فراغ *)(partial + (ثابت حرف *)buf);
		}
		إذا (off + len <= el->وتھ + el->لمائی) {
			partial = len;
		} وإلا {
			partial = el->وتھ + el->لمائی - off;
		}
		حافظہ_نقل(el->انکڑے + (off - el->وتھ), buf, partial);
		len -= partial;
		off += partial;
		buf = (ثابت فراغ *)(partial + (ثابت حرف *)buf);

		إذا (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			اذهب fail;
		}

		أرجع 0;
	}

	/* see if we can append the new entry to an existing entry */
	إذا (بہترین_el && بہترین_el->وتھ + بہترین_el->لمائی == off &&
	    (off+len < tdb->transaction->old_map_ناپ ||
	     off > tdb->transaction->old_map_ناپ)) {
		بنانشانی حرف *انکڑے = بہترین_el->انکڑے;
		el = بہترین_el;
		el->انکڑے = (بنانشانی حرف *)realloc(el->انکڑے,
						    el->لمائی + len);
		إذا (el->انکڑے == عدم) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->انکڑے = انکڑے;
			أرجع -1;
		}
		إذا (buf) {
			حافظہ_نقل(el->انکڑے + el->لمائی, buf, len);
		} وإلا {
			memset(el->انکڑے + el->لمائی, TDB_PAD_BYTE, len);
		}
		el->لمائی += len;
		أرجع 0;
	}

	/* add a new entry at the end of the list */
	el = (بنية tdb_transaction_el *)حافظہ_دیو(حجم(*el));
	إذا (el == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		أرجع -1;
	}
	el->اگلا = عدم;
	el->prev = tdb->transaction->elements_آخری;
	el->وتھ = off;
	el->لمائی = len;
	el->انکڑے = (بنانشانی حرف *)حافظہ_دیو(len);
	إذا (el->انکڑے == عدم) {
		حرر(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		أرجع -1;
	}
	إذا (buf) {
		حافظہ_نقل(el->انکڑے, buf, len);
	} وإلا {
		memset(el->انکڑے, TDB_PAD_BYTE, len);
	}
	إذا (el->prev) {
		el->prev->اگلا = el;
	} وإلا {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_آخری = el;
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
ساكن فراغ transaction_اگلا_hash_chain(بنية tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	لكل (;h < tdb->سرخی.hash_ناپ;h++) {
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
			   صحيح probe EXT2FS_ATTR((بن_ورتیا)))
{
	إذا (len <= tdb->map_ناپ) {
		أرجع 0;
	}
	أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ساكن صحيح transaction_expand_file(بنية tdb_context *tdb, tdb_off_t ناپ,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	إذا (transaction_write(tdb, ناپ, عدم, addition) != 0) {
		أرجع -1;
	}

	أرجع 0;
}

/*
  brlock during a transaction - ignore them
*/
ساكن صحيح transaction_brlock(بنية tdb_context *tdb EXT2FS_ATTR((بن_ورتیا)),
			      tdb_off_t وتھ EXT2FS_ATTR((بن_ورتیا)),
			      صحيح rw_type EXT2FS_ATTR((بن_ورتیا)),
			      صحيح lck_type EXT2FS_ATTR((بن_ورتیا)),
			      صحيح probe EXT2FS_ATTR((بن_ورتیا)),
			      size_t len EXT2FS_ATTR((بن_ورتیا)))
{
	أرجع 0;
}

ساكن ثابت بنية tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_اگلا_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
صحيح tdb_transaction_شروع(بنية tdb_context *tdb)
{
	/* some sanity checks */
	إذا (tdb->read_only || (tdb->flags & TDB_اندرونی) || tdb->traverse_read) {
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

	إذا (tdb->عدد_locks != 0 || tdb->global_جندرا.گنتی) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_جندرا;
		أرجع -1;
	}

	إذا (tdb->travlocks.اگلا != عدم) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_جندرا;
		أرجع -1;
	}

	tdb->transaction = (بنية tdb_transaction *)
		صفر_حافظہ_دیو(حجم(بنية tdb_transaction), 1);
	إذا (tdb->transaction == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	إذا (tdb_transaction_جندرا(tdb, F_WRLCK) == -1) {
		محفوظ_FREE(tdb->transaction);
		أرجع -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	إذا (tdb_brlock(tdb, FREELIST_چوٹی, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_جندرا;
		اذهب fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		صفر_حافظہ_دیو(tdb->سرخی.hash_ناپ+1, حجم(u32));
	إذا (tdb->transaction->hash_heads == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب fail;
	}
	إذا (tdb->methods->tdb_read(tdb, FREELIST_چوٹی, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_ناپ(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		اذهب fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_ناپ + 1, 1);
	tdb->transaction->old_map_ناپ = tdb->map_ناپ;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	إذا (transaction_write(tdb, FREELIST_چوٹی, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_ناپ(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		اذهب fail;
	}

	أرجع 0;

fail:
	tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	محفوظ_FREE(tdb->transaction->hash_heads);
	محفوظ_FREE(tdb->transaction);
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

	tdb->map_ناپ = tdb->transaction->old_map_ناپ;

	/* free all the transaction elements */
	طالما (tdb->transaction->elements) {
		بنية tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->اگلا;
		حرر(el->انکڑے);
		حرر(el);
	}

	/* remove any global lock created during the transaction */
	إذا (tdb->global_جندرا.گنتی != 0) {
		tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW, 0, 4*tdb->سرخی.hash_ناپ);
		tdb->global_جندرا.گنتی = 0;
	}

	/* remove any locks created during the transaction */
	إذا (tdb->عدد_locks != 0) {
		صحيح i;
		لكل (i=0;i<tdb->عدد_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_چوٹی+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->عدد_locks = 0;
		tdb->عدد_lockrecs = 0;
		محفوظ_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	محفوظ_FREE(tdb->transaction->hash_heads);
	محفوظ_FREE(tdb->transaction);

	أرجع 0;
}

/*
  sync to disk
*/
ساكن صحيح transaction_sync(بنية tdb_context *tdb, tdb_off_t وتھ, tdb_len_t لمائی)
{
	إذا (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		أرجع -1;
	}
#إذا defined(HAVE_MSYNC) && defined(MS_SYNC)
	إذا (tdb->map_ptr) {
		tdb_off_t moffset = وتھ & ~(tdb->ورقہ_ناپ-1);
		إذا (msync(moffset + (حرف *)tdb->map_ptr,
			  لمائی + (وتھ - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			أرجع -1;
		}
	}
#جے_انت
	أرجع 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ساكن tdb_len_t tdb_recovery_ناپ(بنية tdb_context *tdb)
{
	بنية tdb_transaction_el *el;
	tdb_len_t recovery_ناپ = 0;

	recovery_ناپ = حجم(u32);
	لكل (el=tdb->transaction->elements;el;el=el->اگلا) {
		إذا (el->وتھ >= tdb->transaction->old_map_ناپ) {
			استمر;
		}
		recovery_ناپ += 2*حجم(tdb_off_t) + el->لمائی;
	}

	أرجع recovery_ناپ;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ساكن صحيح tdb_recovery_allocate(بنية tdb_context *tdb,
				 tdb_len_t *recovery_ناپ,
				 tdb_off_t *recovery_وتھ,
				 tdb_len_t *recovery_max_ناپ)
{
	بنية list_struct rec;
	ثابت بنية tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_سر;

	إذا (tdb_ofs_read(tdb, TDB_RECOVERY_سر, &recovery_سر) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		أرجع -1;
	}

	rec.rec_len = 0;

	إذا (recovery_سر != 0 &&
	    methods->tdb_read(tdb, recovery_سر, &rec, حجم(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		أرجع -1;
	}

	*recovery_ناپ = tdb_recovery_ناپ(tdb);

	إذا (recovery_سر != 0 && *recovery_ناپ <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_ناپ = rec.rec_len;
		*recovery_وتھ = recovery_سر;
		أرجع 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	إذا (recovery_سر != 0) {
		إذا (tdb_free(tdb, recovery_سر, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			أرجع -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_ناپ = tdb_recovery_ناپ(tdb);

	/* round up to a multiple of page size */
	*recovery_max_ناپ = TDB_ALIGN(حجم(rec) + *recovery_ناپ, tdb->ورقہ_ناپ) - حجم(rec);
	*recovery_وتھ = tdb->map_ناپ;
	recovery_سر = *recovery_وتھ;

	إذا (methods->tdb_expand_file(tdb, tdb->transaction->old_map_ناپ,
				     (tdb->map_ناپ - tdb->transaction->old_map_ناپ) +
				     حجم(rec) + *recovery_max_ناپ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		أرجع -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_ناپ + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_ناپ = tdb->map_ناپ;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_سر);
	إذا (methods->tdb_write(tdb, TDB_RECOVERY_سر,
			       &recovery_سر, حجم(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		أرجع -1;
	}

	أرجع 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ساكن صحيح transaction_تیاری_recovery(بنية tdb_context *tdb,
				      tdb_off_t *magic_وتھ)
{
	بنية tdb_transaction_el *el;
	tdb_len_t recovery_ناپ;
	بنانشانی حرف *انکڑے, *p;
	ثابت بنية tdb_methods *methods = tdb->transaction->io_methods;
	بنية list_struct *rec;
	tdb_off_t recovery_وتھ, recovery_max_ناپ;
	tdb_off_t old_map_ناپ = tdb->transaction->old_map_ناپ;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	إذا (tdb_recovery_allocate(tdb, &recovery_ناپ,
				  &recovery_وتھ, &recovery_max_ناپ) == -1) {
		أرجع -1;
	}

	انکڑے = (بنانشانی حرف *)حافظہ_دیو(recovery_ناپ + حجم(*rec));
	إذا (انکڑے == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	rec = (بنية list_struct *)انکڑے;
	memset(rec, 0, حجم(*rec));

	rec->magic    = 0;
	rec->انکڑے_len = recovery_ناپ;
	rec->rec_len  = recovery_max_ناپ;
	rec->کلید_len  = old_map_ناپ;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = انکڑے + حجم(*rec);
	لكل (el=tdb->transaction->elements;el;el=el->اگلا) {
		إذا (el->وتھ >= old_map_ناپ) {
			استمر;
		}
		إذا (el->وتھ + el->لمائی > tdb->transaction->old_map_ناپ) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			حرر(انکڑے);
			tdb->ecode = TDB_ERR_CORRUPT;
			أرجع -1;
		}
		حافظہ_نقل(p, &el->وتھ, 4);
		حافظہ_نقل(p+4, &el->لمائی, 4);
		إذا (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		إذا (methods->tdb_read(tdb, el->وتھ, p + 8, el->لمائی, 0) != 0) {
			حرر(انکڑے);
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
		p += 8 + el->لمائی;
	}

	/* and the tailer */
	tailer = حجم(*rec) + recovery_max_ناپ;
	حافظہ_نقل(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	إذا (methods->tdb_write(tdb, recovery_وتھ, انکڑے, حجم(*rec) + recovery_ناپ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		حرر(انکڑے);
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	إذا (transaction_sync(tdb, recovery_وتھ, حجم(*rec) + recovery_ناپ) == -1) {
		حرر(انکڑے);
		أرجع -1;
	}

	حرر(انکڑے);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_وتھ = recovery_وتھ + offsetof(بنية list_struct, magic);

	إذا (methods->tdb_write(tdb, *magic_وتھ, &magic, حجم(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* ensure the recovery magic marker is on disk */
	إذا (transaction_sync(tdb, *magic_وتھ, حجم(magic)) == -1) {
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
	tdb_off_t magic_وتھ = 0;
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
	إذا (tdb->عدد_locks || tdb->global_جندرا.گنتی) {
		tdb->ecode = TDB_ERR_جندرا;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	إذا (tdb_brlock_upgrade(tdb, FREELIST_چوٹی, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_جندرا;
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	إذا (tdb_brlock(tdb, GLOBAL_جندرا, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_جندرا;
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	إذا (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		إذا (transaction_تیاری_recovery(tdb, &magic_وتھ) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_جندرا, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			أرجع -1;
		}
	}

	/* expand the file to the new size if needed */
	إذا (tdb->map_ناپ != tdb->transaction->old_map_ناپ) {
		إذا (methods->tdb_expand_file(tdb, tdb->transaction->old_map_ناپ,
					     tdb->map_ناپ -
					     tdb->transaction->old_map_ناپ) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_جندرا, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			أرجع -1;
		}
		tdb->map_ناپ = tdb->transaction->old_map_ناپ;
		methods->tdb_oob(tdb, tdb->map_ناپ + 1, 1);
	}

	/* perform all the writes */
	طالما (tdb->transaction->elements) {
		بنية tdb_transaction_el *el = tdb->transaction->elements;

		إذا (methods->tdb_write(tdb, el->وتھ, el->انکڑے, el->لمائی) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_جندرا, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			أرجع -1;
		}
		tdb->transaction->elements = el->اگلا;
		حرر(el->انکڑے);
		حرر(el);
	}

	إذا (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		إذا (transaction_sync(tdb, 0, tdb->map_ناپ) == -1) {
			أرجع -1;
		}

		/* remove the recovery marker */
		إذا (methods->tdb_write(tdb, magic_وتھ, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			أرجع -1;
		}

		/* ensure the recovery marker has been removed on disk */
		إذا (transaction_sync(tdb, magic_وتھ, 4) == -1) {
			أرجع -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_جندرا, F_UNLCK, F_SETLKW, 0, 1);

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
#جے_تعریف_شدہ HAVE_UTIME
	utime(tdb->name, عدم);
#جے_انت

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
	tdb_off_t recovery_سر, recovery_eof;
	بنانشانی حرف *انکڑے, *p;
	u32 zero = 0;
	بنية list_struct rec;

	/* find the recovery area */
	إذا (tdb_ofs_read(tdb, TDB_RECOVERY_سر, &recovery_سر) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	إذا (recovery_سر == 0) {
		/* we have never allocated a recovery record */
		أرجع 0;
	}

	/* read the recovery record */
	إذا (tdb->methods->tdb_read(tdb, recovery_سر, &rec,
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

	recovery_eof = rec.کلید_len;

	انکڑے = (بنانشانی حرف *)حافظہ_دیو(rec.انکڑے_len);
	إذا (انکڑے == عدم) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	/* read the full recovery data */
	إذا (tdb->methods->tdb_read(tdb, recovery_سر + حجم(rec), انکڑے,
				   rec.انکڑے_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		حرر(انکڑے);
		أرجع -1;
	}

	/* recover the file data */
	p = انکڑے;
	طالما (p+8 < انکڑے + rec.انکڑے_len) {
		u32 ofs, len;
		إذا (DOCONV()) {
			tdb_convert(p, 8);
		}
		حافظہ_نقل(&ofs, p, 4);
		حافظہ_نقل(&len, p+4, 4);

		إذا (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			حرر(انکڑے);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
		p += 8 + len;
	}

	حرر(انکڑے);

	إذا (transaction_sync(tdb, 0, tdb->map_ناپ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	إذا (recovery_eof <= recovery_سر) {
		إذا (tdb_ofs_write(tdb, TDB_RECOVERY_سر, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
	}

	/* remove the recovery magic */
	إذا (tdb_ofs_write(tdb, recovery_سر + offsetof(بنية list_struct, magic),
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
	tdb->map_ناپ = recovery_eof;
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
	إذا (tdb->methods->tdb_oob(tdb, rec->اگلا+حجم(*rec), 0) != 0)
		أرجع -1;
	أرجع 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ساكن صحيح remove_from_freelist(بنية tdb_context *tdb, tdb_off_t off, tdb_off_t اگلا)
{
	tdb_off_t آخری_ptr, i;

	/* read in the freelist top */
	آخری_ptr = FREELIST_چوٹی;
	طالما (tdb_ofs_read(tdb, آخری_ptr, &i) != -1 && i != 0) {
		إذا (i == off) {
			/* We've found it! */
			أرجع tdb_ofs_write(tdb, آخری_ptr, &اگلا);
		}
		/* Follow chain (next offset is at start of record) */
		آخری_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ساكن صحيح update_tailer(بنية tdb_context *tdb, tdb_off_t وتھ,
			 ثابت بنية list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = حجم(*rec) + rec->rec_len;
	أرجع tdb_ofs_write(tdb, وتھ + totalsize - حجم(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
صحيح tdb_free(بنية tdb_context *tdb, tdb_off_t وتھ, بنية list_struct *rec)
{
	tdb_off_t دایاں, بایاں;

	/* Allocation and tailer lock */
	إذا (tdb_جندرا(tdb, -1, F_WRLCK) != 0)
		أرجع -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	إذا (update_tailer(tdb, وتھ, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		اذهب fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	دایاں = وتھ + حجم(*rec) + rec->rec_len;
	إذا (دایاں + حجم(*rec) <= tdb->map_ناپ) {
		بنية list_struct r;

		إذا (tdb->methods->tdb_read(tdb, دایاں, &r, حجم(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", دایاں));
			اذهب بایاں;
		}

		/* If it's free, expand to include it. */
		إذا (r.magic == TDB_FREE_MAGIC) {
			إذا (remove_from_freelist(tdb, دایاں, r.اگلا) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", دایاں));
				اذهب بایاں;
			}
			rec->rec_len += حجم(r) + r.rec_len;
		}
	}

بایاں:
	/* Look left */
	بایاں = وتھ - حجم(tdb_off_t);
	إذا (بایاں > TDB_انکڑے_شروع(tdb->سرخی.hash_ناپ)) {
		بنية list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		إذا (tdb_ofs_read(tdb, بایاں, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", بایاں));
			اذهب update;
		}

		/* it could be uninitialised data */
		إذا (leftsize == 0 || leftsize == TDB_PAD_U32) {
			اذهب update;
		}

		بایاں = وتھ - leftsize;

		/* Now read in record */
		إذا (tdb->methods->tdb_read(tdb, بایاں, &l, حجم(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", بایاں, leftsize));
			اذهب update;
		}

		/* If it's free, expand to include it. */
		إذا (l.magic == TDB_FREE_MAGIC) {
			إذا (remove_from_freelist(tdb, بایاں, l.اگلا) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", بایاں));
				اذهب update;
			} وإلا {
				وتھ = بایاں;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	إذا (update_tailer(tdb, وتھ, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", وتھ));
		اذهب fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	إذا (tdb_ofs_read(tdb, FREELIST_چوٹی, &rec->اگلا) == -1 ||
	    tdb_rec_write(tdb, وتھ, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_چوٹی, &وتھ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", وتھ));
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
ساكن tdb_off_t tdb_allocate_ofs(بنية tdb_context *tdb, tdb_len_t لمائی, tdb_off_t rec_ptr,
				بنية list_struct *rec, tdb_off_t آخری_ptr)
{
	بنية list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', حجم(newrec));

	/* found it - now possibly split it up  */
	إذا (rec->rec_len > لمائی + MIN_REC_ناپ) {
		/* Length of left piece */
		لمائی = TDB_ALIGN(لمائی, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (حجم(*rec) + لمائی);
		newrec_ptr = rec_ptr + حجم(*rec) + لمائی;

		/* And left record is shortened */
		rec->rec_len = لمائی;
	} وإلا {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	إذا (tdb_ofs_write(tdb, آخری_ptr, &rec->اگلا) == -1) {
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
tdb_off_t tdb_allocate(بنية tdb_context *tdb, tdb_len_t لمائی, بنية list_struct *rec)
{
	tdb_off_t rec_ptr, آخری_ptr, newrec_ptr;
	بنية {
		tdb_off_t rec_ptr, آخری_ptr;
		tdb_len_t rec_len;
	} bestfit;

	إذا (tdb_جندرا(tdb, -1, F_WRLCK) == -1)
		أرجع 0;

	/* Extra bytes required for tailer */
	لمائی += حجم(tdb_off_t);

 again:
	آخری_ptr = FREELIST_چوٹی;

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, FREELIST_چوٹی, &rec_ptr) == -1)
		اذهب fail;

	bestfit.rec_ptr = 0;
	bestfit.آخری_ptr = 0;
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

		إذا (rec->rec_len >= لمائی) {
			إذا (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.آخری_ptr = آخری_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				إذا (bestfit.rec_len < 2*لمائی) {
					اكسر;
				}
			}
		}

		/* move to the next record */
		آخری_ptr = rec_ptr;
		rec_ptr = rec->اگلا;
	}

	إذا (bestfit.rec_ptr != 0) {
		إذا (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			اذهب fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, لمائی, bestfit.rec_ptr, rec, bestfit.آخری_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		أرجع newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	إذا (tdb_expand(tdb, لمائی + حجم(*rec)) == 0)
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

ساكن صحيح تکیا_داخل_کرو(بنية tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_انکڑے کلید, انکڑے;

	memset(&انکڑے, '\0', حجم(انکڑے));
	کلید.dptr = (بنانشانی حرف *)&rec_ptr;
	کلید.dsize = حجم(rec_ptr);
	أرجع tdb_store(mem_tdb, کلید, انکڑے, TDB_داخل_کرو);
}

صحيح tdb_validate_freelist(بنية tdb_context *tdb, صحيح *pnum_entries)
{
	بنية tdb_context *mem_tdb = عدم;
	بنية list_struct rec;
	tdb_off_t rec_ptr, آخری_ptr;
	صحيح ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->سرخی.hash_ناپ,
				TDB_اندرونی, O_RDWR, 0600);
	إذا (!mem_tdb) {
		أرجع -1;
	}

	إذا (tdb_جندرا(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		أرجع 0;
	}

	آخری_ptr = FREELIST_چوٹی;

	/* Store the FREELIST_TOP record. */
	إذا (تکیا_داخل_کرو(mem_tdb, آخری_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		اذهب fail;
	}

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, FREELIST_چوٹی, &rec_ptr) == -1) {
		اذهب fail;
	}

	طالما (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		إذا (تکیا_داخل_کرو(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			اذهب fail;
		}

		إذا (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}

		/* move to the next record */
		آخری_ptr = rec_ptr;
		rec_ptr = rec.اگلا;
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
ساكن صحيح tdb_اگلا_جندرا(بنية tdb_context *tdb, بنية tdb_traverse_جندرا *tlock,
			 بنية list_struct *rec)
{
	صحيح want_اگلا = (tlock->off != 0);

	/* Lock each chain from the start one. */
	لكل (; tlock->hash < tdb->سرخی.hash_ناپ; tlock->hash++) {
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
			tdb->methods->اگلا_hash_chain(tdb, &tlock->hash);
			إذا (tlock->hash == tdb->سرخی.hash_ناپ) {
				استمر;
			}
		}

		إذا (tdb_جندرا(tdb, tlock->hash, tlock->جندرا_rw) == -1)
			أرجع -1;

		/* No previous record?  Start at top of chain. */
		إذا (!tlock->off) {
			إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(tlock->hash),
				     &tlock->off) == -1)
				اذهب fail;
		} وإلا {
			/* Otherwise unlock the previous record. */
			إذا (tdb_unlock_ریکارڈ(tdb, tlock->off) != 0)
				اذهب fail;
		}

		إذا (want_اگلا) {
			/* We have offset of old record: grab next */
			إذا (tdb_rec_read(tdb, tlock->off, rec) == -1)
				اذهب fail;
			tlock->off = rec->اگلا;
		}

		/* Iterate through chain */
		طالما( tlock->off) {
			tdb_off_t current;
			إذا (tdb_rec_read(tdb, tlock->off, rec) == -1)
				اذهب fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			إذا (tlock->off == rec->اگلا) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				اذهب fail;
			}

			إذا (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				إذا (tdb_جندرا_ریکارڈ(tdb, tlock->off) != 0)
					اذهب fail;
				أرجع tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->اگلا;
			إذا (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				اذهب fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->جندرا_rw);
		want_اگلا = 0;
	}
	/* We finished iteration without finding anything */
	أرجع TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	إذا (tdb_unlock(tdb, tlock->hash, tlock->جندرا_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	أرجع -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ساكن صحيح tdb_traverse_اندرونی(بنية tdb_context *tdb,
				 tdb_traverse_func fn, فراغ *نجی_انکڑے,
				 بنية tdb_traverse_جندرا *tl)
{
	TDB_انکڑے کلید, dbuf;
	بنية list_struct rec;
	صحيح ret, گنتی = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->اگلا = tdb->travlocks.اگلا;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.اگلا = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	طالما ((ret = tdb_اگلا_جندرا(tdb, tl, &rec)) > 0) {
		گنتی++;
		/* now read the full record */
		کلید.dptr = tdb_alloc_read(tdb, tl->off + حجم(rec),
					  rec.کلید_len + rec.انکڑے_len);
		إذا (!کلید.dptr) {
			ret = -1;
			إذا (tdb_unlock(tdb, tl->hash, tl->جندرا_rw) != 0)
				اذهب باہرلا;
			إذا (tdb_unlock_ریکارڈ(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			اذهب باہرلا;
		}
		کلید.dsize = rec.کلید_len;
		dbuf.dptr = کلید.dptr + rec.کلید_len;
		dbuf.dsize = rec.انکڑے_len;

		/* Drop chain lock, call out */
		إذا (tdb_unlock(tdb, tl->hash, tl->جندرا_rw) != 0) {
			ret = -1;
			محفوظ_FREE(کلید.dptr);
			اذهب باہرلا;
		}
		إذا (fn && fn(tdb, کلید, dbuf, نجی_انکڑے)) {
			/* They want us to terminate traversal */
			ret = گنتی;
			إذا (tdb_unlock_ریکارڈ(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			محفوظ_FREE(کلید.dptr);
			اذهب باہرلا;
		}
		محفوظ_FREE(کلید.dptr);
	}
باہرلا:
	tdb->travlocks.اگلا = tl->اگلا;
	إذا (ret < 0)
		أرجع -1;
	وإلا
		أرجع گنتی;
}


/*
  a write style traverse - temporarily marks the db read only
*/
صحيح tdb_traverse_read(بنية tdb_context *tdb,
		      tdb_traverse_func fn, فراغ *نجی_انکڑے)
{
	بنية tdb_traverse_جندرا tl = { عدم, 0, 0, F_RDLCK };
	صحيح ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	إذا (tdb_transaction_جندرا(tdb, F_RDLCK)) {
		أرجع -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_اندرونی(tdb, fn, نجی_انکڑے, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	أرجع ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
صحيح tdb_traverse(بنية tdb_context *tdb,
		 tdb_traverse_func fn, فراغ *نجی_انکڑے)
{
	بنية tdb_traverse_جندرا tl = { عدم, 0, 0, F_WRLCK };
	صحيح ret;

	إذا (tdb->read_only || tdb->traverse_read) {
		أرجع tdb_traverse_read(tdb, fn, نجی_انکڑے);
	}

	إذا (tdb_transaction_جندرا(tdb, F_WRLCK)) {
		أرجع -1;
	}

	ret = tdb_traverse_اندرونی(tdb, fn, نجی_انکڑے, &tl);

	tdb_transaction_unlock(tdb);

	أرجع ret;
}


/* find the first entry in the database and return its key */
TDB_انکڑے tdb_firstkey(بنية tdb_context *tdb)
{
	TDB_انکڑے کلید;
	بنية list_struct rec;

	/* release any old lock */
	إذا (tdb_unlock_ریکارڈ(tdb, tdb->travlocks.off) != 0)
		أرجع tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.جندرا_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	إذا (tdb_اگلا_جندرا(tdb, &tdb->travlocks, &rec) <= 0)
		أرجع tdb_null;
	/* now read the key */
	کلید.dsize = rec.کلید_len;
	کلید.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),کلید.dsize);

	/* Unlock the hash chain of the record we just read. */
	إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.جندرا_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	أرجع کلید;
}

/* find the next entry in the database, returning its key */
TDB_انکڑے tdb_nextkey(بنية tdb_context *tdb, TDB_انکڑے oldkey)
{
	u32 oldhash;
	TDB_انکڑے کلید = tdb_null;
	بنية list_struct rec;
	بنانشانی حرف *k = عدم;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	إذا (tdb->travlocks.off) {
		إذا (tdb_جندرا(tdb,tdb->travlocks.hash,tdb->travlocks.جندرا_rw))
			أرجع tdb_null;
		إذا (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),
					    rec.کلید_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			إذا (tdb_unlock_ریکارڈ(tdb, tdb->travlocks.off) != 0) {
				محفوظ_FREE(k);
				أرجع tdb_null;
			}
			إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.جندرا_rw) != 0) {
				محفوظ_FREE(k);
				أرجع tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		محفوظ_FREE(k);
	}

	إذا (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_لبھو_جندرا_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.جندرا_rw, &rec);
		إذا (!tdb->travlocks.off)
			أرجع tdb_null;
		tdb->travlocks.hash = بالٹی(rec.بھریا_hash);
		إذا (tdb_جندرا_ریکارڈ(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			أرجع tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	إذا (tdb_اگلا_جندرا(tdb, &tdb->travlocks, &rec) > 0) {
		کلید.dsize = rec.کلید_len;
		کلید.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+حجم(rec),
					  کلید.dsize);
		/* Unlock the chain of this new record */
		إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.جندرا_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	إذا (tdb_unlock(tdb, بالٹی(oldhash), tdb->travlocks.جندرا_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	أرجع کلید;
}

/* file: dump.c */

ساكن tdb_off_t tdb_dump_ریکارڈ(بنية tdb_context *tdb, صحيح hash,
				 tdb_off_t وتھ)
{
	بنية list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	إذا (tdb->methods->tdb_read(tdb, وتھ, (حرف *)&rec,
				   حجم(rec), DOCONV()) == -1) {
		لکھو("ERROR: failed to read record at %u\n", وتھ);
		أرجع 0;
	}

	لکھو(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, وتھ, rec.اگلا, rec.rec_len, rec.کلید_len, rec.انکڑے_len,
	       rec.بھریا_hash, rec.magic);

	tailer_ofs = وتھ + حجم(rec) + rec.rec_len - حجم(tdb_off_t);

	إذا (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		لکھو("ERROR: failed to read tailer at %u\n", tailer_ofs);
		أرجع rec.اگلا;
	}

	إذا (tailer != rec.rec_len + حجم(rec)) {
		لکھو("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(بنانشانی صحيح)tailer, (بنانشانی صحيح)(rec.rec_len + حجم(rec)));
	}
	أرجع rec.اگلا;
}

ساكن صحيح tdb_dump_chain(بنية tdb_context *tdb, صحيح i)
{
	tdb_off_t rec_ptr, چوٹی;

	چوٹی = TDB_HASH_چوٹی(i);

	إذا (tdb_جندرا(tdb, i, F_WRLCK) != 0)
		أرجع -1;

	إذا (tdb_ofs_read(tdb, چوٹی, &rec_ptr) == -1)
		أرجع tdb_unlock(tdb, i, F_WRLCK);

	إذا (rec_ptr)
		لکھو("hash=%d\n", i);

	طالما (rec_ptr) {
		rec_ptr = tdb_dump_ریکارڈ(tdb, i, rec_ptr);
	}

	أرجع tdb_unlock(tdb, i, F_WRLCK);
}

فراغ tdb_dump_all(بنية tdb_context *tdb)
{
	صحيح i;
	لكل (i = 0; i < (صحيح)tdb->سرخی.hash_ناپ; i++) {
		tdb_dump_chain(tdb, i);
	}
	لکھو("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

صحيح tdb_printfreelist(بنية tdb_context *tdb)
{
	صحيح ret;
	لما کل_free = 0;
	tdb_off_t وتھ, rec_ptr;
	بنية list_struct rec;

	إذا ((ret = tdb_جندرا(tdb, -1, F_WRLCK)) != 0)
		أرجع ret;

	وتھ = FREELIST_چوٹی;

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, وتھ, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		أرجع 0;
	}

	لکھو("freelist top=[0x%08x]\n", rec_ptr );
	طالما (rec_ptr) {
		إذا (tdb->methods->tdb_read(tdb, rec_ptr, (حرف *)&rec,
					   حجم(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			أرجع -1;
		}

		إذا (rec.magic != TDB_FREE_MAGIC) {
			لکھو("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			أرجع -1;
		}

		لکھو("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		کل_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.اگلا;
	}
	لکھو("total rec_len = [0x%08x (%d)]\n", (صحيح)کل_free,
               (صحيح)کل_free);

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

ساكن صحيح tdb_کلید_compare(TDB_انکڑے کلید, TDB_انکڑے انکڑے,
			   فراغ *نجی_انکڑے EXT2FS_ATTR((بن_ورتیا)))
{
	أرجع memcmp(انکڑے.dptr, کلید.dptr, انکڑے.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ساكن tdb_off_t tdb_لبھو(بنية tdb_context *tdb, TDB_انکڑے کلید, u32 hash,
			بنية list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		أرجع 0;

	/* keep looking until we find the right record */
	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, r) == -1)
			أرجع 0;

		إذا (!TDB_DEAD(r) && hash==r->بھریا_hash
		    && کلید.dsize==r->کلید_len
		    && tdb_parse_انکڑے(tdb, کلید, rec_ptr + حجم(*r),
				      r->کلید_len, tdb_کلید_compare,
				      عدم) == 0) {
			أرجع rec_ptr;
		}
		rec_ptr = r->اگلا;
	}
	أرجع TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_لبھو_جندرا_hash(بنية tdb_context *tdb, TDB_انکڑے کلید, u32 hash, صحيح locktype,
			   بنية list_struct *rec)
{
	u32 rec_ptr;

	إذا (tdb_جندرا(tdb, بالٹی(hash), locktype) == -1)
		أرجع 0;
	إذا (!(rec_ptr = tdb_لبھو(tdb, کلید, hash, rec)))
		tdb_unlock(tdb, بالٹی(hash), locktype);
	أرجع rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ساكن صحيح tdb_update_hash(بنية tdb_context *tdb, TDB_انکڑے کلید, u32 hash, TDB_انکڑے dbuf)
{
	بنية list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	إذا (!(rec_ptr = tdb_لبھو(tdb, کلید, hash, &rec)))
		أرجع -1;

	/* must be long enough key, data and tailer */
	إذا (rec.rec_len < کلید.dsize + dbuf.dsize + حجم(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		أرجع -1;
	}

	إذا (tdb->methods->tdb_write(tdb, rec_ptr + حجم(rec) + rec.کلید_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		أرجع -1;

	إذا (dbuf.dsize != rec.انکڑے_len) {
		/* update size */
		rec.انکڑے_len = dbuf.dsize;
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
TDB_انکڑے tdb_fetch(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	TDB_انکڑے ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	إذا (!(rec_ptr = tdb_لبھو_جندرا_hash(tdb,کلید,hash,F_RDLCK,&rec)))
		أرجع tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + حجم(rec) + rec.کلید_len,
				  rec.انکڑے_len);
	ret.dsize = rec.انکڑے_len;
	tdb_unlock(tdb, بالٹی(rec.بھریا_hash), F_RDLCK);
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

صحيح tdb_parse_ریکارڈ(بنية tdb_context *tdb, TDB_انکڑے کلید,
		     صحيح (*parser)(TDB_انکڑے کلید, TDB_انکڑے انکڑے,
				   فراغ *نجی_انکڑے),
		     فراغ *نجی_انکڑے)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	صحيح ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);

	إذا (!(rec_ptr = tdb_لبھو_جندرا_hash(tdb,کلید,hash,F_RDLCK,&rec))) {
		أرجع TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_انکڑے(tdb, کلید, rec_ptr + حجم(rec) + rec.کلید_len,
			     rec.انکڑے_len, parser, نجی_انکڑے);

	tdb_unlock(tdb, بالٹی(rec.بھریا_hash), F_RDLCK);

	أرجع ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ساكن صحيح tdb_exists_hash(بنية tdb_context *tdb, TDB_انکڑے کلید, u32 hash)
{
	بنية list_struct rec;

	إذا (tdb_لبھو_جندرا_hash(tdb, کلید, hash, F_RDLCK, &rec) == 0)
		أرجع 0;
	tdb_unlock(tdb, بالٹی(rec.بھریا_hash), F_RDLCK);
	أرجع 1;
}

صحيح tdb_exists(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	أرجع tdb_exists_hash(tdb, کلید, hash);
}

/* actually delete an entry in the database given the offset */
صحيح tdb_do_delete(بنية tdb_context *tdb, tdb_off_t rec_ptr, بنية list_struct*rec)
{
	tdb_off_t آخری_ptr, i;
	بنية list_struct lastrec;

	إذا (tdb->read_only || tdb->traverse_read) أرجع -1;

	إذا (tdb_write_جندرا_ریکارڈ(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		أرجع tdb_rec_write(tdb, rec_ptr, rec);
	}
	إذا (tdb_write_unlock_ریکارڈ(tdb, rec_ptr) != 0)
		أرجع -1;

	/* find previous record in hash chain */
	إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(rec->بھریا_hash), &i) == -1)
		أرجع -1;
	لكل (آخری_ptr = 0; i != rec_ptr; آخری_ptr = i, i = lastrec.اگلا)
		إذا (tdb_rec_read(tdb, i, &lastrec) == -1)
			أرجع -1;

	/* unlink it: next ptr is at start of record. */
	إذا (آخری_ptr == 0)
		آخری_ptr = TDB_HASH_چوٹی(rec->بھریا_hash);
	إذا (tdb_ofs_write(tdb, آخری_ptr, &rec->اگلا) == -1)
		أرجع -1;

	/* recover the space */
	إذا (tdb_free(tdb, rec_ptr, rec) == -1)
		أرجع -1;
	أرجع 0;
}

ساكن صحيح tdb_گنتی_dead(بنية tdb_context *tdb, u32 hash)
{
	صحيح res = 0;
	tdb_off_t rec_ptr;
	بنية list_struct rec;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		أرجع 0;

	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			أرجع 0;

		إذا (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.اگلا;
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

	إذا (tdb_جندرا(tdb, -1, F_WRLCK) == -1) {
		أرجع -1;
	}

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		اذهب fail;

	طالما (rec_ptr) {
		tdb_off_t اگلا;

		إذا (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}

		اگلا = rec.اگلا;

		إذا (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}
		rec_ptr = اگلا;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع res;
}

/* delete an entry in the database given a key */
ساكن صحيح tdb_delete_hash(بنية tdb_context *tdb, TDB_انکڑے کلید, u32 hash)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	صحيح ret;

	إذا (tdb->max_dead_ریکارڈاں != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		إذا (tdb_جندرا(tdb, بالٹی(hash), F_WRLCK) == -1)
			أرجع -1;

		إذا (tdb_گنتی_dead(tdb, hash) >= tdb->max_dead_ریکارڈاں) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		إذا (!(rec_ptr = tdb_لبھو(tdb, کلید, hash, &rec))) {
			tdb_unlock(tdb, بالٹی(hash), F_WRLCK);
			أرجع -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	وإلا {
		إذا (!(rec_ptr = tdb_لبھو_جندرا_hash(tdb, کلید, hash, F_WRLCK,
						   &rec)))
			أرجع -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	إذا (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	إذا (tdb_unlock(tdb, بالٹی(rec.بھریا_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	أرجع ret;
}

صحيح tdb_delete(بنية tdb_context *tdb, TDB_انکڑے کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	أرجع tdb_delete_hash(tdb, کلید, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ساكن tdb_off_t tdb_لبھو_dead(بنية tdb_context *tdb, u32 hash,
			       بنية list_struct *r, tdb_len_t لمائی)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		أرجع 0;

	/* keep looking until we find the right record */
	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, r) == -1)
			أرجع 0;

		إذا (TDB_DEAD(r) && r->rec_len >= لمائی) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			أرجع rec_ptr;
		}
		rec_ptr = r->اگلا;
	}
	أرجع 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
صحيح tdb_store(بنية tdb_context *tdb, TDB_انکڑے کلید, TDB_انکڑے dbuf, صحيح flag)
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
	hash = tdb->hash_fn(&کلید);
	إذا (tdb_جندرا(tdb, بالٹی(hash), F_WRLCK) == -1)
		أرجع -1;

	/* check for it existing, on insert. */
	إذا (flag == TDB_داخل_کرو) {
		إذا (tdb_exists_hash(tdb, کلید, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			اذهب fail;
		}
	} وإلا {
		/* first try in-place update, on modify or replace. */
		إذا (tdb_update_hash(tdb, کلید, hash, dbuf) == 0) {
			اذهب پورا;
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
	إذا (flag != TDB_داخل_کرو)
		tdb_delete_hash(tdb, کلید, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	إذا (!(p = (حرف *)حافظہ_دیو(کلید.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب fail;
	}

	حافظہ_نقل(p, کلید.dptr, کلید.dsize);
	إذا (dbuf.dsize)
		حافظہ_نقل(p+کلید.dsize, dbuf.dptr, dbuf.dsize);

	إذا (tdb->max_dead_ریکارڈاں != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_لبھو_dead(
			tdb, hash, &rec,
			کلید.dsize + dbuf.dsize + حجم(tdb_off_t));

		إذا (rec_ptr != 0) {
			rec.کلید_len = کلید.dsize;
			rec.انکڑے_len = dbuf.dsize;
			rec.بھریا_hash = hash;
			rec.magic = TDB_MAGIC;
			إذا (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + حجم(rec),
				    p, کلید.dsize + dbuf.dsize) == -1) {
				اذهب fail;
			}
			اذهب پورا;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	إذا (tdb_جندرا(tdb, -1, F_WRLCK) == -1) {
		اذهب fail;
	}

	إذا ((tdb->max_dead_ریکارڈاں != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		اذهب fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, کلید.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	إذا (rec_ptr == 0) {
		اذهب fail;
	}

	/* Read hash top into next ptr */
	إذا (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec.اگلا) == -1)
		اذهب fail;

	rec.کلید_len = کلید.dsize;
	rec.انکڑے_len = dbuf.dsize;
	rec.بھریا_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	إذا (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+حجم(rec), p, کلید.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		اذهب fail;
	}

 پورا:
	ret = 0;
 fail:
	إذا (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	محفوظ_FREE(p);
	tdb_unlock(tdb, بالٹی(hash), F_WRLCK);
	أرجع ret;
}


/* Append to an entry. Create if not exist. */
صحيح tdb_append(بنية tdb_context *tdb, TDB_انکڑے کلید, TDB_انکڑے new_dbuf)
{
	u32 hash;
	TDB_انکڑے dbuf;
	صحيح ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	إذا (tdb_جندرا(tdb, بالٹی(hash), F_WRLCK) == -1)
		أرجع -1;

	dbuf = tdb_fetch(tdb, کلید);

	إذا (dbuf.dptr == عدم) {
		dbuf.dptr = (بنانشانی حرف *)حافظہ_دیو(new_dbuf.dsize);
	} وإلا {
		بنانشانی حرف *new_dptr = (بنانشانی حرف *)realloc(dbuf.dptr,
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

	حافظہ_نقل(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, کلید, dbuf, 0);

failed:
	tdb_unlock(tdb, بالٹی(hash), F_WRLCK);
	محفوظ_FREE(dbuf.dptr);
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
	أرجع tdb->لوگارتھم.log_fn;
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

صحيح tdb_hash_ناپ(بنية tdb_context *tdb)
{
	أرجع tdb->سرخی.hash_ناپ;
}

size_t tdb_map_ناپ(بنية tdb_context *tdb)
{
	أرجع tdb->map_ناپ;
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
ساكن بنانشانی صحيح default_tdb_hash(TDB_انکڑے *کلید)
{
	u32 قدر;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	لكل (قدر = 0, i=0; i < کلید->dsize; i++)
		قدر = قدر * 256 + کلید->dptr[i] + (قدر >> 24) * 241;

	أرجع قدر;
}


/* initialise a new database with a specified hash size */
ساكن صحيح tdb_new_database(بنية tdb_context *tdb, صحيح hash_ناپ)
{
	بنية tdb_سرخی *newdb;
	صحيح ناپ, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	ناپ = حجم(بنية tdb_سرخی) + (hash_ناپ+1)*حجم(tdb_off_t);
	إذا (!(newdb = (بنية tdb_سرخی *)صفر_حافظہ_دیو(ناپ, 1)))
		أرجع TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->نسخہ = TDB_نسخہ;
	newdb->hash_ناپ = hash_ناپ;
	إذا (tdb->flags & TDB_اندرونی) {
		tdb->map_ناپ = ناپ;
		tdb->map_ptr = (حرف *)newdb;
		حافظہ_نقل(&tdb->سرخی, newdb, حجم(tdb->سرخی));
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
	حافظہ_نقل(&tdb->سرخی, newdb, حجم(tdb->سرخی));
	/* Don't endian-convert the magic food! */
	حافظہ_نقل(newdb->magic_food, TDB_MAGIC_FOOD, لڑی_لمبائی(TDB_MAGIC_FOOD)+1);
	إذا (اكتب(tdb->fd, newdb, ناپ) != ناپ) {
		ret = -1;
	} وإلا {
		ret = 0;
	}

  fail:
	محفوظ_FREE(newdb);
	أرجع ret;
}



ساكن صحيح tdb_already_open(dev_t device,
			    ino_t ino)
{
	بنية tdb_context *i;

	لكل (i = tdbs; i; i = i->اگلا) {
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
بنية tdb_context *tdb_open(ثابت حرف *name, صحيح hash_ناپ, صحيح tdb_flags,
		      صحيح open_flags, mode_t منوال)
{
	أرجع tdb_open_ex(name, hash_ناپ, tdb_flags, open_flags, منوال, عدم, عدم);
}

/* a default logging function */
ساكن فراغ null_log_fn(بنية tdb_context *tdb, تعداد tdb_debug_level level, ثابت حرف *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ساكن فراغ null_log_fn(بنية tdb_context *tdb EXT2FS_ATTR((بن_ورتیا)),
			تعداد tdb_debug_level level EXT2FS_ATTR((بن_ورتیا)),
			ثابت حرف *fmt EXT2FS_ATTR((بن_ورتیا)), ...)
{
}


بنية tdb_context *tdb_open_ex(ثابت حرف *name, صحيح hash_ناپ, صحيح tdb_flags,
				صحيح open_flags, mode_t منوال,
				ثابت بنية tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	بنية tdb_context *tdb;
	بنية stat st;
	صحيح rev = 0, locked = 0;
	بنانشانی حرف *vp;
	u32 vertest;

	إذا (!(tdb = (بنية tdb_context *)صفر_حافظہ_دیو(1, حجم *tdb))) {
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
		tdb->لوگارتھم = *log_ctx;
	} وإلا {
		tdb->لوگارتھم.log_fn = null_log_fn;
		tdb->لوگارتھم.log_نجی = عدم;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->ورقہ_ناپ = sysconf(_SC_PAGESIZE);
	إذا (tdb->ورقہ_ناپ <= 0) {
		tdb->ورقہ_ناپ = 0x2000;
	}

	إذا ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		اذهب fail;
	}

	إذا (hash_ناپ == 0)
		hash_ناپ = DEFAULT_HASH_ناپ;
	إذا ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_پہلا;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	إذا (tdb->flags & TDB_اندرونی) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_پہلا;
		إذا (tdb_new_database(tdb, hash_ناپ) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			اذهب fail;
		}
		اذهب اندرونی;
	}

	إذا ((tdb->fd = open(name, open_flags, منوال)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		اذهب fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	إذا (tdb->methods->tdb_brlock(tdb, GLOBAL_جندرا, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		اذهب fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	إذا ((tdb_flags & TDB_CLEAR_IF_پہلا) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, سرگرم_جندرا, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		إذا (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			اذهب fail; /* errno set by ftruncate */
		}
	}

	إذا (اقرأ(tdb->fd, &tdb->سرخی, حجم(tdb->سرخی)) != حجم(tdb->سرخی)
	    || memcmp(tdb->سرخی.magic_food, TDB_MAGIC_FOOD,
		      حجم(TDB_MAGIC_FOOD)) != 0
	    || (tdb->سرخی.نسخہ != TDB_نسخہ
		&& !(rev = (tdb->سرخی.نسخہ==TDB_BYTEREV(TDB_نسخہ))))) {
		/* its not a valid database - possibly initialise it */
		إذا (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_ناپ) == -1) {
			errno = EIO; /* ie bad format or something */
			اذهب fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (بنانشانی حرف *)&tdb->سرخی.نسخہ;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_نسخہ) ? TDB_BIGENDIAN : 0;
	إذا (!rev)
		tdb->flags &= ~TDB_CONVERT;
	وإلا {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->سرخی, حجم(tdb->سرخی));
	}
	إذا (fstat(tdb->fd, &st) == -1)
		اذهب fail;

	إذا (tdb->سرخی.rwlocks != 0) {
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

	tdb->map_ناپ = st.st_ناپ;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_ریکارڈاں = 0;
	tdb_mmap(tdb);
	إذا (locked) {
		إذا (tdb->methods->tdb_brlock(tdb, سرگرم_جندرا, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			اذهب fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	إذا (tdb_flags & TDB_CLEAR_IF_پہلا) {
		/* leave this lock in place to indicate it's in use */
		إذا (tdb->methods->tdb_brlock(tdb, سرگرم_جندرا, F_RDLCK, F_SETLKW, 0, 1) == -1)
			اذهب fail;
	}

	/* if needed, run recovery */
	إذا (tdb_transaction_recover(tdb) == -1) {
		اذهب fail;
	}

 اندرونی:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	إذا (tdb->methods->tdb_brlock(tdb, GLOBAL_جندرا, F_UNLCK, F_SETLKW, 0, 1) == -1)
		اذهب fail;
	tdb->اگلا = tdbs;
	tdbs = tdb;
	أرجع tdb;

 fail:
	{ صحيح save_errno = errno;

	إذا (!tdb)
		أرجع عدم;

	إذا (tdb->map_ptr) {
		إذا (tdb->flags & TDB_اندرونی)
			محفوظ_FREE(tdb->map_ptr);
		وإلا
			tdb_munmap(tdb);
	}
	محفوظ_FREE(tdb->name);
	إذا (tdb->fd != -1)
		إذا (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	محفوظ_FREE(tdb);
	errno = save_errno;
	أرجع عدم;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

فراغ tdb_سیٹ_max_dead(بنية tdb_context *tdb, صحيح max_dead)
{
	tdb->max_dead_ریکارڈاں = max_dead;
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
		إذا (tdb->flags & TDB_اندرونی)
			محفوظ_FREE(tdb->map_ptr);
		وإلا
			tdb_munmap(tdb);
	}
	محفوظ_FREE(tdb->name);
	إذا (tdb->fd != -1)
		ret = close(tdb->fd);
	محفوظ_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	لكل (i = &tdbs; *i; i = &(*i)->اگلا) {
		إذا (*i == tdb) {
			*i = tdb->اگلا;
			اكسر;
		}
	}

	memset(tdb, 0, حجم(*tdb));
	محفوظ_FREE(tdb);

	أرجع ret;
}

/* register a logging function */
فراغ tdb_سیٹ_logging_function(بنية tdb_context *tdb,
                              ثابت بنية tdb_logging_context *log_ctx)
{
        tdb->لوگارتھم = *log_ctx;
}

فراغ *tdb_get_logging_نجی(بنية tdb_context *tdb)
{
	أرجع tdb->لوگارتھم.log_نجی;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
صحيح tdb_reopen(بنية tdb_context *tdb)
{
	بنية stat st;

	إذا (tdb->flags & TDB_اندرونی) {
		أرجع 0; /* Nothing to do. */
	}

	إذا (tdb->عدد_locks != 0 || tdb->global_جندرا.گنتی) {
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
	إذا ((tdb->flags & TDB_CLEAR_IF_پہلا) &&
	    (tdb->methods->tdb_brlock(tdb, سرگرم_جندرا, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
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
صحيح tdb_reopen_all(صحيح ماپا_longlived)
{
	بنية tdb_context *tdb;

	لكل (tdb=tdbs; tdb; tdb = tdb->اگلا) {
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
		إذا (ماپا_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_پہلا;
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
