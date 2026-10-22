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

#اگر_تعریف_شده CONFIG_STAND_ALONE
#تعریف_کن HAVE_MMAP
#تعریف_کن HAVE_STRDUP
#تعریف_کن HAVE_SYS_MMAN_H
#تعریف_کن HAVE_UTIME_H
#تعریف_کن HAVE_UTIME
#پایان_اگر
#اگر_تعریف_نشده __FreeBSD__
#تعریف_کن XOPEN_منبع 600
#پایان_اگر

#ضمن "config.h"
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <stdarg.h>
#ضمن <stddef.h>
#ضمن <errno.h>
#ضمن <string.h>
#اگر_تعریف_شده HAVE_SYS_SELECT_H
#ضمن <sys/select.h>
#پایان_اگر
#ضمن <sys/time.h>
#ضمن <sys/types.h>
#ضمن <time.h>
#اگر_تعریف_شده HAVE_UTIME_H
#ضمن <utime.h>
#پایان_اگر
#ضمن <sys/stat.h>
#ضمن <sys/file.h>
#ضمن <fcntl.h>

#اگر_تعریف_شده HAVE_SYS_MMAN_H
#ضمن <sys/mman.h>
#پایان_اگر

#اگر_تعریف_شده __GNUC__
#تعریف_کن EXT2FS_ATTR(x) __attribute__(x)
#وإلا
#تعریف_کن EXT2FS_ATTR(x)
#پایان_اگر

#اگر_تعریف_نشده MAP_FILE
#تعریف_کن MAP_FILE 0
#پایان_اگر

#اگر_تعریف_نشده MAP_FAILED
#تعریف_کن MAP_FAILED ((فراغ *)-1)
#پایان_اگر

#اگر_تعریف_نشده HAVE_STRDUP
#تعریف_کن strdup rep_strdup
ساكن حرف *rep_strdup(ثابت حرف *s)
{
	حرف *ret;
	صحيح درازی;

	إذا (!s)
		أرجع عدم;
	درازی = طول_رشته(s);
	ret = حافظه_بده(درازی + 1);
	إذا (ret) {
		strncpy(ret, s, درازی);
		ret[درازی] = '\0';
	}
	أرجع ret;
}
#پایان_اگر

#اگر_تعریف_نشده PRINTF_ATTRIBUTE
#إذا (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#تعریف_کن PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وإلا
#تعریف_کن PRINTF_ATTRIBUTE(a1, a2)
#پایان_اگر
#پایان_اگر

#إذا defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
تعريفنوع صحيح bool;
#پایان_اگر

#ضمن "tdb.h"

ساكن TDB_داده tdb_null;

#اگر_تعریف_نشده u32
#تعریف_کن u32 بیعلامت
#پایان_اگر

تعريفنوع u32 tdb_len_t;
تعريفنوع u32 tdb_off_t;

#اگر_تعریف_نشده offsetof
#تعریف_کن offsetof(t,f) ((بیعلامت صحيح)&((t *)0)->f)
#پایان_اگر

#تعریف_کن TDB_MAGIC_FOOD "TDB file\n"
#تعریف_کن TDB_نسخه (0x26011967 + 6)
#تعریف_کن TDB_MAGIC (0x26011999U)
#تعریف_کن TDB_FREE_MAGIC (~TDB_MAGIC)
#تعریف_کن TDB_DEAD_MAGIC (0xFEE1DEAD)
#تعریف_کن TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#تعریف_کن TDB_ALIGNMENT 4
#تعریف_کن MIN_REC_اندازه (2*حجم(بنية list_struct) + TDB_ALIGNMENT)
#تعریف_کن DEFAULT_HASH_اندازه 131
#تعریف_کن FREELIST_بالاترین (حجم(بنية tdb_سرآیند))
#تعریف_کن TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#تعریف_کن TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#تعریف_کن TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#تعریف_کن TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#تعریف_کن TDB_HASH_بالاترین(hash) (FREELIST_بالاترین + (سطل(hash)+1)*حجم(tdb_off_t))
#تعریف_کن TDB_HASHTABLE_اندازه(tdb) ((tdb->سرآیند.hash_اندازه+1)*حجم(tdb_off_t))
#تعریف_کن TDB_داده_شروع(hash_اندازه) TDB_HASH_بالاترین(hash_اندازه-1)
#تعریف_کن TDB_RECOVERY_سر offsetof(بنية tdb_سرآیند, recovery_شروع)
#تعریف_کن TDB_SEQNUM_OFS    offsetof(بنية tdb_سرآیند, sequence_عدد)
#تعریف_کن TDB_PAD_BYTE 0x42
#تعریف_کن TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#تعریف_کن TDB_LOG(x) tdb->لگاریتم.log_fn x

/* lock offsets */
#تعریف_کن GLOBAL_قفل      0
#تعریف_کن فعال_قفل      4
#تعریف_کن TRANSACTION_قفل 8

/* free memory if the pointer is valid and zero the pointer */
#اگر_تعریف_نشده امن_FREE
#تعریف_کن امن_FREE(x) افعل { إذا ((x) != عدم) {حرر(x); (x)=عدم;} } طالما(0)
#پایان_اگر

#تعریف_کن سطل(hash) ((hash) % tdb->سرآیند.hash_اندازه)

#تعریف_کن DOCONV() (tdb->flags & TDB_CONVERT)
#تعریف_کن CONVERT(x) (DOCONV() ? tdb_convert(&x, حجم(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
بنية list_struct {
	tdb_off_t بعدی; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t کلید_len; /* byte length of key */
	tdb_len_t داده_len; /* byte length of data */
	u32 پر_hash; /* the full 32 bit hash of the key */
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
بنية tdb_سرآیند {
	حرف magic_food[32]; /* for /etc/magic */
	u32 نسخه; /* version of the code */
	u32 hash_اندازه; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_شروع; /* offset of transaction recovery region */
	tdb_off_t sequence_عدد; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

بنية tdb_قفل_type {
	صحيح list;
	u32 شمار;
	u32 ltype;
};

بنية tdb_traverse_قفل {
	بنية tdb_traverse_قفل *بعدی;
	u32 off;
	u32 hash;
	صحيح قفل_rw;
};


بنية tdb_methods {
	صحيح (*tdb_read)(بنية tdb_context *, tdb_off_t , فراغ *, tdb_len_t , صحيح );
	صحيح (*tdb_write)(بنية tdb_context *, tdb_off_t, ثابت فراغ *, tdb_len_t);
	فراغ (*بعدی_hash_chain)(بنية tdb_context *, u32 *);
	صحيح (*tdb_oob)(بنية tdb_context *, tdb_off_t , صحيح );
	صحيح (*tdb_expand_file)(بنية tdb_context *, tdb_off_t , tdb_off_t );
	صحيح (*tdb_brlock)(بنية tdb_context *, tdb_off_t , صحيح, صحيح, صحيح, size_t);
};

بنية tdb_context {
	حرف *name; /* the name of the database */
	فراغ *map_ptr; /* where it is currently mapped */
	صحيح fd; /* open file descriptor for the database */
	tdb_len_t map_اندازه; /* how much space has been mapped */
	صحيح read_only; /* opened read-only */
	صحيح traverse_read; /* read-only traversal */
	بنية tdb_قفل_type global_قفل;
	صحيح رقم_lockrecs;
	بنية tdb_قفل_type *lockrecs; /* only real locks, all with count>0 */
	تعداد TDB_ERROR ecode; /* error code for last tdb error */
	بنية tdb_سرآیند سرآیند; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	بنية tdb_traverse_قفل travlocks; /* current traversal locks */
	بنية tdb_context *بعدی; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	بنية tdb_logging_context لگاریتم;
	بیعلامت صحيح (*hash_fn)(TDB_داده *کلید);
	صحيح open_flags; /* flags used in the open - needed by reopen */
	بیعلامت صحيح رقم_locks; /* number of chain locks held */
	ثابت بنية tdb_methods *methods;
	بنية tdb_transaction *transaction;
	صحيح صفحه_اندازه;
	صحيح max_dead_رکوردها;
	bool have_transaction_قفل;
	tdb_len_t real_map_اندازه; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ساكن صحيح tdb_munmap(بنية tdb_context *tdb);
ساكن فراغ tdb_mmap(بنية tdb_context *tdb);
ساكن صحيح tdb_قفل(بنية tdb_context *tdb, صحيح list, صحيح ltype);
صحيح tdb_قفل_nonblock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
ساكن صحيح tdb_unlock(بنية tdb_context *tdb, صحيح list, صحيح ltype);
ساكن صحيح tdb_brlock(بنية tdb_context *tdb, tdb_off_t جابه_جایی, صحيح rw_type, صحيح lck_type, صحيح probe, size_t len);
ساكن صحيح tdb_transaction_قفل(بنية tdb_context *tdb, صحيح ltype);
ساكن صحيح tdb_transaction_unlock(بنية tdb_context *tdb);
ساكن صحيح tdb_brlock_upgrade(بنية tdb_context *tdb, tdb_off_t جابه_جایی, size_t len);
ساكن صحيح tdb_write_قفل_رکورد(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_write_unlock_رکورد(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_off_t *d);
ساكن صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_off_t *d);
ساكن فراغ *tdb_convert(فراغ *buf, u32 اندازه);
ساكن صحيح tdb_free(بنية tdb_context *tdb, tdb_off_t جابه_جایی, بنية list_struct *rec);
ساكن tdb_off_t tdb_allocate(بنية tdb_context *tdb, tdb_len_t درازی, بنية list_struct *rec);
ساكن صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_off_t *d);
ساكن صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_off_t *d);
ساكن صحيح tdb_قفل_رکورد(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_unlock_رکورد(بنية tdb_context *tdb, tdb_off_t off);
ساكن صحيح tdb_rec_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, بنية list_struct *rec);
ساكن صحيح tdb_rec_write(بنية tdb_context *tdb, tdb_off_t جابه_جایی, بنية list_struct *rec);
ساكن صحيح tdb_do_delete(بنية tdb_context *tdb, tdb_off_t rec_ptr, بنية list_struct *rec);
ساكن بیعلامت حرف *tdb_alloc_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_len_t len);
ساكن صحيح tdb_parse_داده(بنية tdb_context *tdb, TDB_داده کلید,
		   tdb_off_t جابه_جایی, tdb_len_t len,
		   صحيح (*parser)(TDB_داده کلید, TDB_داده داده,
				 فراغ *خصوصی_داده),
		   فراغ *خصوصی_داده);
ساكن tdb_off_t tdb_پیدا_کن_قفل_hash(بنية tdb_context *tdb, TDB_داده کلید, u32 hash, صحيح locktype,
			   بنية list_struct *rec);
ساكن فراغ tdb_io_init(بنية tdb_context *tdb);
ساكن صحيح tdb_expand(بنية tdb_context *tdb, tdb_off_t اندازه);
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
	     {TDB_ERR_قفل, "Locking error"},
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

#تعریف_کن TDB_MARK_قفل 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
صحيح tdb_brlock(بنية tdb_context *tdb, tdb_off_t جابه_جایی,
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
	fl.l_شروع = جابه_جایی;
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
			tdb->ecode = TDB_ERR_قفل;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, جابه_جایی, rw_type, lck_type, (صحيح)len));
		}
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}
	أرجع 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
صحيح tdb_brlock_upgrade(بنية tdb_context *tdb, tdb_off_t جابه_جایی, size_t len)
{
	صحيح شمار = 1000;
	طالما (شمار--) {
		بنية timeval tv;
		إذا (tdb_brlock(tdb, جابه_جایی, F_WRLCK, F_SETLKW, 1, len) == 0) {
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
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", جابه_جایی));
	أرجع -1;
}


/* lock a list in the database. list -1 is the alloc list */
ساكن صحيح tdb_قفل_2(بنية tdb_context *tdb, صحيح list, صحيح ltype, صحيح op)
{
	بنية tdb_قفل_type *new_lck;
	صحيح i;
	bool mark_قفل = ((ltype & TDB_MARK_قفل) == TDB_MARK_قفل);

	ltype &= ~TDB_MARK_قفل;

	/* a global lock allows us to avoid per chain locks */
	إذا (tdb->global_قفل.شمار &&
	    ((u32)ltype == tdb->global_قفل.ltype || ltype == F_RDLCK)) {
		أرجع 0;
	}

	إذا (tdb->global_قفل.شمار) {
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}

	إذا (list < -1 || list >= (صحيح)tdb->سرآیند.hash_اندازه) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		أرجع -1;
	}
	إذا (tdb->flags & TDB_NOLOCK)
		أرجع 0;

	لكل (i=0; i<tdb->رقم_lockrecs; i++) {
		إذا (tdb->lockrecs[i].list == list) {
			إذا (tdb->lockrecs[i].شمار == 0) {
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
			tdb->lockrecs[i].شمار++;
			أرجع 0;
		}
	}

	new_lck = (بنية tdb_قفل_type *)realloc(
		tdb->lockrecs,
		حجم(*tdb->lockrecs) * (tdb->رقم_lockrecs+1));
	إذا (new_lck == عدم) {
		errno = ENOMEM;
		أرجع -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	إذا (!mark_قفل &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_بالاترین+4*list, ltype, op,
				     0, 1)) {
		أرجع -1;
	}

	tdb->رقم_locks++;

	tdb->lockrecs[tdb->رقم_lockrecs].list = list;
	tdb->lockrecs[tdb->رقم_lockrecs].شمار = 1;
	tdb->lockrecs[tdb->رقم_lockrecs].ltype = ltype;
	tdb->رقم_lockrecs += 1;

	أرجع 0;
}

/* lock a list in the database. list -1 is the alloc list */
صحيح tdb_قفل(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	صحيح ret;
	ret = tdb_قفل_2(tdb, list, ltype, F_SETLKW);
	إذا (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	أرجع ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
صحيح tdb_قفل_nonblock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	أرجع tdb_قفل_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
صحيح tdb_unlock(بنية tdb_context *tdb, صحيح list, صحيح ltype)
{
	صحيح ret = -1;
	صحيح i;
	بنية tdb_قفل_type *lck = عدم;
	bool mark_قفل = ((ltype & TDB_MARK_قفل) == TDB_MARK_قفل);

	ltype &= ~TDB_MARK_قفل;

	/* a global lock allows us to avoid per chain locks */
	إذا (tdb->global_قفل.شمار &&
	    ((u32)ltype == tdb->global_قفل.ltype || ltype == F_RDLCK)) {
		أرجع 0;
	}

	إذا (tdb->global_قفل.شمار) {
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}

	إذا (tdb->flags & TDB_NOLOCK)
		أرجع 0;

	/* Sanity checks */
	إذا (list < -1 || list >= (صحيح)tdb->سرآیند.hash_اندازه) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->سرآیند.hash_اندازه));
		أرجع ret;
	}

	لكل (i=0; i<tdb->رقم_lockrecs; i++) {
		إذا (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			اكسر;
		}
	}

	إذا ((lck == عدم) || (lck->شمار == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		أرجع -1;
	}

	إذا (lck->شمار > 1) {
		lck->شمار--;
		أرجع 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	إذا (mark_قفل) {
		ret = 0;
	} وإلا {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_بالاترین+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->رقم_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	إذا (tdb->رقم_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->رقم_lockrecs-1];
	}
	tdb->رقم_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	إذا (tdb->رقم_lockrecs == 0) {
		امن_FREE(tdb->lockrecs);
	}

	إذا (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	أرجع ret;
}

/*
  get the transaction lock
 */
صحيح tdb_transaction_قفل(بنية tdb_context *tdb, صحيح ltype)
{
	إذا (tdb->have_transaction_قفل || tdb->global_قفل.شمار) {
		أرجع 0;
	}
	إذا (tdb->methods->tdb_brlock(tdb, TRANSACTION_قفل, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_قفل;
		أرجع -1;
	}
	tdb->have_transaction_قفل = 1;
	أرجع 0;
}

/*
  release the transaction lock
 */
صحيح tdb_transaction_unlock(بنية tdb_context *tdb)
{
	صحيح ret;
	إذا (!tdb->have_transaction_قفل) {
		أرجع 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_قفل, F_UNLCK, F_SETLKW, 0, 1);
	إذا (ret == 0) {
		tdb->have_transaction_قفل = 0;
	}
	أرجع ret;
}




/* lock/unlock entire database */
ساكن صحيح _tdb_lockall(بنية tdb_context *tdb, صحيح ltype, صحيح op)
{
	bool mark_قفل = ((ltype & TDB_MARK_قفل) == TDB_MARK_قفل);

	ltype &= ~TDB_MARK_قفل;

	/* There are no locks on read-only dbs */
	إذا (tdb->read_only || tdb->traverse_read)
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);

	إذا (tdb->global_قفل.شمار && tdb->global_قفل.ltype == (u32)ltype) {
		tdb->global_قفل.شمار++;
		أرجع 0;
	}

	إذا (tdb->global_قفل.شمار) {
		/* a global lock of a different type exists */
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}

	إذا (tdb->رقم_locks != 0) {
		/* can't combine global and chain locks */
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}

	إذا (!mark_قفل &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_بالاترین, ltype, op,
				     0, 4*tdb->سرآیند.hash_اندازه)) {
		إذا (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		أرجع -1;
	}

	tdb->global_قفل.شمار = 1;
	tdb->global_قفل.ltype = ltype;

	أرجع 0;
}



/* unlock entire db */
ساكن صحيح _tdb_unlockall(بنية tdb_context *tdb, صحيح ltype)
{
	bool mark_قفل = ((ltype & TDB_MARK_قفل) == TDB_MARK_قفل);

	ltype &= ~TDB_MARK_قفل;

	/* There are no locks on read-only dbs */
	إذا (tdb->read_only || tdb->traverse_read) {
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}

	إذا (tdb->global_قفل.ltype != (u32)ltype ||
	    tdb->global_قفل.شمار == 0) {
		أرجع TDB_ERRCODE(TDB_ERR_قفل, -1);
	}

	إذا (tdb->global_قفل.شمار > 1) {
		tdb->global_قفل.شمار--;
		أرجع 0;
	}

	إذا (!mark_قفل &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW,
				     0, 4*tdb->سرآیند.hash_اندازه)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		أرجع -1;
	}

	tdb->global_قفل.شمار = 0;
	tdb->global_قفل.ltype = 0;

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
	أرجع _tdb_lockall(tdb, F_WRLCK | TDB_MARK_قفل, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
صحيح tdb_lockall_unmark(بنية tdb_context *tdb)
{
	أرجع _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_قفل);
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
صحيح tdb_chainlock(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_قفل(tdb, سطل(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
صحيح tdb_chainlock_nonblock(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_قفل_nonblock(tdb, سطل(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
صحيح tdb_chainlock_mark(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_قفل(tdb, سطل(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_قفل);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
صحيح tdb_chainlock_unmark(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_unlock(tdb, سطل(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_قفل);
}

صحيح tdb_chainunlock(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_unlock(tdb, سطل(tdb->hash_fn(&کلید)), F_WRLCK);
}

صحيح tdb_chainlock_read(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_قفل(tdb, سطل(tdb->hash_fn(&کلید)), F_RDLCK);
}

صحيح tdb_chainunlock_read(بنية tdb_context *tdb, TDB_داده کلید)
{
	أرجع tdb_unlock(tdb, سطل(tdb->hash_fn(&کلید)), F_RDLCK);
}



/* record lock stops delete underneath */
صحيح tdb_قفل_رکورد(بنية tdb_context *tdb, tdb_off_t off)
{
	أرجع off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحيح tdb_write_قفل_رکورد(بنية tdb_context *tdb, tdb_off_t off)
{
	بنية tdb_traverse_قفل *i;
	لكل (i = &tdb->travlocks; i; i = i->بعدی)
		إذا (i->off == off)
			أرجع -1;
	أرجع tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحيح tdb_write_unlock_رکورد(بنية tdb_context *tdb, tdb_off_t off)
{
	أرجع tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
صحيح tdb_unlock_رکورد(بنية tdb_context *tdb, tdb_off_t off)
{
	بنية tdb_traverse_قفل *i;
	u32 شمار = 0;

	إذا (off == 0)
		أرجع 0;
	لكل (i = &tdb->travlocks; i; i = i->بعدی)
		إذا (i->off == off)
			شمار++;
	أرجع (شمار == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
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
	إذا (len <= tdb->map_اندازه)
		أرجع 0;
	إذا (tdb->flags & TDB_داخلی) {
		إذا (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (صحيح)len, (صحيح)tdb->map_اندازه));
		}
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	إذا (fstat(tdb->fd, &st) == -1) {
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	إذا (st.st_اندازه < (off_t)len) {
		إذا (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (صحيح)len, (صحيح)st.st_اندازه));
		}
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	إذا (tdb_munmap(tdb) == -1)
		أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_اندازه = st.st_اندازه;
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
		کپی_حافظه(off + (حرف *)tdb->map_ptr, buf, len);
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
فراغ *tdb_convert(فراغ *buf, u32 اندازه)
{
	u32 i, *p = (u32 *)buf;
	لكل (i = 0; i < اندازه / 4; i++)
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
		کپی_حافظه(buf, off + (حرف *)tdb->map_ptr, len);
	} وإلا {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		إذا (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (صحيح)off, (صحيح)len, (صحيح)ret, strerror(errno),
				 (صحيح)tdb->map_اندازه));
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
ساكن فراغ tdb_بعدی_hash_chain(بنية tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	إذا (tdb->map_ptr) {
		لكل (;h < tdb->سرآیند.hash_اندازه;h++) {
			إذا (0 != *(u32 *)(TDB_HASH_بالاترین(h) + (بیعلامت حرف *)tdb->map_ptr)) {
				اكسر;
			}
		}
	} وإلا {
		u32 off=0;
		لكل (;h < tdb->سرآیند.hash_اندازه;h++) {
			إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(h), &off) != 0 || off != 0) {
				اكسر;
			}
		}
	}
	(*chain) = h;
}


صحيح tdb_munmap(بنية tdb_context *tdb)
{
	إذا (tdb->flags & TDB_داخلی)
		أرجع 0;

#اگر_تعریف_شده HAVE_MMAP
	إذا (tdb->map_ptr) {
		صحيح ret = munmap(tdb->map_ptr, tdb->real_map_اندازه);
		إذا (ret != 0)
			أرجع ret;
		tdb->real_map_اندازه = 0;
	}
#پایان_اگر
	tdb->map_ptr = عدم;
	أرجع 0;
}

فراغ tdb_mmap(بنية tdb_context *tdb)
{
	إذا (tdb->flags & TDB_داخلی)
		أرجع;

#اگر_تعریف_شده HAVE_MMAP
	إذا (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(عدم, tdb->map_اندازه,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		إذا (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_اندازه = 0;
			tdb->map_ptr = عدم;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_اندازه, strerror(errno)));
		}
		tdb->real_map_اندازه = tdb->map_اندازه;
	} وإلا {
		tdb->map_ptr = عدم;
	}
#وإلا
	tdb->map_ptr = عدم;
#پایان_اگر
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ساكن صحيح tdb_expand_file(بنية tdb_context *tdb, tdb_off_t اندازه, tdb_off_t addition)
{
	حرف buf[1024];

	إذا (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		أرجع -1;
	}

	إذا (ftruncate(tdb->fd, اندازه+addition) == -1) {
		حرف b = 0;
		إذا (pwrite(tdb->fd,  &b, 1, (اندازه+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 اندازه+addition, strerror(errno)));
			أرجع -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, حجم(buf));
	طالما (addition) {
		صحيح n = addition>حجم(buf)?حجم(buf):addition;
		صحيح ret = pwrite(tdb->fd, buf, n, اندازه);
		إذا (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			أرجع -1;
		}
		addition -= n;
		اندازه += n;
	}
	أرجع 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
صحيح tdb_expand(بنية tdb_context *tdb, tdb_off_t اندازه)
{
	بنية list_struct rec;
	tdb_off_t جابه_جایی;

	إذا (tdb_قفل(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		أرجع -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_اندازه + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	اندازه = TDB_ALIGN(tdb->map_اندازه + اندازه*10, tdb->صفحه_اندازه) - tdb->map_اندازه;

	إذا (!(tdb->flags & TDB_داخلی))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	إذا (!(tdb->flags & TDB_داخلی)) {
		إذا (tdb->methods->tdb_expand_file(tdb, tdb->map_اندازه, اندازه) != 0)
			اذهب fail;
	}

	tdb->map_اندازه += اندازه;

	إذا (tdb->flags & TDB_داخلی) {
		حرف *new_map_ptr = (حرف *)realloc(tdb->map_ptr,
						    tdb->map_اندازه);
		إذا (!new_map_ptr) {
			tdb->map_اندازه -= اندازه;
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
	rec.rec_len = اندازه - حجم(rec);

	/* link it into the free list */
	جابه_جایی = tdb->map_اندازه - اندازه;
	إذا (tdb_free(tdb, جابه_جایی, &rec) == -1)
		اذهب fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع -1;
}

/* read/write a tdb_off_t */
صحيح tdb_ofs_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_off_t *d)
{
	أرجع tdb->methods->tdb_read(tdb, جابه_جایی, (حرف*)d, حجم(*d), DOCONV());
}

صحيح tdb_ofs_write(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_off_t *d)
{
	tdb_off_t off = *d;
	أرجع tdb->methods->tdb_write(tdb, جابه_جایی, CONVERT(off), حجم(*d));
}


/* read a lump of data, allocating the space for it */
بیعلامت حرف *tdb_alloc_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_len_t len)
{
	بیعلامت حرف *buf;

	/* some systems don't like zero length malloc */
	إذا (len == 0) {
		len = 1;
	}

	إذا (!(buf = (بیعلامت حرف *)حافظه_بده(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		أرجع TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	إذا (tdb->methods->tdb_read(tdb, جابه_جایی, buf, len, 0) == -1) {
		امن_FREE(buf);
		أرجع عدم;
	}
	أرجع buf;
}

/* Give a piece of tdb data to a parser */

صحيح tdb_parse_داده(بنية tdb_context *tdb, TDB_داده کلید,
		   tdb_off_t جابه_جایی, tdb_len_t len,
		   صحيح (*parser)(TDB_داده کلید, TDB_داده داده,
				 فراغ *خصوصی_داده),
		   فراغ *خصوصی_داده)
{
	TDB_داده داده;
	صحيح نتیجه;

	داده.dsize = len;

	إذا ((tdb->transaction == عدم) && (tdb->map_ptr != عدم)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		إذا (tdb->methods->tdb_oob(tdb, جابه_جایی+len, 0) != 0) {
			أرجع -1;
		}
		داده.dptr = جابه_جایی + (بیعلامت حرف *)tdb->map_ptr;
		أرجع parser(کلید, داده, خصوصی_داده);
	}

	إذا (!(داده.dptr = tdb_alloc_read(tdb, جابه_جایی, len))) {
		أرجع -1;
	}

	نتیجه = parser(کلید, داده, خصوصی_داده);
	حرر(داده.dptr);
	أرجع نتیجه;
}

/* read/write a record */
صحيح tdb_rec_read(بنية tdb_context *tdb, tdb_off_t جابه_جایی, بنية list_struct *rec)
{
	إذا (tdb->methods->tdb_read(tdb, جابه_جایی, rec, حجم(*rec),DOCONV()) == -1)
		أرجع -1;
	إذا (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, جابه_جایی));
		أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	أرجع tdb->methods->tdb_oob(tdb, rec->بعدی+حجم(*rec), 0);
}

صحيح tdb_rec_write(بنية tdb_context *tdb, tdb_off_t جابه_جایی, بنية list_struct *rec)
{
	بنية list_struct r = *rec;
	أرجع tdb->methods->tdb_write(tdb, جابه_جایی, CONVERT(r), حجم(r));
}

ساكن ثابت بنية tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_بعدی_hash_chain,
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
	بنية tdb_transaction_el *بعدی, *prev;
	tdb_off_t جابه_جایی;
	tdb_len_t درازی;
	بیعلامت حرف *داده;
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
	بنية tdb_transaction_el *elements, *elements_آخرین;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	صحيح transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	صحيح nesting;

	/* old file size before transaction */
	tdb_len_t old_map_اندازه;
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
	لكل (el=tdb->transaction->elements_آخرین;el;el=el->prev) {
		tdb_len_t partial;

		إذا (off+len <= el->جابه_جایی) {
			استمر;
		}
		إذا (off >= el->جابه_جایی + el->درازی) {
			استمر;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		إذا (off < el->جابه_جایی) {
			partial = el->جابه_جایی - off;
			إذا (transaction_read(tdb, off, buf, partial, cv) != 0) {
				اذهب fail;
			}
			len -= partial;
			off += partial;
			buf = (فراغ *)(partial + (حرف *)buf);
		}
		إذا (off + len <= el->جابه_جایی + el->درازی) {
			partial = len;
		} وإلا {
			partial = el->جابه_جایی + el->درازی - off;
		}
		کپی_حافظه(buf, el->داده + (off - el->جابه_جایی), partial);
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
	بنية tdb_transaction_el *el, *بهترین_el=عدم;

	إذا (len == 0) {
		أرجع 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	إذا (len == حجم(tdb_off_t) && off >= FREELIST_بالاترین &&
	    off < FREELIST_بالاترین+TDB_HASHTABLE_اندازه(tdb)) {
		u32 chain = (off-FREELIST_بالاترین) / حجم(tdb_off_t);
		کپی_حافظه(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	لكل (el=tdb->transaction->elements_آخرین;el;el=el->prev) {
		tdb_len_t partial;

		إذا (بهترین_el == عدم && off == el->جابه_جایی+el->درازی) {
			بهترین_el = el;
		}

		إذا (off+len <= el->جابه_جایی) {
			استمر;
		}
		إذا (off >= el->جابه_جایی + el->درازی) {
			استمر;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		إذا (off < el->جابه_جایی) {
			partial = el->جابه_جایی - off;
			إذا (transaction_write(tdb, off, buf, partial) != 0) {
				اذهب fail;
			}
			len -= partial;
			off += partial;
			buf = (ثابت فراغ *)(partial + (ثابت حرف *)buf);
		}
		إذا (off + len <= el->جابه_جایی + el->درازی) {
			partial = len;
		} وإلا {
			partial = el->جابه_جایی + el->درازی - off;
		}
		کپی_حافظه(el->داده + (off - el->جابه_جایی), buf, partial);
		len -= partial;
		off += partial;
		buf = (ثابت فراغ *)(partial + (ثابت حرف *)buf);

		إذا (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			اذهب fail;
		}

		أرجع 0;
	}

	/* see if we can append the new entry to an existing entry */
	إذا (بهترین_el && بهترین_el->جابه_جایی + بهترین_el->درازی == off &&
	    (off+len < tdb->transaction->old_map_اندازه ||
	     off > tdb->transaction->old_map_اندازه)) {
		بیعلامت حرف *داده = بهترین_el->داده;
		el = بهترین_el;
		el->داده = (بیعلامت حرف *)realloc(el->داده,
						    el->درازی + len);
		إذا (el->داده == عدم) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->داده = داده;
			أرجع -1;
		}
		إذا (buf) {
			کپی_حافظه(el->داده + el->درازی, buf, len);
		} وإلا {
			memset(el->داده + el->درازی, TDB_PAD_BYTE, len);
		}
		el->درازی += len;
		أرجع 0;
	}

	/* add a new entry at the end of the list */
	el = (بنية tdb_transaction_el *)حافظه_بده(حجم(*el));
	إذا (el == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		أرجع -1;
	}
	el->بعدی = عدم;
	el->prev = tdb->transaction->elements_آخرین;
	el->جابه_جایی = off;
	el->درازی = len;
	el->داده = (بیعلامت حرف *)حافظه_بده(len);
	إذا (el->داده == عدم) {
		حرر(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		أرجع -1;
	}
	إذا (buf) {
		کپی_حافظه(el->داده, buf, len);
	} وإلا {
		memset(el->داده, TDB_PAD_BYTE, len);
	}
	إذا (el->prev) {
		el->prev->بعدی = el;
	} وإلا {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_آخرین = el;
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
ساكن فراغ transaction_بعدی_hash_chain(بنية tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	لكل (;h < tdb->سرآیند.hash_اندازه;h++) {
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
			   صحيح probe EXT2FS_ATTR((استفادهنشده)))
{
	إذا (len <= tdb->map_اندازه) {
		أرجع 0;
	}
	أرجع TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ساكن صحيح transaction_expand_file(بنية tdb_context *tdb, tdb_off_t اندازه,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	إذا (transaction_write(tdb, اندازه, عدم, addition) != 0) {
		أرجع -1;
	}

	أرجع 0;
}

/*
  brlock during a transaction - ignore them
*/
ساكن صحيح transaction_brlock(بنية tdb_context *tdb EXT2FS_ATTR((استفادهنشده)),
			      tdb_off_t جابه_جایی EXT2FS_ATTR((استفادهنشده)),
			      صحيح rw_type EXT2FS_ATTR((استفادهنشده)),
			      صحيح lck_type EXT2FS_ATTR((استفادهنشده)),
			      صحيح probe EXT2FS_ATTR((استفادهنشده)),
			      size_t len EXT2FS_ATTR((استفادهنشده)))
{
	أرجع 0;
}

ساكن ثابت بنية tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_بعدی_hash_chain,
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
	إذا (tdb->read_only || (tdb->flags & TDB_داخلی) || tdb->traverse_read) {
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

	إذا (tdb->رقم_locks != 0 || tdb->global_قفل.شمار) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_قفل;
		أرجع -1;
	}

	إذا (tdb->travlocks.بعدی != عدم) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_قفل;
		أرجع -1;
	}

	tdb->transaction = (بنية tdb_transaction *)
		حافظه_صفر_بده(حجم(بنية tdb_transaction), 1);
	إذا (tdb->transaction == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	إذا (tdb_transaction_قفل(tdb, F_WRLCK) == -1) {
		امن_FREE(tdb->transaction);
		أرجع -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	إذا (tdb_brlock(tdb, FREELIST_بالاترین, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_قفل;
		اذهب fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		حافظه_صفر_بده(tdb->سرآیند.hash_اندازه+1, حجم(u32));
	إذا (tdb->transaction->hash_heads == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب fail;
	}
	إذا (tdb->methods->tdb_read(tdb, FREELIST_بالاترین, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_اندازه(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		اذهب fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_اندازه + 1, 1);
	tdb->transaction->old_map_اندازه = tdb->map_اندازه;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	إذا (transaction_write(tdb, FREELIST_بالاترین, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_اندازه(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		اذهب fail;
	}

	أرجع 0;

fail:
	tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	امن_FREE(tdb->transaction->hash_heads);
	امن_FREE(tdb->transaction);
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

	tdb->map_اندازه = tdb->transaction->old_map_اندازه;

	/* free all the transaction elements */
	طالما (tdb->transaction->elements) {
		بنية tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->بعدی;
		حرر(el->داده);
		حرر(el);
	}

	/* remove any global lock created during the transaction */
	إذا (tdb->global_قفل.شمار != 0) {
		tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW, 0, 4*tdb->سرآیند.hash_اندازه);
		tdb->global_قفل.شمار = 0;
	}

	/* remove any locks created during the transaction */
	إذا (tdb->رقم_locks != 0) {
		صحيح i;
		لكل (i=0;i<tdb->رقم_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_بالاترین+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->رقم_locks = 0;
		tdb->رقم_lockrecs = 0;
		امن_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_بالاترین, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	امن_FREE(tdb->transaction->hash_heads);
	امن_FREE(tdb->transaction);

	أرجع 0;
}

/*
  sync to disk
*/
ساكن صحيح transaction_sync(بنية tdb_context *tdb, tdb_off_t جابه_جایی, tdb_len_t درازی)
{
	إذا (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		أرجع -1;
	}
#إذا defined(HAVE_MSYNC) && defined(MS_SYNC)
	إذا (tdb->map_ptr) {
		tdb_off_t moffset = جابه_جایی & ~(tdb->صفحه_اندازه-1);
		إذا (msync(moffset + (حرف *)tdb->map_ptr,
			  درازی + (جابه_جایی - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			أرجع -1;
		}
	}
#پایان_اگر
	أرجع 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ساكن tdb_len_t tdb_recovery_اندازه(بنية tdb_context *tdb)
{
	بنية tdb_transaction_el *el;
	tdb_len_t recovery_اندازه = 0;

	recovery_اندازه = حجم(u32);
	لكل (el=tdb->transaction->elements;el;el=el->بعدی) {
		إذا (el->جابه_جایی >= tdb->transaction->old_map_اندازه) {
			استمر;
		}
		recovery_اندازه += 2*حجم(tdb_off_t) + el->درازی;
	}

	أرجع recovery_اندازه;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ساكن صحيح tdb_recovery_allocate(بنية tdb_context *tdb,
				 tdb_len_t *recovery_اندازه,
				 tdb_off_t *recovery_جابه_جایی,
				 tdb_len_t *recovery_max_اندازه)
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

	*recovery_اندازه = tdb_recovery_اندازه(tdb);

	إذا (recovery_سر != 0 && *recovery_اندازه <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_اندازه = rec.rec_len;
		*recovery_جابه_جایی = recovery_سر;
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
	*recovery_اندازه = tdb_recovery_اندازه(tdb);

	/* round up to a multiple of page size */
	*recovery_max_اندازه = TDB_ALIGN(حجم(rec) + *recovery_اندازه, tdb->صفحه_اندازه) - حجم(rec);
	*recovery_جابه_جایی = tdb->map_اندازه;
	recovery_سر = *recovery_جابه_جایی;

	إذا (methods->tdb_expand_file(tdb, tdb->transaction->old_map_اندازه,
				     (tdb->map_اندازه - tdb->transaction->old_map_اندازه) +
				     حجم(rec) + *recovery_max_اندازه) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		أرجع -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_اندازه + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_اندازه = tdb->map_اندازه;

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
ساكن صحيح transaction_راهاندازی_recovery(بنية tdb_context *tdb,
				      tdb_off_t *magic_جابه_جایی)
{
	بنية tdb_transaction_el *el;
	tdb_len_t recovery_اندازه;
	بیعلامت حرف *داده, *p;
	ثابت بنية tdb_methods *methods = tdb->transaction->io_methods;
	بنية list_struct *rec;
	tdb_off_t recovery_جابه_جایی, recovery_max_اندازه;
	tdb_off_t old_map_اندازه = tdb->transaction->old_map_اندازه;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	إذا (tdb_recovery_allocate(tdb, &recovery_اندازه,
				  &recovery_جابه_جایی, &recovery_max_اندازه) == -1) {
		أرجع -1;
	}

	داده = (بیعلامت حرف *)حافظه_بده(recovery_اندازه + حجم(*rec));
	إذا (داده == عدم) {
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	rec = (بنية list_struct *)داده;
	memset(rec, 0, حجم(*rec));

	rec->magic    = 0;
	rec->داده_len = recovery_اندازه;
	rec->rec_len  = recovery_max_اندازه;
	rec->کلید_len  = old_map_اندازه;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = داده + حجم(*rec);
	لكل (el=tdb->transaction->elements;el;el=el->بعدی) {
		إذا (el->جابه_جایی >= old_map_اندازه) {
			استمر;
		}
		إذا (el->جابه_جایی + el->درازی > tdb->transaction->old_map_اندازه) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			حرر(داده);
			tdb->ecode = TDB_ERR_CORRUPT;
			أرجع -1;
		}
		کپی_حافظه(p, &el->جابه_جایی, 4);
		کپی_حافظه(p+4, &el->درازی, 4);
		إذا (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		إذا (methods->tdb_read(tdb, el->جابه_جایی, p + 8, el->درازی, 0) != 0) {
			حرر(داده);
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
		p += 8 + el->درازی;
	}

	/* and the tailer */
	tailer = حجم(*rec) + recovery_max_اندازه;
	کپی_حافظه(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	إذا (methods->tdb_write(tdb, recovery_جابه_جایی, داده, حجم(*rec) + recovery_اندازه) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		حرر(داده);
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	إذا (transaction_sync(tdb, recovery_جابه_جایی, حجم(*rec) + recovery_اندازه) == -1) {
		حرر(داده);
		أرجع -1;
	}

	حرر(داده);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_جابه_جایی = recovery_جابه_جایی + offsetof(بنية list_struct, magic);

	إذا (methods->tdb_write(tdb, *magic_جابه_جایی, &magic, حجم(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		أرجع -1;
	}

	/* ensure the recovery magic marker is on disk */
	إذا (transaction_sync(tdb, *magic_جابه_جایی, حجم(magic)) == -1) {
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
	tdb_off_t magic_جابه_جایی = 0;
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
	إذا (tdb->رقم_locks || tdb->global_قفل.شمار) {
		tdb->ecode = TDB_ERR_قفل;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	إذا (tdb_brlock_upgrade(tdb, FREELIST_بالاترین, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_قفل;
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	إذا (tdb_brlock(tdb, GLOBAL_قفل, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_قفل;
		tdb_transaction_cancel(tdb);
		أرجع -1;
	}

	إذا (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		إذا (transaction_راهاندازی_recovery(tdb, &magic_جابه_جایی) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_قفل, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			أرجع -1;
		}
	}

	/* expand the file to the new size if needed */
	إذا (tdb->map_اندازه != tdb->transaction->old_map_اندازه) {
		إذا (methods->tdb_expand_file(tdb, tdb->transaction->old_map_اندازه,
					     tdb->map_اندازه -
					     tdb->transaction->old_map_اندازه) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_قفل, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			أرجع -1;
		}
		tdb->map_اندازه = tdb->transaction->old_map_اندازه;
		methods->tdb_oob(tdb, tdb->map_اندازه + 1, 1);
	}

	/* perform all the writes */
	طالما (tdb->transaction->elements) {
		بنية tdb_transaction_el *el = tdb->transaction->elements;

		إذا (methods->tdb_write(tdb, el->جابه_جایی, el->داده, el->درازی) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_قفل, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			أرجع -1;
		}
		tdb->transaction->elements = el->بعدی;
		حرر(el->داده);
		حرر(el);
	}

	إذا (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		إذا (transaction_sync(tdb, 0, tdb->map_اندازه) == -1) {
			أرجع -1;
		}

		/* remove the recovery marker */
		إذا (methods->tdb_write(tdb, magic_جابه_جایی, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			أرجع -1;
		}

		/* ensure the recovery marker has been removed on disk */
		إذا (transaction_sync(tdb, magic_جابه_جایی, 4) == -1) {
			أرجع -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_قفل, F_UNLCK, F_SETLKW, 0, 1);

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
#اگر_تعریف_شده HAVE_UTIME
	utime(tdb->name, عدم);
#پایان_اگر

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
	بیعلامت حرف *داده, *p;
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

	داده = (بیعلامت حرف *)حافظه_بده(rec.داده_len);
	إذا (داده == عدم) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		أرجع -1;
	}

	/* read the full recovery data */
	إذا (tdb->methods->tdb_read(tdb, recovery_سر + حجم(rec), داده,
				   rec.داده_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		حرر(داده);
		أرجع -1;
	}

	/* recover the file data */
	p = داده;
	طالما (p+8 < داده + rec.داده_len) {
		u32 ofs, len;
		إذا (DOCONV()) {
			tdb_convert(p, 8);
		}
		کپی_حافظه(&ofs, p, 4);
		کپی_حافظه(&len, p+4, 4);

		إذا (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			حرر(داده);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			أرجع -1;
		}
		p += 8 + len;
	}

	حرر(داده);

	إذا (transaction_sync(tdb, 0, tdb->map_اندازه) == -1) {
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
	tdb->map_اندازه = recovery_eof;
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
	إذا (tdb->methods->tdb_oob(tdb, rec->بعدی+حجم(*rec), 0) != 0)
		أرجع -1;
	أرجع 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ساكن صحيح remove_from_freelist(بنية tdb_context *tdb, tdb_off_t off, tdb_off_t بعدی)
{
	tdb_off_t آخرین_ptr, i;

	/* read in the freelist top */
	آخرین_ptr = FREELIST_بالاترین;
	طالما (tdb_ofs_read(tdb, آخرین_ptr, &i) != -1 && i != 0) {
		إذا (i == off) {
			/* We've found it! */
			أرجع tdb_ofs_write(tdb, آخرین_ptr, &بعدی);
		}
		/* Follow chain (next offset is at start of record) */
		آخرین_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	أرجع TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ساكن صحيح update_tailer(بنية tdb_context *tdb, tdb_off_t جابه_جایی,
			 ثابت بنية list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = حجم(*rec) + rec->rec_len;
	أرجع tdb_ofs_write(tdb, جابه_جایی + totalsize - حجم(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
صحيح tdb_free(بنية tdb_context *tdb, tdb_off_t جابه_جایی, بنية list_struct *rec)
{
	tdb_off_t راست, چپ;

	/* Allocation and tailer lock */
	إذا (tdb_قفل(tdb, -1, F_WRLCK) != 0)
		أرجع -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	إذا (update_tailer(tdb, جابه_جایی, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		اذهب fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	راست = جابه_جایی + حجم(*rec) + rec->rec_len;
	إذا (راست + حجم(*rec) <= tdb->map_اندازه) {
		بنية list_struct r;

		إذا (tdb->methods->tdb_read(tdb, راست, &r, حجم(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", راست));
			اذهب چپ;
		}

		/* If it's free, expand to include it. */
		إذا (r.magic == TDB_FREE_MAGIC) {
			إذا (remove_from_freelist(tdb, راست, r.بعدی) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", راست));
				اذهب چپ;
			}
			rec->rec_len += حجم(r) + r.rec_len;
		}
	}

چپ:
	/* Look left */
	چپ = جابه_جایی - حجم(tdb_off_t);
	إذا (چپ > TDB_داده_شروع(tdb->سرآیند.hash_اندازه)) {
		بنية list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		إذا (tdb_ofs_read(tdb, چپ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", چپ));
			اذهب update;
		}

		/* it could be uninitialised data */
		إذا (leftsize == 0 || leftsize == TDB_PAD_U32) {
			اذهب update;
		}

		چپ = جابه_جایی - leftsize;

		/* Now read in record */
		إذا (tdb->methods->tdb_read(tdb, چپ, &l, حجم(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", چپ, leftsize));
			اذهب update;
		}

		/* If it's free, expand to include it. */
		إذا (l.magic == TDB_FREE_MAGIC) {
			إذا (remove_from_freelist(tdb, چپ, l.بعدی) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", چپ));
				اذهب update;
			} وإلا {
				جابه_جایی = چپ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	إذا (update_tailer(tdb, جابه_جایی, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", جابه_جایی));
		اذهب fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	إذا (tdb_ofs_read(tdb, FREELIST_بالاترین, &rec->بعدی) == -1 ||
	    tdb_rec_write(tdb, جابه_جایی, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_بالاترین, &جابه_جایی) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", جابه_جایی));
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
ساكن tdb_off_t tdb_allocate_ofs(بنية tdb_context *tdb, tdb_len_t درازی, tdb_off_t rec_ptr,
				بنية list_struct *rec, tdb_off_t آخرین_ptr)
{
	بنية list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', حجم(newrec));

	/* found it - now possibly split it up  */
	إذا (rec->rec_len > درازی + MIN_REC_اندازه) {
		/* Length of left piece */
		درازی = TDB_ALIGN(درازی, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (حجم(*rec) + درازی);
		newrec_ptr = rec_ptr + حجم(*rec) + درازی;

		/* And left record is shortened */
		rec->rec_len = درازی;
	} وإلا {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	إذا (tdb_ofs_write(tdb, آخرین_ptr, &rec->بعدی) == -1) {
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
tdb_off_t tdb_allocate(بنية tdb_context *tdb, tdb_len_t درازی, بنية list_struct *rec)
{
	tdb_off_t rec_ptr, آخرین_ptr, newrec_ptr;
	بنية {
		tdb_off_t rec_ptr, آخرین_ptr;
		tdb_len_t rec_len;
	} bestfit;

	إذا (tdb_قفل(tdb, -1, F_WRLCK) == -1)
		أرجع 0;

	/* Extra bytes required for tailer */
	درازی += حجم(tdb_off_t);

 again:
	آخرین_ptr = FREELIST_بالاترین;

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, FREELIST_بالاترین, &rec_ptr) == -1)
		اذهب fail;

	bestfit.rec_ptr = 0;
	bestfit.آخرین_ptr = 0;
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

		إذا (rec->rec_len >= درازی) {
			إذا (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.آخرین_ptr = آخرین_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				إذا (bestfit.rec_len < 2*درازی) {
					اكسر;
				}
			}
		}

		/* move to the next record */
		آخرین_ptr = rec_ptr;
		rec_ptr = rec->بعدی;
	}

	إذا (bestfit.rec_ptr != 0) {
		إذا (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			اذهب fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, درازی, bestfit.rec_ptr, rec, bestfit.آخرین_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		أرجع newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	إذا (tdb_expand(tdb, درازی + حجم(*rec)) == 0)
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

ساكن صحيح دیده_درج(بنية tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_داده کلید, داده;

	memset(&داده, '\0', حجم(داده));
	کلید.dptr = (بیعلامت حرف *)&rec_ptr;
	کلید.dsize = حجم(rec_ptr);
	أرجع tdb_store(mem_tdb, کلید, داده, TDB_درج);
}

صحيح tdb_validate_freelist(بنية tdb_context *tdb, صحيح *pnum_entries)
{
	بنية tdb_context *mem_tdb = عدم;
	بنية list_struct rec;
	tdb_off_t rec_ptr, آخرین_ptr;
	صحيح ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->سرآیند.hash_اندازه,
				TDB_داخلی, O_RDWR, 0600);
	إذا (!mem_tdb) {
		أرجع -1;
	}

	إذا (tdb_قفل(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		أرجع 0;
	}

	آخرین_ptr = FREELIST_بالاترین;

	/* Store the FREELIST_TOP record. */
	إذا (دیده_درج(mem_tdb, آخرین_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		اذهب fail;
	}

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, FREELIST_بالاترین, &rec_ptr) == -1) {
		اذهب fail;
	}

	طالما (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		إذا (دیده_درج(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			اذهب fail;
		}

		إذا (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}

		/* move to the next record */
		آخرین_ptr = rec_ptr;
		rec_ptr = rec.بعدی;
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
ساكن صحيح tdb_بعدی_قفل(بنية tdb_context *tdb, بنية tdb_traverse_قفل *tlock,
			 بنية list_struct *rec)
{
	صحيح want_بعدی = (tlock->off != 0);

	/* Lock each chain from the start one. */
	لكل (; tlock->hash < tdb->سرآیند.hash_اندازه; tlock->hash++) {
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
			tdb->methods->بعدی_hash_chain(tdb, &tlock->hash);
			إذا (tlock->hash == tdb->سرآیند.hash_اندازه) {
				استمر;
			}
		}

		إذا (tdb_قفل(tdb, tlock->hash, tlock->قفل_rw) == -1)
			أرجع -1;

		/* No previous record?  Start at top of chain. */
		إذا (!tlock->off) {
			إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(tlock->hash),
				     &tlock->off) == -1)
				اذهب fail;
		} وإلا {
			/* Otherwise unlock the previous record. */
			إذا (tdb_unlock_رکورد(tdb, tlock->off) != 0)
				اذهب fail;
		}

		إذا (want_بعدی) {
			/* We have offset of old record: grab next */
			إذا (tdb_rec_read(tdb, tlock->off, rec) == -1)
				اذهب fail;
			tlock->off = rec->بعدی;
		}

		/* Iterate through chain */
		طالما( tlock->off) {
			tdb_off_t current;
			إذا (tdb_rec_read(tdb, tlock->off, rec) == -1)
				اذهب fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			إذا (tlock->off == rec->بعدی) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				اذهب fail;
			}

			إذا (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				إذا (tdb_قفل_رکورد(tdb, tlock->off) != 0)
					اذهب fail;
				أرجع tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->بعدی;
			إذا (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				اذهب fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->قفل_rw);
		want_بعدی = 0;
	}
	/* We finished iteration without finding anything */
	أرجع TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	إذا (tdb_unlock(tdb, tlock->hash, tlock->قفل_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	أرجع -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ساكن صحيح tdb_traverse_داخلی(بنية tdb_context *tdb,
				 tdb_traverse_func fn, فراغ *خصوصی_داده,
				 بنية tdb_traverse_قفل *tl)
{
	TDB_داده کلید, dbuf;
	بنية list_struct rec;
	صحيح ret, شمار = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->بعدی = tdb->travlocks.بعدی;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.بعدی = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	طالما ((ret = tdb_بعدی_قفل(tdb, tl, &rec)) > 0) {
		شمار++;
		/* now read the full record */
		کلید.dptr = tdb_alloc_read(tdb, tl->off + حجم(rec),
					  rec.کلید_len + rec.داده_len);
		إذا (!کلید.dptr) {
			ret = -1;
			إذا (tdb_unlock(tdb, tl->hash, tl->قفل_rw) != 0)
				اذهب بیرون;
			إذا (tdb_unlock_رکورد(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			اذهب بیرون;
		}
		کلید.dsize = rec.کلید_len;
		dbuf.dptr = کلید.dptr + rec.کلید_len;
		dbuf.dsize = rec.داده_len;

		/* Drop chain lock, call out */
		إذا (tdb_unlock(tdb, tl->hash, tl->قفل_rw) != 0) {
			ret = -1;
			امن_FREE(کلید.dptr);
			اذهب بیرون;
		}
		إذا (fn && fn(tdb, کلید, dbuf, خصوصی_داده)) {
			/* They want us to terminate traversal */
			ret = شمار;
			إذا (tdb_unlock_رکورد(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			امن_FREE(کلید.dptr);
			اذهب بیرون;
		}
		امن_FREE(کلید.dptr);
	}
بیرون:
	tdb->travlocks.بعدی = tl->بعدی;
	إذا (ret < 0)
		أرجع -1;
	وإلا
		أرجع شمار;
}


/*
  a write style traverse - temporarily marks the db read only
*/
صحيح tdb_traverse_read(بنية tdb_context *tdb,
		      tdb_traverse_func fn, فراغ *خصوصی_داده)
{
	بنية tdb_traverse_قفل tl = { عدم, 0, 0, F_RDLCK };
	صحيح ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	إذا (tdb_transaction_قفل(tdb, F_RDLCK)) {
		أرجع -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_داخلی(tdb, fn, خصوصی_داده, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	أرجع ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
صحيح tdb_traverse(بنية tdb_context *tdb,
		 tdb_traverse_func fn, فراغ *خصوصی_داده)
{
	بنية tdb_traverse_قفل tl = { عدم, 0, 0, F_WRLCK };
	صحيح ret;

	إذا (tdb->read_only || tdb->traverse_read) {
		أرجع tdb_traverse_read(tdb, fn, خصوصی_داده);
	}

	إذا (tdb_transaction_قفل(tdb, F_WRLCK)) {
		أرجع -1;
	}

	ret = tdb_traverse_داخلی(tdb, fn, خصوصی_داده, &tl);

	tdb_transaction_unlock(tdb);

	أرجع ret;
}


/* find the first entry in the database and return its key */
TDB_داده tdb_firstkey(بنية tdb_context *tdb)
{
	TDB_داده کلید;
	بنية list_struct rec;

	/* release any old lock */
	إذا (tdb_unlock_رکورد(tdb, tdb->travlocks.off) != 0)
		أرجع tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.قفل_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	إذا (tdb_بعدی_قفل(tdb, &tdb->travlocks, &rec) <= 0)
		أرجع tdb_null;
	/* now read the key */
	کلید.dsize = rec.کلید_len;
	کلید.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),کلید.dsize);

	/* Unlock the hash chain of the record we just read. */
	إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.قفل_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	أرجع کلید;
}

/* find the next entry in the database, returning its key */
TDB_داده tdb_nextkey(بنية tdb_context *tdb, TDB_داده oldkey)
{
	u32 oldhash;
	TDB_داده کلید = tdb_null;
	بنية list_struct rec;
	بیعلامت حرف *k = عدم;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	إذا (tdb->travlocks.off) {
		إذا (tdb_قفل(tdb,tdb->travlocks.hash,tdb->travlocks.قفل_rw))
			أرجع tdb_null;
		إذا (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),
					    rec.کلید_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			إذا (tdb_unlock_رکورد(tdb, tdb->travlocks.off) != 0) {
				امن_FREE(k);
				أرجع tdb_null;
			}
			إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.قفل_rw) != 0) {
				امن_FREE(k);
				أرجع tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		امن_FREE(k);
	}

	إذا (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_پیدا_کن_قفل_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.قفل_rw, &rec);
		إذا (!tdb->travlocks.off)
			أرجع tdb_null;
		tdb->travlocks.hash = سطل(rec.پر_hash);
		إذا (tdb_قفل_رکورد(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			أرجع tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	إذا (tdb_بعدی_قفل(tdb, &tdb->travlocks, &rec) > 0) {
		کلید.dsize = rec.کلید_len;
		کلید.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+حجم(rec),
					  کلید.dsize);
		/* Unlock the chain of this new record */
		إذا (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.قفل_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	إذا (tdb_unlock(tdb, سطل(oldhash), tdb->travlocks.قفل_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	أرجع کلید;
}

/* file: dump.c */

ساكن tdb_off_t tdb_dump_رکورد(بنية tdb_context *tdb, صحيح hash,
				 tdb_off_t جابه_جایی)
{
	بنية list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	إذا (tdb->methods->tdb_read(tdb, جابه_جایی, (حرف *)&rec,
				   حجم(rec), DOCONV()) == -1) {
		چاپ_کن("ERROR: failed to read record at %u\n", جابه_جایی);
		أرجع 0;
	}

	چاپ_کن(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, جابه_جایی, rec.بعدی, rec.rec_len, rec.کلید_len, rec.داده_len,
	       rec.پر_hash, rec.magic);

	tailer_ofs = جابه_جایی + حجم(rec) + rec.rec_len - حجم(tdb_off_t);

	إذا (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		چاپ_کن("ERROR: failed to read tailer at %u\n", tailer_ofs);
		أرجع rec.بعدی;
	}

	إذا (tailer != rec.rec_len + حجم(rec)) {
		چاپ_کن("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(بیعلامت صحيح)tailer, (بیعلامت صحيح)(rec.rec_len + حجم(rec)));
	}
	أرجع rec.بعدی;
}

ساكن صحيح tdb_dump_chain(بنية tdb_context *tdb, صحيح i)
{
	tdb_off_t rec_ptr, بالاترین;

	بالاترین = TDB_HASH_بالاترین(i);

	إذا (tdb_قفل(tdb, i, F_WRLCK) != 0)
		أرجع -1;

	إذا (tdb_ofs_read(tdb, بالاترین, &rec_ptr) == -1)
		أرجع tdb_unlock(tdb, i, F_WRLCK);

	إذا (rec_ptr)
		چاپ_کن("hash=%d\n", i);

	طالما (rec_ptr) {
		rec_ptr = tdb_dump_رکورد(tdb, i, rec_ptr);
	}

	أرجع tdb_unlock(tdb, i, F_WRLCK);
}

فراغ tdb_dump_all(بنية tdb_context *tdb)
{
	صحيح i;
	لكل (i = 0; i < (صحيح)tdb->سرآیند.hash_اندازه; i++) {
		tdb_dump_chain(tdb, i);
	}
	چاپ_کن("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

صحيح tdb_printfreelist(بنية tdb_context *tdb)
{
	صحيح ret;
	دراز مجموع_free = 0;
	tdb_off_t جابه_جایی, rec_ptr;
	بنية list_struct rec;

	إذا ((ret = tdb_قفل(tdb, -1, F_WRLCK)) != 0)
		أرجع ret;

	جابه_جایی = FREELIST_بالاترین;

	/* read in the freelist top */
	إذا (tdb_ofs_read(tdb, جابه_جایی, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		أرجع 0;
	}

	چاپ_کن("freelist top=[0x%08x]\n", rec_ptr );
	طالما (rec_ptr) {
		إذا (tdb->methods->tdb_read(tdb, rec_ptr, (حرف *)&rec,
					   حجم(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			أرجع -1;
		}

		إذا (rec.magic != TDB_FREE_MAGIC) {
			چاپ_کن("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			أرجع -1;
		}

		چاپ_کن("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		مجموع_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.بعدی;
	}
	چاپ_کن("total rec_len = [0x%08x (%d)]\n", (صحيح)مجموع_free,
               (صحيح)مجموع_free);

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

ساكن صحيح tdb_کلید_compare(TDB_داده کلید, TDB_داده داده,
			   فراغ *خصوصی_داده EXT2FS_ATTR((استفادهنشده)))
{
	أرجع memcmp(داده.dptr, کلید.dptr, داده.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ساكن tdb_off_t tdb_پیدا_کن(بنية tdb_context *tdb, TDB_داده کلید, u32 hash,
			بنية list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		أرجع 0;

	/* keep looking until we find the right record */
	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, r) == -1)
			أرجع 0;

		إذا (!TDB_DEAD(r) && hash==r->پر_hash
		    && کلید.dsize==r->کلید_len
		    && tdb_parse_داده(tdb, کلید, rec_ptr + حجم(*r),
				      r->کلید_len, tdb_کلید_compare,
				      عدم) == 0) {
			أرجع rec_ptr;
		}
		rec_ptr = r->بعدی;
	}
	أرجع TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_پیدا_کن_قفل_hash(بنية tdb_context *tdb, TDB_داده کلید, u32 hash, صحيح locktype,
			   بنية list_struct *rec)
{
	u32 rec_ptr;

	إذا (tdb_قفل(tdb, سطل(hash), locktype) == -1)
		أرجع 0;
	إذا (!(rec_ptr = tdb_پیدا_کن(tdb, کلید, hash, rec)))
		tdb_unlock(tdb, سطل(hash), locktype);
	أرجع rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ساكن صحيح tdb_update_hash(بنية tdb_context *tdb, TDB_داده کلید, u32 hash, TDB_داده dbuf)
{
	بنية list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	إذا (!(rec_ptr = tdb_پیدا_کن(tdb, کلید, hash, &rec)))
		أرجع -1;

	/* must be long enough key, data and tailer */
	إذا (rec.rec_len < کلید.dsize + dbuf.dsize + حجم(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		أرجع -1;
	}

	إذا (tdb->methods->tdb_write(tdb, rec_ptr + حجم(rec) + rec.کلید_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		أرجع -1;

	إذا (dbuf.dsize != rec.داده_len) {
		/* update size */
		rec.داده_len = dbuf.dsize;
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
TDB_داده tdb_fetch(بنية tdb_context *tdb, TDB_داده کلید)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	TDB_داده ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	إذا (!(rec_ptr = tdb_پیدا_کن_قفل_hash(tdb,کلید,hash,F_RDLCK,&rec)))
		أرجع tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + حجم(rec) + rec.کلید_len,
				  rec.داده_len);
	ret.dsize = rec.داده_len;
	tdb_unlock(tdb, سطل(rec.پر_hash), F_RDLCK);
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

صحيح tdb_parse_رکورد(بنية tdb_context *tdb, TDB_داده کلید,
		     صحيح (*parser)(TDB_داده کلید, TDB_داده داده,
				   فراغ *خصوصی_داده),
		     فراغ *خصوصی_داده)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	صحيح ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);

	إذا (!(rec_ptr = tdb_پیدا_کن_قفل_hash(tdb,کلید,hash,F_RDLCK,&rec))) {
		أرجع TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_داده(tdb, کلید, rec_ptr + حجم(rec) + rec.کلید_len,
			     rec.داده_len, parser, خصوصی_داده);

	tdb_unlock(tdb, سطل(rec.پر_hash), F_RDLCK);

	أرجع ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ساكن صحيح tdb_exists_hash(بنية tdb_context *tdb, TDB_داده کلید, u32 hash)
{
	بنية list_struct rec;

	إذا (tdb_پیدا_کن_قفل_hash(tdb, کلید, hash, F_RDLCK, &rec) == 0)
		أرجع 0;
	tdb_unlock(tdb, سطل(rec.پر_hash), F_RDLCK);
	أرجع 1;
}

صحيح tdb_exists(بنية tdb_context *tdb, TDB_داده کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	أرجع tdb_exists_hash(tdb, کلید, hash);
}

/* actually delete an entry in the database given the offset */
صحيح tdb_do_delete(بنية tdb_context *tdb, tdb_off_t rec_ptr, بنية list_struct*rec)
{
	tdb_off_t آخرین_ptr, i;
	بنية list_struct lastrec;

	إذا (tdb->read_only || tdb->traverse_read) أرجع -1;

	إذا (tdb_write_قفل_رکورد(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		أرجع tdb_rec_write(tdb, rec_ptr, rec);
	}
	إذا (tdb_write_unlock_رکورد(tdb, rec_ptr) != 0)
		أرجع -1;

	/* find previous record in hash chain */
	إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(rec->پر_hash), &i) == -1)
		أرجع -1;
	لكل (آخرین_ptr = 0; i != rec_ptr; آخرین_ptr = i, i = lastrec.بعدی)
		إذا (tdb_rec_read(tdb, i, &lastrec) == -1)
			أرجع -1;

	/* unlink it: next ptr is at start of record. */
	إذا (آخرین_ptr == 0)
		آخرین_ptr = TDB_HASH_بالاترین(rec->پر_hash);
	إذا (tdb_ofs_write(tdb, آخرین_ptr, &rec->بعدی) == -1)
		أرجع -1;

	/* recover the space */
	إذا (tdb_free(tdb, rec_ptr, rec) == -1)
		أرجع -1;
	أرجع 0;
}

ساكن صحيح tdb_شمار_dead(بنية tdb_context *tdb, u32 hash)
{
	صحيح res = 0;
	tdb_off_t rec_ptr;
	بنية list_struct rec;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		أرجع 0;

	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			أرجع 0;

		إذا (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.بعدی;
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

	إذا (tdb_قفل(tdb, -1, F_WRLCK) == -1) {
		أرجع -1;
	}

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		اذهب fail;

	طالما (rec_ptr) {
		tdb_off_t بعدی;

		إذا (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}

		بعدی = rec.بعدی;

		إذا (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			اذهب fail;
		}
		rec_ptr = بعدی;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	أرجع res;
}

/* delete an entry in the database given a key */
ساكن صحيح tdb_delete_hash(بنية tdb_context *tdb, TDB_داده کلید, u32 hash)
{
	tdb_off_t rec_ptr;
	بنية list_struct rec;
	صحيح ret;

	إذا (tdb->max_dead_رکوردها != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		إذا (tdb_قفل(tdb, سطل(hash), F_WRLCK) == -1)
			أرجع -1;

		إذا (tdb_شمار_dead(tdb, hash) >= tdb->max_dead_رکوردها) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		إذا (!(rec_ptr = tdb_پیدا_کن(tdb, کلید, hash, &rec))) {
			tdb_unlock(tdb, سطل(hash), F_WRLCK);
			أرجع -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	وإلا {
		إذا (!(rec_ptr = tdb_پیدا_کن_قفل_hash(tdb, کلید, hash, F_WRLCK,
						   &rec)))
			أرجع -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	إذا (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	إذا (tdb_unlock(tdb, سطل(rec.پر_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	أرجع ret;
}

صحيح tdb_delete(بنية tdb_context *tdb, TDB_داده کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	أرجع tdb_delete_hash(tdb, کلید, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ساكن tdb_off_t tdb_پیدا_کن_dead(بنية tdb_context *tdb, u32 hash,
			       بنية list_struct *r, tdb_len_t درازی)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1)
		أرجع 0;

	/* keep looking until we find the right record */
	طالما (rec_ptr) {
		إذا (tdb_rec_read(tdb, rec_ptr, r) == -1)
			أرجع 0;

		إذا (TDB_DEAD(r) && r->rec_len >= درازی) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			أرجع rec_ptr;
		}
		rec_ptr = r->بعدی;
	}
	أرجع 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
صحيح tdb_store(بنية tdb_context *tdb, TDB_داده کلید, TDB_داده dbuf, صحيح flag)
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
	إذا (tdb_قفل(tdb, سطل(hash), F_WRLCK) == -1)
		أرجع -1;

	/* check for it existing, on insert. */
	إذا (flag == TDB_درج) {
		إذا (tdb_exists_hash(tdb, کلید, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			اذهب fail;
		}
	} وإلا {
		/* first try in-place update, on modify or replace. */
		إذا (tdb_update_hash(tdb, کلید, hash, dbuf) == 0) {
			اذهب انجامشده;
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
	إذا (flag != TDB_درج)
		tdb_delete_hash(tdb, کلید, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	إذا (!(p = (حرف *)حافظه_بده(کلید.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		اذهب fail;
	}

	کپی_حافظه(p, کلید.dptr, کلید.dsize);
	إذا (dbuf.dsize)
		کپی_حافظه(p+کلید.dsize, dbuf.dptr, dbuf.dsize);

	إذا (tdb->max_dead_رکوردها != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_پیدا_کن_dead(
			tdb, hash, &rec,
			کلید.dsize + dbuf.dsize + حجم(tdb_off_t));

		إذا (rec_ptr != 0) {
			rec.کلید_len = کلید.dsize;
			rec.داده_len = dbuf.dsize;
			rec.پر_hash = hash;
			rec.magic = TDB_MAGIC;
			إذا (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + حجم(rec),
				    p, کلید.dsize + dbuf.dsize) == -1) {
				اذهب fail;
			}
			اذهب انجامشده;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	إذا (tdb_قفل(tdb, -1, F_WRLCK) == -1) {
		اذهب fail;
	}

	إذا ((tdb->max_dead_رکوردها != 0)
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
	إذا (tdb_ofs_read(tdb, TDB_HASH_بالاترین(hash), &rec.بعدی) == -1)
		اذهب fail;

	rec.کلید_len = کلید.dsize;
	rec.داده_len = dbuf.dsize;
	rec.پر_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	إذا (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+حجم(rec), p, کلید.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_بالاترین(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		اذهب fail;
	}

 انجامشده:
	ret = 0;
 fail:
	إذا (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	امن_FREE(p);
	tdb_unlock(tdb, سطل(hash), F_WRLCK);
	أرجع ret;
}


/* Append to an entry. Create if not exist. */
صحيح tdb_append(بنية tdb_context *tdb, TDB_داده کلید, TDB_داده new_dbuf)
{
	u32 hash;
	TDB_داده dbuf;
	صحيح ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	إذا (tdb_قفل(tdb, سطل(hash), F_WRLCK) == -1)
		أرجع -1;

	dbuf = tdb_fetch(tdb, کلید);

	إذا (dbuf.dptr == عدم) {
		dbuf.dptr = (بیعلامت حرف *)حافظه_بده(new_dbuf.dsize);
	} وإلا {
		بیعلامت حرف *new_dptr = (بیعلامت حرف *)realloc(dbuf.dptr,
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

	کپی_حافظه(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, کلید, dbuf, 0);

failed:
	tdb_unlock(tdb, سطل(hash), F_WRLCK);
	امن_FREE(dbuf.dptr);
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
	أرجع tdb->لگاریتم.log_fn;
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

صحيح tdb_hash_اندازه(بنية tdb_context *tdb)
{
	أرجع tdb->سرآیند.hash_اندازه;
}

size_t tdb_map_اندازه(بنية tdb_context *tdb)
{
	أرجع tdb->map_اندازه;
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
ساكن بیعلامت صحيح default_tdb_hash(TDB_داده *کلید)
{
	u32 مقدار;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	لكل (مقدار = 0, i=0; i < کلید->dsize; i++)
		مقدار = مقدار * 256 + کلید->dptr[i] + (مقدار >> 24) * 241;

	أرجع مقدار;
}


/* initialise a new database with a specified hash size */
ساكن صحيح tdb_new_database(بنية tdb_context *tdb, صحيح hash_اندازه)
{
	بنية tdb_سرآیند *newdb;
	صحيح اندازه, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	اندازه = حجم(بنية tdb_سرآیند) + (hash_اندازه+1)*حجم(tdb_off_t);
	إذا (!(newdb = (بنية tdb_سرآیند *)حافظه_صفر_بده(اندازه, 1)))
		أرجع TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->نسخه = TDB_نسخه;
	newdb->hash_اندازه = hash_اندازه;
	إذا (tdb->flags & TDB_داخلی) {
		tdb->map_اندازه = اندازه;
		tdb->map_ptr = (حرف *)newdb;
		کپی_حافظه(&tdb->سرآیند, newdb, حجم(tdb->سرآیند));
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
	کپی_حافظه(&tdb->سرآیند, newdb, حجم(tdb->سرآیند));
	/* Don't endian-convert the magic food! */
	کپی_حافظه(newdb->magic_food, TDB_MAGIC_FOOD, طول_رشته(TDB_MAGIC_FOOD)+1);
	إذا (اكتب(tdb->fd, newdb, اندازه) != اندازه) {
		ret = -1;
	} وإلا {
		ret = 0;
	}

  fail:
	امن_FREE(newdb);
	أرجع ret;
}



ساكن صحيح tdb_already_open(dev_t device,
			    ino_t ino)
{
	بنية tdb_context *i;

	لكل (i = tdbs; i; i = i->بعدی) {
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
بنية tdb_context *tdb_open(ثابت حرف *name, صحيح hash_اندازه, صحيح tdb_flags,
		      صحيح open_flags, mode_t نما)
{
	أرجع tdb_open_ex(name, hash_اندازه, tdb_flags, open_flags, نما, عدم, عدم);
}

/* a default logging function */
ساكن فراغ null_log_fn(بنية tdb_context *tdb, تعداد tdb_debug_level level, ثابت حرف *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ساكن فراغ null_log_fn(بنية tdb_context *tdb EXT2FS_ATTR((استفادهنشده)),
			تعداد tdb_debug_level level EXT2FS_ATTR((استفادهنشده)),
			ثابت حرف *fmt EXT2FS_ATTR((استفادهنشده)), ...)
{
}


بنية tdb_context *tdb_open_ex(ثابت حرف *name, صحيح hash_اندازه, صحيح tdb_flags,
				صحيح open_flags, mode_t نما,
				ثابت بنية tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	بنية tdb_context *tdb;
	بنية stat st;
	صحيح rev = 0, locked = 0;
	بیعلامت حرف *vp;
	u32 vertest;

	إذا (!(tdb = (بنية tdb_context *)حافظه_صفر_بده(1, حجم *tdb))) {
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
		tdb->لگاریتم = *log_ctx;
	} وإلا {
		tdb->لگاریتم.log_fn = null_log_fn;
		tdb->لگاریتم.log_خصوصی = عدم;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->صفحه_اندازه = sysconf(_SC_PAGESIZE);
	إذا (tdb->صفحه_اندازه <= 0) {
		tdb->صفحه_اندازه = 0x2000;
	}

	إذا ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		اذهب fail;
	}

	إذا (hash_اندازه == 0)
		hash_اندازه = DEFAULT_HASH_اندازه;
	إذا ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_اولی;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	إذا (tdb->flags & TDB_داخلی) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_اولی;
		إذا (tdb_new_database(tdb, hash_اندازه) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			اذهب fail;
		}
		اذهب داخلی;
	}

	إذا ((tdb->fd = open(name, open_flags, نما)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		اذهب fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	إذا (tdb->methods->tdb_brlock(tdb, GLOBAL_قفل, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		اذهب fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	إذا ((tdb_flags & TDB_CLEAR_IF_اولی) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, فعال_قفل, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		إذا (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			اذهب fail; /* errno set by ftruncate */
		}
	}

	إذا (اقرأ(tdb->fd, &tdb->سرآیند, حجم(tdb->سرآیند)) != حجم(tdb->سرآیند)
	    || memcmp(tdb->سرآیند.magic_food, TDB_MAGIC_FOOD,
		      حجم(TDB_MAGIC_FOOD)) != 0
	    || (tdb->سرآیند.نسخه != TDB_نسخه
		&& !(rev = (tdb->سرآیند.نسخه==TDB_BYTEREV(TDB_نسخه))))) {
		/* its not a valid database - possibly initialise it */
		إذا (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_اندازه) == -1) {
			errno = EIO; /* ie bad format or something */
			اذهب fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (بیعلامت حرف *)&tdb->سرآیند.نسخه;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_نسخه) ? TDB_BIGENDIAN : 0;
	إذا (!rev)
		tdb->flags &= ~TDB_CONVERT;
	وإلا {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->سرآیند, حجم(tdb->سرآیند));
	}
	إذا (fstat(tdb->fd, &st) == -1)
		اذهب fail;

	إذا (tdb->سرآیند.rwlocks != 0) {
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

	tdb->map_اندازه = st.st_اندازه;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_رکوردها = 0;
	tdb_mmap(tdb);
	إذا (locked) {
		إذا (tdb->methods->tdb_brlock(tdb, فعال_قفل, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			اذهب fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	إذا (tdb_flags & TDB_CLEAR_IF_اولی) {
		/* leave this lock in place to indicate it's in use */
		إذا (tdb->methods->tdb_brlock(tdb, فعال_قفل, F_RDLCK, F_SETLKW, 0, 1) == -1)
			اذهب fail;
	}

	/* if needed, run recovery */
	إذا (tdb_transaction_recover(tdb) == -1) {
		اذهب fail;
	}

 داخلی:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	إذا (tdb->methods->tdb_brlock(tdb, GLOBAL_قفل, F_UNLCK, F_SETLKW, 0, 1) == -1)
		اذهب fail;
	tdb->بعدی = tdbs;
	tdbs = tdb;
	أرجع tdb;

 fail:
	{ صحيح save_errno = errno;

	إذا (!tdb)
		أرجع عدم;

	إذا (tdb->map_ptr) {
		إذا (tdb->flags & TDB_داخلی)
			امن_FREE(tdb->map_ptr);
		وإلا
			tdb_munmap(tdb);
	}
	امن_FREE(tdb->name);
	إذا (tdb->fd != -1)
		إذا (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	امن_FREE(tdb);
	errno = save_errno;
	أرجع عدم;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

فراغ tdb_مجموعه_max_dead(بنية tdb_context *tdb, صحيح max_dead)
{
	tdb->max_dead_رکوردها = max_dead;
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
		إذا (tdb->flags & TDB_داخلی)
			امن_FREE(tdb->map_ptr);
		وإلا
			tdb_munmap(tdb);
	}
	امن_FREE(tdb->name);
	إذا (tdb->fd != -1)
		ret = close(tdb->fd);
	امن_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	لكل (i = &tdbs; *i; i = &(*i)->بعدی) {
		إذا (*i == tdb) {
			*i = tdb->بعدی;
			اكسر;
		}
	}

	memset(tdb, 0, حجم(*tdb));
	امن_FREE(tdb);

	أرجع ret;
}

/* register a logging function */
فراغ tdb_مجموعه_logging_function(بنية tdb_context *tdb,
                              ثابت بنية tdb_logging_context *log_ctx)
{
        tdb->لگاریتم = *log_ctx;
}

فراغ *tdb_get_logging_خصوصی(بنية tdb_context *tdb)
{
	أرجع tdb->لگاریتم.log_خصوصی;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
صحيح tdb_reopen(بنية tdb_context *tdb)
{
	بنية stat st;

	إذا (tdb->flags & TDB_داخلی) {
		أرجع 0; /* Nothing to do. */
	}

	إذا (tdb->رقم_locks != 0 || tdb->global_قفل.شمار) {
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
	إذا ((tdb->flags & TDB_CLEAR_IF_اولی) &&
	    (tdb->methods->tdb_brlock(tdb, فعال_قفل, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
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
صحيح tdb_reopen_all(صحيح والد_longlived)
{
	بنية tdb_context *tdb;

	لكل (tdb=tdbs; tdb; tdb = tdb->بعدی) {
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
		إذا (والد_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_اولی;
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
