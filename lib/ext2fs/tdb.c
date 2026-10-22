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

#اگر_تعریف_شدہ CONFIG_STAND_ALONE
#تعریف_کرو HAVE_MMAP
#تعریف_کرو HAVE_STRDUP
#تعریف_کرو HAVE_SYS_MMAN_H
#تعریف_کرو HAVE_UTIME_H
#تعریف_کرو HAVE_UTIME
#اگر_ختم
#اگر_غیر_تعریف_شدہ __FreeBSD__
#تعریف_کرو XOPEN_ماخذ 600
#اگر_ختم

#شامل "config.h"
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <stdarg.h>
#شامل <stddef.h>
#شامل <errno.h>
#شامل <string.h>
#اگر_تعریف_شدہ HAVE_SYS_SELECT_H
#شامل <sys/select.h>
#اگر_ختم
#شامل <sys/time.h>
#شامل <sys/types.h>
#شامل <time.h>
#اگر_تعریف_شدہ HAVE_UTIME_H
#شامل <utime.h>
#اگر_ختم
#شامل <sys/stat.h>
#شامل <sys/file.h>
#شامل <fcntl.h>

#اگر_تعریف_شدہ HAVE_SYS_MMAN_H
#شامل <sys/mman.h>
#اگر_ختم

#اگر_تعریف_شدہ __GNUC__
#تعریف_کرو EXT2FS_ATTR(x) __attribute__(x)
#ورنہ
#تعریف_کرو EXT2FS_ATTR(x)
#اگر_ختم

#اگر_غیر_تعریف_شدہ MAP_FILE
#تعریف_کرو MAP_FILE 0
#اگر_ختم

#اگر_غیر_تعریف_شدہ MAP_FAILED
#تعریف_کرو MAP_FAILED ((خالی *)-1)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HAVE_STRDUP
#تعریف_کرو strdup rep_strdup
ساکن حرف *rep_strdup(ثابت حرف *s)
{
	حرف *ret;
	صحیح لمبائی;

	اگر (!s)
		واپس تہی;
	لمبائی = لڑی_لمبائی(s);
	ret = حافظہ_دو(لمبائی + 1);
	اگر (ret) {
		strncpy(ret, s, لمبائی);
		ret[لمبائی] = '\0';
	}
	واپس ret;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ PRINTF_ATTRIBUTE
#اگر (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ورنہ
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2)
#اگر_ختم
#اگر_ختم

#اگر defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
قسمتعریف صحیح bool;
#اگر_ختم

#شامل "tdb.h"

ساکن TDB_ڈیٹا tdb_null;

#اگر_غیر_تعریف_شدہ u32
#تعریف_کرو u32 غیرعلامتی
#اگر_ختم

قسمتعریف u32 tdb_len_t;
قسمتعریف u32 tdb_off_t;

#اگر_غیر_تعریف_شدہ offsetof
#تعریف_کرو offsetof(t,f) ((غیرعلامتی صحیح)&((t *)0)->f)
#اگر_ختم

#تعریف_کرو TDB_MAGIC_FOOD "TDB file\n"
#تعریف_کرو TDB_نسخہ (0x26011967 + 6)
#تعریف_کرو TDB_MAGIC (0x26011999U)
#تعریف_کرو TDB_FREE_MAGIC (~TDB_MAGIC)
#تعریف_کرو TDB_DEAD_MAGIC (0xFEE1DEAD)
#تعریف_کرو TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#تعریف_کرو TDB_ALIGNMENT 4
#تعریف_کرو MIN_REC_سائز (2*حجم(ساخت list_struct) + TDB_ALIGNMENT)
#تعریف_کرو DEFAULT_HASH_سائز 131
#تعریف_کرو FREELIST_چوٹی (حجم(ساخت tdb_سرخی))
#تعریف_کرو TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#تعریف_کرو TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#تعریف_کرو TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#تعریف_کرو TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#تعریف_کرو TDB_HASH_چوٹی(hash) (FREELIST_چوٹی + (بالٹی(hash)+1)*حجم(tdb_off_t))
#تعریف_کرو TDB_HASHTABLE_سائز(tdb) ((tdb->سرخی.hash_سائز+1)*حجم(tdb_off_t))
#تعریف_کرو TDB_ڈیٹا_آغاز(hash_سائز) TDB_HASH_چوٹی(hash_سائز-1)
#تعریف_کرو TDB_RECOVERY_سر offsetof(ساخت tdb_سرخی, recovery_آغاز)
#تعریف_کرو TDB_SEQNUM_OFS    offsetof(ساخت tdb_سرخی, sequence_نمبر)
#تعریف_کرو TDB_PAD_BYTE 0x42
#تعریف_کرو TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#تعریف_کرو TDB_LOG(x) tdb->لوگارتھم.log_fn x

/* lock offsets */
#تعریف_کرو GLOBAL_تالا      0
#تعریف_کرو فعال_تالا      4
#تعریف_کرو TRANSACTION_تالا 8

/* free memory if the pointer is valid and zero the pointer */
#اگر_غیر_تعریف_شدہ محفوظ_FREE
#تعریف_کرو محفوظ_FREE(x) کرو { اگر ((x) != تہی) {آزاد(x); (x)=تہی;} } جب(0)
#اگر_ختم

#تعریف_کرو بالٹی(hash) ((hash) % tdb->سرخی.hash_سائز)

#تعریف_کرو DOCONV() (tdb->flags & TDB_CONVERT)
#تعریف_کرو CONVERT(x) (DOCONV() ? tdb_convert(&x, حجم(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ساخت list_struct {
	tdb_off_t اگلا; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t کلید_len; /* byte length of key */
	tdb_len_t ڈیٹا_len; /* byte length of data */
	u32 بھرا_hash; /* the full 32 bit hash of the key */
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
ساخت tdb_سرخی {
	حرف magic_food[32]; /* for /etc/magic */
	u32 نسخہ; /* version of the code */
	u32 hash_سائز; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_آغاز; /* offset of transaction recovery region */
	tdb_off_t sequence_نمبر; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ساخت tdb_تالا_type {
	صحیح list;
	u32 گنتی;
	u32 ltype;
};

ساخت tdb_traverse_تالا {
	ساخت tdb_traverse_تالا *اگلا;
	u32 off;
	u32 hash;
	صحیح تالا_rw;
};


ساخت tdb_methods {
	صحیح (*tdb_read)(ساخت tdb_context *, tdb_off_t , خالی *, tdb_len_t , صحیح );
	صحیح (*tdb_write)(ساخت tdb_context *, tdb_off_t, ثابت خالی *, tdb_len_t);
	خالی (*اگلا_hash_chain)(ساخت tdb_context *, u32 *);
	صحیح (*tdb_oob)(ساخت tdb_context *, tdb_off_t , صحیح );
	صحیح (*tdb_expand_file)(ساخت tdb_context *, tdb_off_t , tdb_off_t );
	صحیح (*tdb_brlock)(ساخت tdb_context *, tdb_off_t , صحیح, صحیح, صحیح, size_t);
};

ساخت tdb_context {
	حرف *name; /* the name of the database */
	خالی *map_ptr; /* where it is currently mapped */
	صحیح fd; /* open file descriptor for the database */
	tdb_len_t map_سائز; /* how much space has been mapped */
	صحیح read_only; /* opened read-only */
	صحیح traverse_read; /* read-only traversal */
	ساخت tdb_تالا_type global_تالا;
	صحیح عدد_lockrecs;
	ساخت tdb_تالا_type *lockrecs; /* only real locks, all with count>0 */
	شمار TDB_ERROR ecode; /* error code for last tdb error */
	ساخت tdb_سرخی سرخی; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ساخت tdb_traverse_تالا travlocks; /* current traversal locks */
	ساخت tdb_context *اگلا; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ساخت tdb_logging_context لوگارتھم;
	غیرعلامتی صحیح (*hash_fn)(TDB_ڈیٹا *کلید);
	صحیح open_flags; /* flags used in the open - needed by reopen */
	غیرعلامتی صحیح عدد_locks; /* number of chain locks held */
	ثابت ساخت tdb_methods *methods;
	ساخت tdb_transaction *transaction;
	صحیح صفحہ_سائز;
	صحیح max_dead_اندراجات;
	bool have_transaction_تالا;
	tdb_len_t real_map_سائز; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ساکن صحیح tdb_munmap(ساخت tdb_context *tdb);
ساکن خالی tdb_mmap(ساخت tdb_context *tdb);
ساکن صحیح tdb_تالا(ساخت tdb_context *tdb, صحیح list, صحیح ltype);
صحیح tdb_تالا_nonblock(ساخت tdb_context *tdb, صحیح list, صحیح ltype);
ساکن صحیح tdb_unlock(ساخت tdb_context *tdb, صحیح list, صحیح ltype);
ساکن صحیح tdb_brlock(ساخت tdb_context *tdb, tdb_off_t انحراف, صحیح rw_type, صحیح lck_type, صحیح probe, size_t len);
ساکن صحیح tdb_transaction_تالا(ساخت tdb_context *tdb, صحیح ltype);
ساکن صحیح tdb_transaction_unlock(ساخت tdb_context *tdb);
ساکن صحیح tdb_brlock_upgrade(ساخت tdb_context *tdb, tdb_off_t انحراف, size_t len);
ساکن صحیح tdb_write_تالا_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off);
ساکن صحیح tdb_write_unlock_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off);
ساکن صحیح tdb_ofs_read(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_off_t *d);
ساکن صحیح tdb_ofs_write(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_off_t *d);
ساکن خالی *tdb_convert(خالی *buf, u32 سائز);
ساکن صحیح tdb_free(ساخت tdb_context *tdb, tdb_off_t انحراف, ساخت list_struct *rec);
ساکن tdb_off_t tdb_allocate(ساخت tdb_context *tdb, tdb_len_t لمبائی, ساخت list_struct *rec);
ساکن صحیح tdb_ofs_read(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_off_t *d);
ساکن صحیح tdb_ofs_write(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_off_t *d);
ساکن صحیح tdb_تالا_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off);
ساکن صحیح tdb_unlock_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off);
ساکن صحیح tdb_rec_read(ساخت tdb_context *tdb, tdb_off_t انحراف, ساخت list_struct *rec);
ساکن صحیح tdb_rec_write(ساخت tdb_context *tdb, tdb_off_t انحراف, ساخت list_struct *rec);
ساکن صحیح tdb_do_delete(ساخت tdb_context *tdb, tdb_off_t rec_ptr, ساخت list_struct *rec);
ساکن غیرعلامتی حرف *tdb_alloc_read(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_len_t len);
ساکن صحیح tdb_parse_ڈیٹا(ساخت tdb_context *tdb, TDB_ڈیٹا کلید,
		   tdb_off_t انحراف, tdb_len_t len,
		   صحیح (*parser)(TDB_ڈیٹا کلید, TDB_ڈیٹا ڈیٹا,
				 خالی *نجی_ڈیٹا),
		   خالی *نجی_ڈیٹا);
ساکن tdb_off_t tdb_ڈھونڈو_تالا_hash(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, u32 hash, صحیح locktype,
			   ساخت list_struct *rec);
ساکن خالی tdb_io_init(ساخت tdb_context *tdb);
ساکن صحیح tdb_expand(ساخت tdb_context *tdb, tdb_off_t سائز);
ساکن صحیح tdb_rec_free_read(ساخت tdb_context *tdb, tdb_off_t off,
		      ساخت list_struct *rec);


/* file: error.c */

شمار TDB_ERROR tdb_error(ساخت tdb_context *tdb)
{
	واپس tdb->ecode;
}

ساکن ساخت tdb_errname {
	شمار TDB_ERROR ecode; ثابت حرف *estring;
} emap[] = { {TDB_SUCCESS, "Success"},
	     {TDB_ERR_CORRUPT, "Corrupt database"},
	     {TDB_ERR_IO, "IO Error"},
	     {TDB_ERR_تالا, "Locking error"},
	     {TDB_ERR_OOM, "Out of memory"},
	     {TDB_ERR_EXISTS, "Record exists"},
	     {TDB_ERR_NOLOCK, "Lock exists on other keys"},
	     {TDB_ERR_EINVAL, "Invalid parameter"},
	     {TDB_ERR_NOEXIST, "Record does not exist"},
	     {TDB_ERR_RDONLY, "write not permitted"} };

/* Error string for the last tdb error */
ثابت حرف *tdb_errorstr(ساخت tdb_context *tdb)
{
	u32 i;
	برائے (i = 0; i < حجم(emap) / حجم(ساخت tdb_errname); i++)
		اگر (tdb->ecode == emap[i].ecode)
			واپس emap[i].estring;
	واپس "Invalid error code";
}

/* file: lock.c */

#تعریف_کرو TDB_MARK_تالا 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
صحیح tdb_brlock(ساخت tdb_context *tdb, tdb_off_t انحراف,
	       صحیح rw_type, صحیح lck_type, صحیح probe, size_t len)
{
	ساخت flock fl;
	صحیح ret;

	اگر (tdb->flags & TDB_NOLOCK) {
		واپس 0;
	}

	اگر ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		واپس -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_آغاز = انحراف;
	fl.l_len = len;
	fl.l_pid = 0;

	کرو {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} جب (ret == -1 && errno == EINTR);

	اگر (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		اگر (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_تالا;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, انحراف, rw_type, lck_type, (صحیح)len));
		}
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}
	واپس 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
صحیح tdb_brlock_upgrade(ساخت tdb_context *tdb, tdb_off_t انحراف, size_t len)
{
	صحیح گنتی = 1000;
	جب (گنتی--) {
		ساخت timeval tv;
		اگر (tdb_brlock(tdb, انحراف, F_WRLCK, F_SETLKW, 1, len) == 0) {
			واپس 0;
		}
		اگر (errno != EDEADLK) {
			توڑو;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, تہی, تہی, تہی, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", انحراف));
	واپس -1;
}


/* lock a list in the database. list -1 is the alloc list */
ساکن صحیح tdb_تالا_2(ساخت tdb_context *tdb, صحیح list, صحیح ltype, صحیح op)
{
	ساخت tdb_تالا_type *new_lck;
	صحیح i;
	bool mark_تالا = ((ltype & TDB_MARK_تالا) == TDB_MARK_تالا);

	ltype &= ~TDB_MARK_تالا;

	/* a global lock allows us to avoid per chain locks */
	اگر (tdb->global_تالا.گنتی &&
	    ((u32)ltype == tdb->global_تالا.ltype || ltype == F_RDLCK)) {
		واپس 0;
	}

	اگر (tdb->global_تالا.گنتی) {
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}

	اگر (list < -1 || list >= (صحیح)tdb->سرخی.hash_سائز) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		واپس -1;
	}
	اگر (tdb->flags & TDB_NOLOCK)
		واپس 0;

	برائے (i=0; i<tdb->عدد_lockrecs; i++) {
		اگر (tdb->lockrecs[i].list == list) {
			اگر (tdb->lockrecs[i].گنتی == 0) {
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
			واپس 0;
		}
	}

	new_lck = (ساخت tdb_تالا_type *)realloc(
		tdb->lockrecs,
		حجم(*tdb->lockrecs) * (tdb->عدد_lockrecs+1));
	اگر (new_lck == تہی) {
		errno = ENOMEM;
		واپس -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	اگر (!mark_تالا &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_چوٹی+4*list, ltype, op,
				     0, 1)) {
		واپس -1;
	}

	tdb->عدد_locks++;

	tdb->lockrecs[tdb->عدد_lockrecs].list = list;
	tdb->lockrecs[tdb->عدد_lockrecs].گنتی = 1;
	tdb->lockrecs[tdb->عدد_lockrecs].ltype = ltype;
	tdb->عدد_lockrecs += 1;

	واپس 0;
}

/* lock a list in the database. list -1 is the alloc list */
صحیح tdb_تالا(ساخت tdb_context *tdb, صحیح list, صحیح ltype)
{
	صحیح ret;
	ret = tdb_تالا_2(tdb, list, ltype, F_SETLKW);
	اگر (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	واپس ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
صحیح tdb_تالا_nonblock(ساخت tdb_context *tdb, صحیح list, صحیح ltype)
{
	واپس tdb_تالا_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
صحیح tdb_unlock(ساخت tdb_context *tdb, صحیح list, صحیح ltype)
{
	صحیح ret = -1;
	صحیح i;
	ساخت tdb_تالا_type *lck = تہی;
	bool mark_تالا = ((ltype & TDB_MARK_تالا) == TDB_MARK_تالا);

	ltype &= ~TDB_MARK_تالا;

	/* a global lock allows us to avoid per chain locks */
	اگر (tdb->global_تالا.گنتی &&
	    ((u32)ltype == tdb->global_تالا.ltype || ltype == F_RDLCK)) {
		واپس 0;
	}

	اگر (tdb->global_تالا.گنتی) {
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}

	اگر (tdb->flags & TDB_NOLOCK)
		واپس 0;

	/* Sanity checks */
	اگر (list < -1 || list >= (صحیح)tdb->سرخی.hash_سائز) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->سرخی.hash_سائز));
		واپس ret;
	}

	برائے (i=0; i<tdb->عدد_lockrecs; i++) {
		اگر (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			توڑو;
		}
	}

	اگر ((lck == تہی) || (lck->گنتی == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		واپس -1;
	}

	اگر (lck->گنتی > 1) {
		lck->گنتی--;
		واپس 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	اگر (mark_تالا) {
		ret = 0;
	} ورنہ {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_چوٹی+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->عدد_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	اگر (tdb->عدد_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->عدد_lockrecs-1];
	}
	tdb->عدد_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	اگر (tdb->عدد_lockrecs == 0) {
		محفوظ_FREE(tdb->lockrecs);
	}

	اگر (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	واپس ret;
}

/*
  get the transaction lock
 */
صحیح tdb_transaction_تالا(ساخت tdb_context *tdb, صحیح ltype)
{
	اگر (tdb->have_transaction_تالا || tdb->global_تالا.گنتی) {
		واپس 0;
	}
	اگر (tdb->methods->tdb_brlock(tdb, TRANSACTION_تالا, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_تالا;
		واپس -1;
	}
	tdb->have_transaction_تالا = 1;
	واپس 0;
}

/*
  release the transaction lock
 */
صحیح tdb_transaction_unlock(ساخت tdb_context *tdb)
{
	صحیح ret;
	اگر (!tdb->have_transaction_تالا) {
		واپس 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_تالا, F_UNLCK, F_SETLKW, 0, 1);
	اگر (ret == 0) {
		tdb->have_transaction_تالا = 0;
	}
	واپس ret;
}




/* lock/unlock entire database */
ساکن صحیح _tdb_lockall(ساخت tdb_context *tdb, صحیح ltype, صحیح op)
{
	bool mark_تالا = ((ltype & TDB_MARK_تالا) == TDB_MARK_تالا);

	ltype &= ~TDB_MARK_تالا;

	/* There are no locks on read-only dbs */
	اگر (tdb->read_only || tdb->traverse_read)
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);

	اگر (tdb->global_تالا.گنتی && tdb->global_تالا.ltype == (u32)ltype) {
		tdb->global_تالا.گنتی++;
		واپس 0;
	}

	اگر (tdb->global_تالا.گنتی) {
		/* a global lock of a different type exists */
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}

	اگر (tdb->عدد_locks != 0) {
		/* can't combine global and chain locks */
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}

	اگر (!mark_تالا &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_چوٹی, ltype, op,
				     0, 4*tdb->سرخی.hash_سائز)) {
		اگر (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		واپس -1;
	}

	tdb->global_تالا.گنتی = 1;
	tdb->global_تالا.ltype = ltype;

	واپس 0;
}



/* unlock entire db */
ساکن صحیح _tdb_unlockall(ساخت tdb_context *tdb, صحیح ltype)
{
	bool mark_تالا = ((ltype & TDB_MARK_تالا) == TDB_MARK_تالا);

	ltype &= ~TDB_MARK_تالا;

	/* There are no locks on read-only dbs */
	اگر (tdb->read_only || tdb->traverse_read) {
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}

	اگر (tdb->global_تالا.ltype != (u32)ltype ||
	    tdb->global_تالا.گنتی == 0) {
		واپس TDB_ERRCODE(TDB_ERR_تالا, -1);
	}

	اگر (tdb->global_تالا.گنتی > 1) {
		tdb->global_تالا.گنتی--;
		واپس 0;
	}

	اگر (!mark_تالا &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW,
				     0, 4*tdb->سرخی.hash_سائز)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		واپس -1;
	}

	tdb->global_تالا.گنتی = 0;
	tdb->global_تالا.ltype = 0;

	واپس 0;
}

/* lock entire database with write lock */
صحیح tdb_lockall(ساخت tdb_context *tdb)
{
	واپس _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
صحیح tdb_lockall_mark(ساخت tdb_context *tdb)
{
	واپس _tdb_lockall(tdb, F_WRLCK | TDB_MARK_تالا, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
صحیح tdb_lockall_unmark(ساخت tdb_context *tdb)
{
	واپس _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_تالا);
}

/* lock entire database with write lock - nonblocking variant */
صحیح tdb_lockall_nonblock(ساخت tdb_context *tdb)
{
	واپس _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
صحیح tdb_unlockall(ساخت tdb_context *tdb)
{
	واپس _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
صحیح tdb_lockall_read(ساخت tdb_context *tdb)
{
	واپس _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
صحیح tdb_lockall_read_nonblock(ساخت tdb_context *tdb)
{
	واپس _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
صحیح tdb_unlockall_read(ساخت tdb_context *tdb)
{
	واپس _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
صحیح tdb_chainlock(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_تالا(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
صحیح tdb_chainlock_nonblock(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_تالا_nonblock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
صحیح tdb_chainlock_mark(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_تالا(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_تالا);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
صحیح tdb_chainlock_unmark(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_unlock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK | TDB_MARK_تالا);
}

صحیح tdb_chainunlock(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_unlock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_WRLCK);
}

صحیح tdb_chainlock_read(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_تالا(tdb, بالٹی(tdb->hash_fn(&کلید)), F_RDLCK);
}

صحیح tdb_chainunlock_read(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	واپس tdb_unlock(tdb, بالٹی(tdb->hash_fn(&کلید)), F_RDLCK);
}



/* record lock stops delete underneath */
صحیح tdb_تالا_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off)
{
	واپس off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحیح tdb_write_تالا_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off)
{
	ساخت tdb_traverse_تالا *i;
	برائے (i = &tdb->travlocks; i; i = i->اگلا)
		اگر (i->off == off)
			واپس -1;
	واپس tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
صحیح tdb_write_unlock_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off)
{
	واپس tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
صحیح tdb_unlock_ریکارڈ(ساخت tdb_context *tdb, tdb_off_t off)
{
	ساخت tdb_traverse_تالا *i;
	u32 گنتی = 0;

	اگر (off == 0)
		واپس 0;
	برائے (i = &tdb->travlocks; i; i = i->اگلا)
		اگر (i->off == off)
			گنتی++;
	واپس (گنتی == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ساکن صحیح tdb_oob(ساخت tdb_context *tdb, tdb_off_t len, صحیح probe)
{
	ساخت stat st;
	اگر (len <= tdb->map_سائز)
		واپس 0;
	اگر (tdb->flags & TDB_اندرونی) {
		اگر (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (صحیح)len, (صحیح)tdb->map_سائز));
		}
		واپس TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	اگر (fstat(tdb->fd, &st) == -1) {
		واپس TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	اگر (st.st_سائز < (off_t)len) {
		اگر (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (صحیح)len, (صحیح)st.st_سائز));
		}
		واپس TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	اگر (tdb_munmap(tdb) == -1)
		واپس TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_سائز = st.st_سائز;
	tdb_mmap(tdb);
	واپس 0;
}

/* write a lump of data at a specified offset */
ساکن صحیح tdb_write(ساخت tdb_context *tdb, tdb_off_t off,
		     ثابت خالی *buf, tdb_len_t len)
{
	اگر (len == 0) {
		واپس 0;
	}

	اگر (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		واپس -1;
	}

	اگر (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		واپس -1;

	اگر (tdb->map_ptr) {
		حافظہ_نقل(off + (حرف *)tdb->map_ptr, buf, len);
	} ورنہ اگر (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		واپس TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	واپس 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
خالی *tdb_convert(خالی *buf, u32 سائز)
{
	u32 i, *p = (u32 *)buf;
	برائے (i = 0; i < سائز / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	واپس buf;
}


/* read a lump of data at a specified offset, maybe convert */
ساکن صحیح tdb_read(ساخت tdb_context *tdb, tdb_off_t off, خالی *buf,
		    tdb_len_t len, صحیح cv)
{
	اگر (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		واپس -1;
	}

	اگر (tdb->map_ptr) {
		حافظہ_نقل(buf, off + (حرف *)tdb->map_ptr, len);
	} ورنہ {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		اگر (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (صحیح)off, (صحیح)len, (صحیح)ret, strerror(errno),
				 (صحیح)tdb->map_سائز));
			واپس TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	اگر (cv) {
		tdb_convert(buf, len);
	}
	واپس 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ساکن خالی tdb_اگلا_hash_chain(ساخت tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	اگر (tdb->map_ptr) {
		برائے (;h < tdb->سرخی.hash_سائز;h++) {
			اگر (0 != *(u32 *)(TDB_HASH_چوٹی(h) + (غیرعلامتی حرف *)tdb->map_ptr)) {
				توڑو;
			}
		}
	} ورنہ {
		u32 off=0;
		برائے (;h < tdb->سرخی.hash_سائز;h++) {
			اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(h), &off) != 0 || off != 0) {
				توڑو;
			}
		}
	}
	(*chain) = h;
}


صحیح tdb_munmap(ساخت tdb_context *tdb)
{
	اگر (tdb->flags & TDB_اندرونی)
		واپس 0;

#اگر_تعریف_شدہ HAVE_MMAP
	اگر (tdb->map_ptr) {
		صحیح ret = munmap(tdb->map_ptr, tdb->real_map_سائز);
		اگر (ret != 0)
			واپس ret;
		tdb->real_map_سائز = 0;
	}
#اگر_ختم
	tdb->map_ptr = تہی;
	واپس 0;
}

خالی tdb_mmap(ساخت tdb_context *tdb)
{
	اگر (tdb->flags & TDB_اندرونی)
		واپس;

#اگر_تعریف_شدہ HAVE_MMAP
	اگر (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(تہی, tdb->map_سائز,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		اگر (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_سائز = 0;
			tdb->map_ptr = تہی;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_سائز, strerror(errno)));
		}
		tdb->real_map_سائز = tdb->map_سائز;
	} ورنہ {
		tdb->map_ptr = تہی;
	}
#ورنہ
	tdb->map_ptr = تہی;
#اگر_ختم
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ساکن صحیح tdb_expand_file(ساخت tdb_context *tdb, tdb_off_t سائز, tdb_off_t addition)
{
	حرف buf[1024];

	اگر (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		واپس -1;
	}

	اگر (ftruncate(tdb->fd, سائز+addition) == -1) {
		حرف b = 0;
		اگر (pwrite(tdb->fd,  &b, 1, (سائز+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 سائز+addition, strerror(errno)));
			واپس -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, حجم(buf));
	جب (addition) {
		صحیح n = addition>حجم(buf)?حجم(buf):addition;
		صحیح ret = pwrite(tdb->fd, buf, n, سائز);
		اگر (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			واپس -1;
		}
		addition -= n;
		سائز += n;
	}
	واپس 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
صحیح tdb_expand(ساخت tdb_context *tdb, tdb_off_t سائز)
{
	ساخت list_struct rec;
	tdb_off_t انحراف;

	اگر (tdb_تالا(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		واپس -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_سائز + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	سائز = TDB_ALIGN(tdb->map_سائز + سائز*10, tdb->صفحہ_سائز) - tdb->map_سائز;

	اگر (!(tdb->flags & TDB_اندرونی))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	اگر (!(tdb->flags & TDB_اندرونی)) {
		اگر (tdb->methods->tdb_expand_file(tdb, tdb->map_سائز, سائز) != 0)
			جاؤ fail;
	}

	tdb->map_سائز += سائز;

	اگر (tdb->flags & TDB_اندرونی) {
		حرف *new_map_ptr = (حرف *)realloc(tdb->map_ptr,
						    tdb->map_سائز);
		اگر (!new_map_ptr) {
			tdb->map_سائز -= سائز;
			جاؤ fail;
		}
		tdb->map_ptr = new_map_ptr;
	} ورنہ {
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
	rec.rec_len = سائز - حجم(rec);

	/* link it into the free list */
	انحراف = tdb->map_سائز - سائز;
	اگر (tdb_free(tdb, انحراف, &rec) == -1)
		جاؤ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	واپس 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	واپس -1;
}

/* read/write a tdb_off_t */
صحیح tdb_ofs_read(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_off_t *d)
{
	واپس tdb->methods->tdb_read(tdb, انحراف, (حرف*)d, حجم(*d), DOCONV());
}

صحیح tdb_ofs_write(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_off_t *d)
{
	tdb_off_t off = *d;
	واپس tdb->methods->tdb_write(tdb, انحراف, CONVERT(off), حجم(*d));
}


/* read a lump of data, allocating the space for it */
غیرعلامتی حرف *tdb_alloc_read(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_len_t len)
{
	غیرعلامتی حرف *buf;

	/* some systems don't like zero length malloc */
	اگر (len == 0) {
		len = 1;
	}

	اگر (!(buf = (غیرعلامتی حرف *)حافظہ_دو(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		واپس TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	اگر (tdb->methods->tdb_read(tdb, انحراف, buf, len, 0) == -1) {
		محفوظ_FREE(buf);
		واپس تہی;
	}
	واپس buf;
}

/* Give a piece of tdb data to a parser */

صحیح tdb_parse_ڈیٹا(ساخت tdb_context *tdb, TDB_ڈیٹا کلید,
		   tdb_off_t انحراف, tdb_len_t len,
		   صحیح (*parser)(TDB_ڈیٹا کلید, TDB_ڈیٹا ڈیٹا,
				 خالی *نجی_ڈیٹا),
		   خالی *نجی_ڈیٹا)
{
	TDB_ڈیٹا ڈیٹا;
	صحیح نتیجہ;

	ڈیٹا.dsize = len;

	اگر ((tdb->transaction == تہی) && (tdb->map_ptr != تہی)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		اگر (tdb->methods->tdb_oob(tdb, انحراف+len, 0) != 0) {
			واپس -1;
		}
		ڈیٹا.dptr = انحراف + (غیرعلامتی حرف *)tdb->map_ptr;
		واپس parser(کلید, ڈیٹا, نجی_ڈیٹا);
	}

	اگر (!(ڈیٹا.dptr = tdb_alloc_read(tdb, انحراف, len))) {
		واپس -1;
	}

	نتیجہ = parser(کلید, ڈیٹا, نجی_ڈیٹا);
	آزاد(ڈیٹا.dptr);
	واپس نتیجہ;
}

/* read/write a record */
صحیح tdb_rec_read(ساخت tdb_context *tdb, tdb_off_t انحراف, ساخت list_struct *rec)
{
	اگر (tdb->methods->tdb_read(tdb, انحراف, rec, حجم(*rec),DOCONV()) == -1)
		واپس -1;
	اگر (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, انحراف));
		واپس TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	واپس tdb->methods->tdb_oob(tdb, rec->اگلا+حجم(*rec), 0);
}

صحیح tdb_rec_write(ساخت tdb_context *tdb, tdb_off_t انحراف, ساخت list_struct *rec)
{
	ساخت list_struct r = *rec;
	واپس tdb->methods->tdb_write(tdb, انحراف, CONVERT(r), حجم(r));
}

ساکن ثابت ساخت tdb_methods io_methods = {
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
خالی tdb_io_init(ساخت tdb_context *tdb)
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

ساخت tdb_transaction_el {
	ساخت tdb_transaction_el *اگلا, *پچھلا;
	tdb_off_t انحراف;
	tdb_len_t لمبائی;
	غیرعلامتی حرف *ڈیٹا;
};

/*
  hold the context of any current transaction
*/
ساخت tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ثابت ساخت tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ساخت tdb_transaction_el *elements, *elements_آخری;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	صحیح transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	صحیح nesting;

	/* old file size before transaction */
	tdb_len_t old_map_سائز;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ساکن صحیح transaction_read(ساخت tdb_context *tdb, tdb_off_t off, خالی *buf,
			    tdb_len_t len, صحیح cv)
{
	ساخت tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	برائے (el=tdb->transaction->elements_آخری;el;el=el->پچھلا) {
		tdb_len_t partial;

		اگر (off+len <= el->انحراف) {
			جاری;
		}
		اگر (off >= el->انحراف + el->لمبائی) {
			جاری;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		اگر (off < el->انحراف) {
			partial = el->انحراف - off;
			اگر (transaction_read(tdb, off, buf, partial, cv) != 0) {
				جاؤ fail;
			}
			len -= partial;
			off += partial;
			buf = (خالی *)(partial + (حرف *)buf);
		}
		اگر (off + len <= el->انحراف + el->لمبائی) {
			partial = len;
		} ورنہ {
			partial = el->انحراف + el->لمبائی - off;
		}
		حافظہ_نقل(buf, el->ڈیٹا + (off - el->انحراف), partial);
		اگر (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (خالی *)(partial + (حرف *)buf);

		اگر (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			جاؤ fail;
		}

		واپس 0;
	}

	/* its not in the transaction elements - do a real read */
	واپس tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	واپس -1;
}


/*
  write while in a transaction
*/
ساکن صحیح transaction_write(ساخت tdb_context *tdb, tdb_off_t off,
			     ثابت خالی *buf, tdb_len_t len)
{
	ساخت tdb_transaction_el *el, *بہترین_el=تہی;

	اگر (len == 0) {
		واپس 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	اگر (len == حجم(tdb_off_t) && off >= FREELIST_چوٹی &&
	    off < FREELIST_چوٹی+TDB_HASHTABLE_سائز(tdb)) {
		u32 chain = (off-FREELIST_چوٹی) / حجم(tdb_off_t);
		حافظہ_نقل(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	برائے (el=tdb->transaction->elements_آخری;el;el=el->پچھلا) {
		tdb_len_t partial;

		اگر (بہترین_el == تہی && off == el->انحراف+el->لمبائی) {
			بہترین_el = el;
		}

		اگر (off+len <= el->انحراف) {
			جاری;
		}
		اگر (off >= el->انحراف + el->لمبائی) {
			جاری;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		اگر (off < el->انحراف) {
			partial = el->انحراف - off;
			اگر (transaction_write(tdb, off, buf, partial) != 0) {
				جاؤ fail;
			}
			len -= partial;
			off += partial;
			buf = (ثابت خالی *)(partial + (ثابت حرف *)buf);
		}
		اگر (off + len <= el->انحراف + el->لمبائی) {
			partial = len;
		} ورنہ {
			partial = el->انحراف + el->لمبائی - off;
		}
		حافظہ_نقل(el->ڈیٹا + (off - el->انحراف), buf, partial);
		len -= partial;
		off += partial;
		buf = (ثابت خالی *)(partial + (ثابت حرف *)buf);

		اگر (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			جاؤ fail;
		}

		واپس 0;
	}

	/* see if we can append the new entry to an existing entry */
	اگر (بہترین_el && بہترین_el->انحراف + بہترین_el->لمبائی == off &&
	    (off+len < tdb->transaction->old_map_سائز ||
	     off > tdb->transaction->old_map_سائز)) {
		غیرعلامتی حرف *ڈیٹا = بہترین_el->ڈیٹا;
		el = بہترین_el;
		el->ڈیٹا = (غیرعلامتی حرف *)realloc(el->ڈیٹا,
						    el->لمبائی + len);
		اگر (el->ڈیٹا == تہی) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->ڈیٹا = ڈیٹا;
			واپس -1;
		}
		اگر (buf) {
			حافظہ_نقل(el->ڈیٹا + el->لمبائی, buf, len);
		} ورنہ {
			memset(el->ڈیٹا + el->لمبائی, TDB_PAD_BYTE, len);
		}
		el->لمبائی += len;
		واپس 0;
	}

	/* add a new entry at the end of the list */
	el = (ساخت tdb_transaction_el *)حافظہ_دو(حجم(*el));
	اگر (el == تہی) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		واپس -1;
	}
	el->اگلا = تہی;
	el->پچھلا = tdb->transaction->elements_آخری;
	el->انحراف = off;
	el->لمبائی = len;
	el->ڈیٹا = (غیرعلامتی حرف *)حافظہ_دو(len);
	اگر (el->ڈیٹا == تہی) {
		آزاد(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		واپس -1;
	}
	اگر (buf) {
		حافظہ_نقل(el->ڈیٹا, buf, len);
	} ورنہ {
		memset(el->ڈیٹا, TDB_PAD_BYTE, len);
	}
	اگر (el->پچھلا) {
		el->پچھلا->اگلا = el;
	} ورنہ {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_آخری = el;
	واپس 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	واپس -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ساکن خالی transaction_اگلا_hash_chain(ساخت tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	برائے (;h < tdb->سرخی.hash_سائز;h++) {
		/* the +1 takes account of the freelist */
		اگر (0 != tdb->transaction->hash_heads[h+1]) {
			توڑو;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ساکن صحیح transaction_oob(ساخت tdb_context *tdb, tdb_off_t len,
			   صحیح probe EXT2FS_ATTR((غیر_مستعمل)))
{
	اگر (len <= tdb->map_سائز) {
		واپس 0;
	}
	واپس TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ساکن صحیح transaction_expand_file(ساخت tdb_context *tdb, tdb_off_t سائز,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	اگر (transaction_write(tdb, سائز, تہی, addition) != 0) {
		واپس -1;
	}

	واپس 0;
}

/*
  brlock during a transaction - ignore them
*/
ساکن صحیح transaction_brlock(ساخت tdb_context *tdb EXT2FS_ATTR((غیر_مستعمل)),
			      tdb_off_t انحراف EXT2FS_ATTR((غیر_مستعمل)),
			      صحیح rw_type EXT2FS_ATTR((غیر_مستعمل)),
			      صحیح lck_type EXT2FS_ATTR((غیر_مستعمل)),
			      صحیح probe EXT2FS_ATTR((غیر_مستعمل)),
			      size_t len EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس 0;
}

ساکن ثابت ساخت tdb_methods transaction_methods = {
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
صحیح tdb_transaction_آغاز(ساخت tdb_context *tdb)
{
	/* some sanity checks */
	اگر (tdb->read_only || (tdb->flags & TDB_اندرونی) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		واپس -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	اگر (tdb->transaction != تہی) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		واپس 0;
	}

	اگر (tdb->عدد_locks != 0 || tdb->global_تالا.گنتی) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_تالا;
		واپس -1;
	}

	اگر (tdb->travlocks.اگلا != تہی) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_تالا;
		واپس -1;
	}

	tdb->transaction = (ساخت tdb_transaction *)
		صفر_حافظہ_دو(حجم(ساخت tdb_transaction), 1);
	اگر (tdb->transaction == تہی) {
		tdb->ecode = TDB_ERR_OOM;
		واپس -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	اگر (tdb_transaction_تالا(tdb, F_WRLCK) == -1) {
		محفوظ_FREE(tdb->transaction);
		واپس -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	اگر (tdb_brlock(tdb, FREELIST_چوٹی, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_تالا;
		جاؤ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		صفر_حافظہ_دو(tdb->سرخی.hash_سائز+1, حجم(u32));
	اگر (tdb->transaction->hash_heads == تہی) {
		tdb->ecode = TDB_ERR_OOM;
		جاؤ fail;
	}
	اگر (tdb->methods->tdb_read(tdb, FREELIST_چوٹی, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_سائز(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		جاؤ fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_سائز + 1, 1);
	tdb->transaction->old_map_سائز = tdb->map_سائز;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	اگر (transaction_write(tdb, FREELIST_چوٹی, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_سائز(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		جاؤ fail;
	}

	واپس 0;

fail:
	tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	محفوظ_FREE(tdb->transaction->hash_heads);
	محفوظ_FREE(tdb->transaction);
	واپس -1;
}


/*
  cancel the current transaction
*/
صحیح tdb_transaction_cancel(ساخت tdb_context *tdb)
{
	اگر (tdb->transaction == تہی) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		واپس -1;
	}

	اگر (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		واپس 0;
	}

	tdb->map_سائز = tdb->transaction->old_map_سائز;

	/* free all the transaction elements */
	جب (tdb->transaction->elements) {
		ساخت tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->اگلا;
		آزاد(el->ڈیٹا);
		آزاد(el);
	}

	/* remove any global lock created during the transaction */
	اگر (tdb->global_تالا.گنتی != 0) {
		tdb_brlock(tdb, FREELIST_چوٹی, F_UNLCK, F_SETLKW, 0, 4*tdb->سرخی.hash_سائز);
		tdb->global_تالا.گنتی = 0;
	}

	/* remove any locks created during the transaction */
	اگر (tdb->عدد_locks != 0) {
		صحیح i;
		برائے (i=0;i<tdb->عدد_lockrecs;i++) {
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

	واپس 0;
}

/*
  sync to disk
*/
ساکن صحیح transaction_sync(ساخت tdb_context *tdb, tdb_off_t انحراف, tdb_len_t لمبائی)
{
	اگر (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		واپس -1;
	}
#اگر defined(HAVE_MSYNC) && defined(MS_SYNC)
	اگر (tdb->map_ptr) {
		tdb_off_t moffset = انحراف & ~(tdb->صفحہ_سائز-1);
		اگر (msync(moffset + (حرف *)tdb->map_ptr,
			  لمبائی + (انحراف - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			واپس -1;
		}
	}
#اگر_ختم
	واپس 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ساکن tdb_len_t tdb_recovery_سائز(ساخت tdb_context *tdb)
{
	ساخت tdb_transaction_el *el;
	tdb_len_t recovery_سائز = 0;

	recovery_سائز = حجم(u32);
	برائے (el=tdb->transaction->elements;el;el=el->اگلا) {
		اگر (el->انحراف >= tdb->transaction->old_map_سائز) {
			جاری;
		}
		recovery_سائز += 2*حجم(tdb_off_t) + el->لمبائی;
	}

	واپس recovery_سائز;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ساکن صحیح tdb_recovery_allocate(ساخت tdb_context *tdb,
				 tdb_len_t *recovery_سائز,
				 tdb_off_t *recovery_انحراف,
				 tdb_len_t *recovery_max_سائز)
{
	ساخت list_struct rec;
	ثابت ساخت tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_سر;

	اگر (tdb_ofs_read(tdb, TDB_RECOVERY_سر, &recovery_سر) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		واپس -1;
	}

	rec.rec_len = 0;

	اگر (recovery_سر != 0 &&
	    methods->tdb_read(tdb, recovery_سر, &rec, حجم(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		واپس -1;
	}

	*recovery_سائز = tdb_recovery_سائز(tdb);

	اگر (recovery_سر != 0 && *recovery_سائز <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_سائز = rec.rec_len;
		*recovery_انحراف = recovery_سر;
		واپس 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	اگر (recovery_سر != 0) {
		اگر (tdb_free(tdb, recovery_سر, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			واپس -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_سائز = tdb_recovery_سائز(tdb);

	/* round up to a multiple of page size */
	*recovery_max_سائز = TDB_ALIGN(حجم(rec) + *recovery_سائز, tdb->صفحہ_سائز) - حجم(rec);
	*recovery_انحراف = tdb->map_سائز;
	recovery_سر = *recovery_انحراف;

	اگر (methods->tdb_expand_file(tdb, tdb->transaction->old_map_سائز,
				     (tdb->map_سائز - tdb->transaction->old_map_سائز) +
				     حجم(rec) + *recovery_max_سائز) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		واپس -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_سائز + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_سائز = tdb->map_سائز;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_سر);
	اگر (methods->tdb_write(tdb, TDB_RECOVERY_سر,
			       &recovery_سر, حجم(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		واپس -1;
	}

	واپس 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ساکن صحیح transaction_تیاری_recovery(ساخت tdb_context *tdb,
				      tdb_off_t *magic_انحراف)
{
	ساخت tdb_transaction_el *el;
	tdb_len_t recovery_سائز;
	غیرعلامتی حرف *ڈیٹا, *p;
	ثابت ساخت tdb_methods *methods = tdb->transaction->io_methods;
	ساخت list_struct *rec;
	tdb_off_t recovery_انحراف, recovery_max_سائز;
	tdb_off_t old_map_سائز = tdb->transaction->old_map_سائز;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	اگر (tdb_recovery_allocate(tdb, &recovery_سائز,
				  &recovery_انحراف, &recovery_max_سائز) == -1) {
		واپس -1;
	}

	ڈیٹا = (غیرعلامتی حرف *)حافظہ_دو(recovery_سائز + حجم(*rec));
	اگر (ڈیٹا == تہی) {
		tdb->ecode = TDB_ERR_OOM;
		واپس -1;
	}

	rec = (ساخت list_struct *)ڈیٹا;
	memset(rec, 0, حجم(*rec));

	rec->magic    = 0;
	rec->ڈیٹا_len = recovery_سائز;
	rec->rec_len  = recovery_max_سائز;
	rec->کلید_len  = old_map_سائز;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = ڈیٹا + حجم(*rec);
	برائے (el=tdb->transaction->elements;el;el=el->اگلا) {
		اگر (el->انحراف >= old_map_سائز) {
			جاری;
		}
		اگر (el->انحراف + el->لمبائی > tdb->transaction->old_map_سائز) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			آزاد(ڈیٹا);
			tdb->ecode = TDB_ERR_CORRUPT;
			واپس -1;
		}
		حافظہ_نقل(p, &el->انحراف, 4);
		حافظہ_نقل(p+4, &el->لمبائی, 4);
		اگر (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		اگر (methods->tdb_read(tdb, el->انحراف, p + 8, el->لمبائی, 0) != 0) {
			آزاد(ڈیٹا);
			tdb->ecode = TDB_ERR_IO;
			واپس -1;
		}
		p += 8 + el->لمبائی;
	}

	/* and the tailer */
	tailer = حجم(*rec) + recovery_max_سائز;
	حافظہ_نقل(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	اگر (methods->tdb_write(tdb, recovery_انحراف, ڈیٹا, حجم(*rec) + recovery_سائز) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		آزاد(ڈیٹا);
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	اگر (transaction_sync(tdb, recovery_انحراف, حجم(*rec) + recovery_سائز) == -1) {
		آزاد(ڈیٹا);
		واپس -1;
	}

	آزاد(ڈیٹا);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_انحراف = recovery_انحراف + offsetof(ساخت list_struct, magic);

	اگر (methods->tdb_write(tdb, *magic_انحراف, &magic, حجم(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	/* ensure the recovery magic marker is on disk */
	اگر (transaction_sync(tdb, *magic_انحراف, حجم(magic)) == -1) {
		واپس -1;
	}

	واپس 0;
}

/*
  commit the current transaction
*/
صحیح tdb_transaction_commit(ساخت tdb_context *tdb)
{
	ثابت ساخت tdb_methods *methods;
	tdb_off_t magic_انحراف = 0;
	u32 zero = 0;

	اگر (tdb->transaction == تہی) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		واپس -1;
	}

	اگر (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		واپس -1;
	}

	اگر (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		واپس 0;
	}

	/* check for a null transaction */
	اگر (tdb->transaction->elements == تہی) {
		tdb_transaction_cancel(tdb);
		واپس 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	اگر (tdb->عدد_locks || tdb->global_تالا.گنتی) {
		tdb->ecode = TDB_ERR_تالا;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		واپس -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	اگر (tdb_brlock_upgrade(tdb, FREELIST_چوٹی, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_تالا;
		tdb_transaction_cancel(tdb);
		واپس -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	اگر (tdb_brlock(tdb, GLOBAL_تالا, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_تالا;
		tdb_transaction_cancel(tdb);
		واپس -1;
	}

	اگر (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		اگر (transaction_تیاری_recovery(tdb, &magic_انحراف) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_تالا, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			واپس -1;
		}
	}

	/* expand the file to the new size if needed */
	اگر (tdb->map_سائز != tdb->transaction->old_map_سائز) {
		اگر (methods->tdb_expand_file(tdb, tdb->transaction->old_map_سائز,
					     tdb->map_سائز -
					     tdb->transaction->old_map_سائز) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_تالا, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			واپس -1;
		}
		tdb->map_سائز = tdb->transaction->old_map_سائز;
		methods->tdb_oob(tdb, tdb->map_سائز + 1, 1);
	}

	/* perform all the writes */
	جب (tdb->transaction->elements) {
		ساخت tdb_transaction_el *el = tdb->transaction->elements;

		اگر (methods->tdb_write(tdb, el->انحراف, el->ڈیٹا, el->لمبائی) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_تالا, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			واپس -1;
		}
		tdb->transaction->elements = el->اگلا;
		آزاد(el->ڈیٹا);
		آزاد(el);
	}

	اگر (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		اگر (transaction_sync(tdb, 0, tdb->map_سائز) == -1) {
			واپس -1;
		}

		/* remove the recovery marker */
		اگر (methods->tdb_write(tdb, magic_انحراف, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			واپس -1;
		}

		/* ensure the recovery marker has been removed on disk */
		اگر (transaction_sync(tdb, magic_انحراف, 4) == -1) {
			واپس -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_تالا, F_UNLCK, F_SETLKW, 0, 1);

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
#اگر_تعریف_شدہ HAVE_UTIME
	utime(tdb->name, تہی);
#اگر_ختم

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	واپس 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
صحیح tdb_transaction_recover(ساخت tdb_context *tdb)
{
	tdb_off_t recovery_سر, recovery_eof;
	غیرعلامتی حرف *ڈیٹا, *p;
	u32 zero = 0;
	ساخت list_struct rec;

	/* find the recovery area */
	اگر (tdb_ofs_read(tdb, TDB_RECOVERY_سر, &recovery_سر) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	اگر (recovery_سر == 0) {
		/* we have never allocated a recovery record */
		واپس 0;
	}

	/* read the recovery record */
	اگر (tdb->methods->tdb_read(tdb, recovery_سر, &rec,
				   حجم(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	اگر (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		واپس 0;
	}

	اگر (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		واپس -1;
	}

	recovery_eof = rec.کلید_len;

	ڈیٹا = (غیرعلامتی حرف *)حافظہ_دو(rec.ڈیٹا_len);
	اگر (ڈیٹا == تہی) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		واپس -1;
	}

	/* read the full recovery data */
	اگر (tdb->methods->tdb_read(tdb, recovery_سر + حجم(rec), ڈیٹا,
				   rec.ڈیٹا_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		آزاد(ڈیٹا);
		واپس -1;
	}

	/* recover the file data */
	p = ڈیٹا;
	جب (p+8 < ڈیٹا + rec.ڈیٹا_len) {
		u32 ofs, len;
		اگر (DOCONV()) {
			tdb_convert(p, 8);
		}
		حافظہ_نقل(&ofs, p, 4);
		حافظہ_نقل(&len, p+4, 4);

		اگر (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			آزاد(ڈیٹا);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			واپس -1;
		}
		p += 8 + len;
	}

	آزاد(ڈیٹا);

	اگر (transaction_sync(tdb, 0, tdb->map_سائز) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	اگر (recovery_eof <= recovery_سر) {
		اگر (tdb_ofs_write(tdb, TDB_RECOVERY_سر, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			واپس -1;
		}
	}

	/* remove the recovery magic */
	اگر (tdb_ofs_write(tdb, recovery_سر + offsetof(ساخت list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	اگر (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}
	tdb->map_سائز = recovery_eof;
	tdb_mmap(tdb);

	اگر (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		واپس -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	واپس 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ساکن صحیح tdb_rec_free_read(ساخت tdb_context *tdb, tdb_off_t off, ساخت list_struct *rec)
{
	اگر (tdb->methods->tdb_read(tdb, off, rec, حجم(*rec),DOCONV()) == -1)
		واپس -1;

	اگر (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		اگر (tdb->methods->tdb_write(tdb, off, rec, حجم(*rec)) == -1)
			واپس -1;
	}

	اگر (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		واپس TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	اگر (tdb->methods->tdb_oob(tdb, rec->اگلا+حجم(*rec), 0) != 0)
		واپس -1;
	واپس 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ساکن صحیح remove_from_freelist(ساخت tdb_context *tdb, tdb_off_t off, tdb_off_t اگلا)
{
	tdb_off_t آخری_ptr, i;

	/* read in the freelist top */
	آخری_ptr = FREELIST_چوٹی;
	جب (tdb_ofs_read(tdb, آخری_ptr, &i) != -1 && i != 0) {
		اگر (i == off) {
			/* We've found it! */
			واپس tdb_ofs_write(tdb, آخری_ptr, &اگلا);
		}
		/* Follow chain (next offset is at start of record) */
		آخری_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	واپس TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ساکن صحیح update_tailer(ساخت tdb_context *tdb, tdb_off_t انحراف,
			 ثابت ساخت list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = حجم(*rec) + rec->rec_len;
	واپس tdb_ofs_write(tdb, انحراف + totalsize - حجم(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
صحیح tdb_free(ساخت tdb_context *tdb, tdb_off_t انحراف, ساخت list_struct *rec)
{
	tdb_off_t دایاں, بایاں;

	/* Allocation and tailer lock */
	اگر (tdb_تالا(tdb, -1, F_WRLCK) != 0)
		واپس -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	اگر (update_tailer(tdb, انحراف, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		جاؤ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	دایاں = انحراف + حجم(*rec) + rec->rec_len;
	اگر (دایاں + حجم(*rec) <= tdb->map_سائز) {
		ساخت list_struct r;

		اگر (tdb->methods->tdb_read(tdb, دایاں, &r, حجم(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", دایاں));
			جاؤ بایاں;
		}

		/* If it's free, expand to include it. */
		اگر (r.magic == TDB_FREE_MAGIC) {
			اگر (remove_from_freelist(tdb, دایاں, r.اگلا) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", دایاں));
				جاؤ بایاں;
			}
			rec->rec_len += حجم(r) + r.rec_len;
		}
	}

بایاں:
	/* Look left */
	بایاں = انحراف - حجم(tdb_off_t);
	اگر (بایاں > TDB_ڈیٹا_آغاز(tdb->سرخی.hash_سائز)) {
		ساخت list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		اگر (tdb_ofs_read(tdb, بایاں, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", بایاں));
			جاؤ update;
		}

		/* it could be uninitialised data */
		اگر (leftsize == 0 || leftsize == TDB_PAD_U32) {
			جاؤ update;
		}

		بایاں = انحراف - leftsize;

		/* Now read in record */
		اگر (tdb->methods->tdb_read(tdb, بایاں, &l, حجم(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", بایاں, leftsize));
			جاؤ update;
		}

		/* If it's free, expand to include it. */
		اگر (l.magic == TDB_FREE_MAGIC) {
			اگر (remove_from_freelist(tdb, بایاں, l.اگلا) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", بایاں));
				جاؤ update;
			} ورنہ {
				انحراف = بایاں;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	اگر (update_tailer(tdb, انحراف, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", انحراف));
		جاؤ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	اگر (tdb_ofs_read(tdb, FREELIST_چوٹی, &rec->اگلا) == -1 ||
	    tdb_rec_write(tdb, انحراف, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_چوٹی, &انحراف) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", انحراف));
		جاؤ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	واپس 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	واپس -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ساکن tdb_off_t tdb_allocate_ofs(ساخت tdb_context *tdb, tdb_len_t لمبائی, tdb_off_t rec_ptr,
				ساخت list_struct *rec, tdb_off_t آخری_ptr)
{
	ساخت list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', حجم(newrec));

	/* found it - now possibly split it up  */
	اگر (rec->rec_len > لمبائی + MIN_REC_سائز) {
		/* Length of left piece */
		لمبائی = TDB_ALIGN(لمبائی, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (حجم(*rec) + لمبائی);
		newrec_ptr = rec_ptr + حجم(*rec) + لمبائی;

		/* And left record is shortened */
		rec->rec_len = لمبائی;
	} ورنہ {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	اگر (tdb_ofs_write(tdb, آخری_ptr, &rec->اگلا) == -1) {
		واپس 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	اگر (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		واپس 0;
	}

	/* Did we create new block? */
	اگر (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		اگر (update_tailer(tdb, rec_ptr, rec) == -1) {
			واپس 0;
		}

		/* Free new record */
		اگر (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			واپس 0;
		}
	}

	/* all done - return the new record offset */
	واپس rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ساخت tdb_context *tdb, tdb_len_t لمبائی, ساخت list_struct *rec)
{
	tdb_off_t rec_ptr, آخری_ptr, newrec_ptr;
	ساخت {
		tdb_off_t rec_ptr, آخری_ptr;
		tdb_len_t rec_len;
	} bestfit;

	اگر (tdb_تالا(tdb, -1, F_WRLCK) == -1)
		واپس 0;

	/* Extra bytes required for tailer */
	لمبائی += حجم(tdb_off_t);

 again:
	آخری_ptr = FREELIST_چوٹی;

	/* read in the freelist top */
	اگر (tdb_ofs_read(tdb, FREELIST_چوٹی, &rec_ptr) == -1)
		جاؤ fail;

	bestfit.rec_ptr = 0;
	bestfit.آخری_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	جب (rec_ptr) {
		اگر (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			جاؤ fail;
		}

		اگر (rec->rec_len >= لمبائی) {
			اگر (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.آخری_ptr = آخری_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				اگر (bestfit.rec_len < 2*لمبائی) {
					توڑو;
				}
			}
		}

		/* move to the next record */
		آخری_ptr = rec_ptr;
		rec_ptr = rec->اگلا;
	}

	اگر (bestfit.rec_ptr != 0) {
		اگر (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			جاؤ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, لمبائی, bestfit.rec_ptr, rec, bestfit.آخری_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		واپس newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	اگر (tdb_expand(tdb, لمبائی + حجم(*rec)) == 0)
		جاؤ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	واپس 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ساکن صحیح نظر_آیا_داخل_کرو(ساخت tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_ڈیٹا کلید, ڈیٹا;

	memset(&ڈیٹا, '\0', حجم(ڈیٹا));
	کلید.dptr = (غیرعلامتی حرف *)&rec_ptr;
	کلید.dsize = حجم(rec_ptr);
	واپس tdb_store(mem_tdb, کلید, ڈیٹا, TDB_داخل_کرو);
}

صحیح tdb_validate_freelist(ساخت tdb_context *tdb, صحیح *pnum_entries)
{
	ساخت tdb_context *mem_tdb = تہی;
	ساخت list_struct rec;
	tdb_off_t rec_ptr, آخری_ptr;
	صحیح ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->سرخی.hash_سائز,
				TDB_اندرونی, O_RDWR, 0600);
	اگر (!mem_tdb) {
		واپس -1;
	}

	اگر (tdb_تالا(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		واپس 0;
	}

	آخری_ptr = FREELIST_چوٹی;

	/* Store the FREELIST_TOP record. */
	اگر (نظر_آیا_داخل_کرو(mem_tdb, آخری_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		جاؤ fail;
	}

	/* read in the freelist top */
	اگر (tdb_ofs_read(tdb, FREELIST_چوٹی, &rec_ptr) == -1) {
		جاؤ fail;
	}

	جب (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		اگر (نظر_آیا_داخل_کرو(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			جاؤ fail;
		}

		اگر (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			جاؤ fail;
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
	واپس ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ساکن صحیح tdb_اگلا_تالا(ساخت tdb_context *tdb, ساخت tdb_traverse_تالا *tlock,
			 ساخت list_struct *rec)
{
	صحیح want_اگلا = (tlock->off != 0);

	/* Lock each chain from the start one. */
	برائے (; tlock->hash < tdb->سرخی.hash_سائز; tlock->hash++) {
		اگر (!tlock->off && tlock->hash != 0) {
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
			اگر (tlock->hash == tdb->سرخی.hash_سائز) {
				جاری;
			}
		}

		اگر (tdb_تالا(tdb, tlock->hash, tlock->تالا_rw) == -1)
			واپس -1;

		/* No previous record?  Start at top of chain. */
		اگر (!tlock->off) {
			اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(tlock->hash),
				     &tlock->off) == -1)
				جاؤ fail;
		} ورنہ {
			/* Otherwise unlock the previous record. */
			اگر (tdb_unlock_ریکارڈ(tdb, tlock->off) != 0)
				جاؤ fail;
		}

		اگر (want_اگلا) {
			/* We have offset of old record: grab next */
			اگر (tdb_rec_read(tdb, tlock->off, rec) == -1)
				جاؤ fail;
			tlock->off = rec->اگلا;
		}

		/* Iterate through chain */
		جب( tlock->off) {
			tdb_off_t current;
			اگر (tdb_rec_read(tdb, tlock->off, rec) == -1)
				جاؤ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			اگر (tlock->off == rec->اگلا) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				جاؤ fail;
			}

			اگر (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				اگر (tdb_تالا_ریکارڈ(tdb, tlock->off) != 0)
					جاؤ fail;
				واپس tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->اگلا;
			اگر (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				جاؤ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->تالا_rw);
		want_اگلا = 0;
	}
	/* We finished iteration without finding anything */
	واپس TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	اگر (tdb_unlock(tdb, tlock->hash, tlock->تالا_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	واپس -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ساکن صحیح tdb_traverse_اندرونی(ساخت tdb_context *tdb,
				 tdb_traverse_func fn, خالی *نجی_ڈیٹا,
				 ساخت tdb_traverse_تالا *tl)
{
	TDB_ڈیٹا کلید, dbuf;
	ساخت list_struct rec;
	صحیح ret, گنتی = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->اگلا = tdb->travlocks.اگلا;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.اگلا = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	جب ((ret = tdb_اگلا_تالا(tdb, tl, &rec)) > 0) {
		گنتی++;
		/* now read the full record */
		کلید.dptr = tdb_alloc_read(tdb, tl->off + حجم(rec),
					  rec.کلید_len + rec.ڈیٹا_len);
		اگر (!کلید.dptr) {
			ret = -1;
			اگر (tdb_unlock(tdb, tl->hash, tl->تالا_rw) != 0)
				جاؤ برآمد;
			اگر (tdb_unlock_ریکارڈ(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			جاؤ برآمد;
		}
		کلید.dsize = rec.کلید_len;
		dbuf.dptr = کلید.dptr + rec.کلید_len;
		dbuf.dsize = rec.ڈیٹا_len;

		/* Drop chain lock, call out */
		اگر (tdb_unlock(tdb, tl->hash, tl->تالا_rw) != 0) {
			ret = -1;
			محفوظ_FREE(کلید.dptr);
			جاؤ برآمد;
		}
		اگر (fn && fn(tdb, کلید, dbuf, نجی_ڈیٹا)) {
			/* They want us to terminate traversal */
			ret = گنتی;
			اگر (tdb_unlock_ریکارڈ(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			محفوظ_FREE(کلید.dptr);
			جاؤ برآمد;
		}
		محفوظ_FREE(کلید.dptr);
	}
برآمد:
	tdb->travlocks.اگلا = tl->اگلا;
	اگر (ret < 0)
		واپس -1;
	ورنہ
		واپس گنتی;
}


/*
  a write style traverse - temporarily marks the db read only
*/
صحیح tdb_traverse_read(ساخت tdb_context *tdb,
		      tdb_traverse_func fn, خالی *نجی_ڈیٹا)
{
	ساخت tdb_traverse_تالا tl = { تہی, 0, 0, F_RDLCK };
	صحیح ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	اگر (tdb_transaction_تالا(tdb, F_RDLCK)) {
		واپس -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_اندرونی(tdb, fn, نجی_ڈیٹا, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	واپس ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
صحیح tdb_traverse(ساخت tdb_context *tdb,
		 tdb_traverse_func fn, خالی *نجی_ڈیٹا)
{
	ساخت tdb_traverse_تالا tl = { تہی, 0, 0, F_WRLCK };
	صحیح ret;

	اگر (tdb->read_only || tdb->traverse_read) {
		واپس tdb_traverse_read(tdb, fn, نجی_ڈیٹا);
	}

	اگر (tdb_transaction_تالا(tdb, F_WRLCK)) {
		واپس -1;
	}

	ret = tdb_traverse_اندرونی(tdb, fn, نجی_ڈیٹا, &tl);

	tdb_transaction_unlock(tdb);

	واپس ret;
}


/* find the first entry in the database and return its key */
TDB_ڈیٹا tdb_firstkey(ساخت tdb_context *tdb)
{
	TDB_ڈیٹا کلید;
	ساخت list_struct rec;

	/* release any old lock */
	اگر (tdb_unlock_ریکارڈ(tdb, tdb->travlocks.off) != 0)
		واپس tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.تالا_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	اگر (tdb_اگلا_تالا(tdb, &tdb->travlocks, &rec) <= 0)
		واپس tdb_null;
	/* now read the key */
	کلید.dsize = rec.کلید_len;
	کلید.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),کلید.dsize);

	/* Unlock the hash chain of the record we just read. */
	اگر (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.تالا_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	واپس کلید;
}

/* find the next entry in the database, returning its key */
TDB_ڈیٹا tdb_nextkey(ساخت tdb_context *tdb, TDB_ڈیٹا oldkey)
{
	u32 oldhash;
	TDB_ڈیٹا کلید = tdb_null;
	ساخت list_struct rec;
	غیرعلامتی حرف *k = تہی;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	اگر (tdb->travlocks.off) {
		اگر (tdb_تالا(tdb,tdb->travlocks.hash,tdb->travlocks.تالا_rw))
			واپس tdb_null;
		اگر (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+حجم(rec),
					    rec.کلید_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			اگر (tdb_unlock_ریکارڈ(tdb, tdb->travlocks.off) != 0) {
				محفوظ_FREE(k);
				واپس tdb_null;
			}
			اگر (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.تالا_rw) != 0) {
				محفوظ_FREE(k);
				واپس tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		محفوظ_FREE(k);
	}

	اگر (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_ڈھونڈو_تالا_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.تالا_rw, &rec);
		اگر (!tdb->travlocks.off)
			واپس tdb_null;
		tdb->travlocks.hash = بالٹی(rec.بھرا_hash);
		اگر (tdb_تالا_ریکارڈ(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			واپس tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	اگر (tdb_اگلا_تالا(tdb, &tdb->travlocks, &rec) > 0) {
		کلید.dsize = rec.کلید_len;
		کلید.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+حجم(rec),
					  کلید.dsize);
		/* Unlock the chain of this new record */
		اگر (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.تالا_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	اگر (tdb_unlock(tdb, بالٹی(oldhash), tdb->travlocks.تالا_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	واپس کلید;
}

/* file: dump.c */

ساکن tdb_off_t tdb_dump_ریکارڈ(ساخت tdb_context *tdb, صحیح hash,
				 tdb_off_t انحراف)
{
	ساخت list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	اگر (tdb->methods->tdb_read(tdb, انحراف, (حرف *)&rec,
				   حجم(rec), DOCONV()) == -1) {
		لکھو("ERROR: failed to read record at %u\n", انحراف);
		واپس 0;
	}

	لکھو(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, انحراف, rec.اگلا, rec.rec_len, rec.کلید_len, rec.ڈیٹا_len,
	       rec.بھرا_hash, rec.magic);

	tailer_ofs = انحراف + حجم(rec) + rec.rec_len - حجم(tdb_off_t);

	اگر (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		لکھو("ERROR: failed to read tailer at %u\n", tailer_ofs);
		واپس rec.اگلا;
	}

	اگر (tailer != rec.rec_len + حجم(rec)) {
		لکھو("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(غیرعلامتی صحیح)tailer, (غیرعلامتی صحیح)(rec.rec_len + حجم(rec)));
	}
	واپس rec.اگلا;
}

ساکن صحیح tdb_dump_chain(ساخت tdb_context *tdb, صحیح i)
{
	tdb_off_t rec_ptr, چوٹی;

	چوٹی = TDB_HASH_چوٹی(i);

	اگر (tdb_تالا(tdb, i, F_WRLCK) != 0)
		واپس -1;

	اگر (tdb_ofs_read(tdb, چوٹی, &rec_ptr) == -1)
		واپس tdb_unlock(tdb, i, F_WRLCK);

	اگر (rec_ptr)
		لکھو("hash=%d\n", i);

	جب (rec_ptr) {
		rec_ptr = tdb_dump_ریکارڈ(tdb, i, rec_ptr);
	}

	واپس tdb_unlock(tdb, i, F_WRLCK);
}

خالی tdb_dump_all(ساخت tdb_context *tdb)
{
	صحیح i;
	برائے (i = 0; i < (صحیح)tdb->سرخی.hash_سائز; i++) {
		tdb_dump_chain(tdb, i);
	}
	لکھو("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

صحیح tdb_printfreelist(ساخت tdb_context *tdb)
{
	صحیح ret;
	طویل کل_free = 0;
	tdb_off_t انحراف, rec_ptr;
	ساخت list_struct rec;

	اگر ((ret = tdb_تالا(tdb, -1, F_WRLCK)) != 0)
		واپس ret;

	انحراف = FREELIST_چوٹی;

	/* read in the freelist top */
	اگر (tdb_ofs_read(tdb, انحراف, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		واپس 0;
	}

	لکھو("freelist top=[0x%08x]\n", rec_ptr );
	جب (rec_ptr) {
		اگر (tdb->methods->tdb_read(tdb, rec_ptr, (حرف *)&rec,
					   حجم(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			واپس -1;
		}

		اگر (rec.magic != TDB_FREE_MAGIC) {
			لکھو("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			واپس -1;
		}

		لکھو("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		کل_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.اگلا;
	}
	لکھو("total rec_len = [0x%08x (%d)]\n", (صحیح)کل_free,
               (صحیح)کل_free);

	واپس tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
خالی tdb_increment_seqnum_nonblock(ساخت tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	اگر (!(tdb->flags & TDB_SEQNUM)) {
		واپس;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	اگر (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		واپس;
	seqnum++;
	(خالی) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ساکن خالی tdb_increment_seqnum(ساخت tdb_context *tdb)
{
	اگر (!(tdb->flags & TDB_SEQNUM)) {
		واپس;
	}

	اگر (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		واپس;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ساکن صحیح tdb_کلید_compare(TDB_ڈیٹا کلید, TDB_ڈیٹا ڈیٹا,
			   خالی *نجی_ڈیٹا EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس memcmp(ڈیٹا.dptr, کلید.dptr, ڈیٹا.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ساکن tdb_off_t tdb_ڈھونڈو(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, u32 hash,
			ساخت list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		واپس 0;

	/* keep looking until we find the right record */
	جب (rec_ptr) {
		اگر (tdb_rec_read(tdb, rec_ptr, r) == -1)
			واپس 0;

		اگر (!TDB_DEAD(r) && hash==r->بھرا_hash
		    && کلید.dsize==r->کلید_len
		    && tdb_parse_ڈیٹا(tdb, کلید, rec_ptr + حجم(*r),
				      r->کلید_len, tdb_کلید_compare,
				      تہی) == 0) {
			واپس rec_ptr;
		}
		rec_ptr = r->اگلا;
	}
	واپس TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_ڈھونڈو_تالا_hash(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, u32 hash, صحیح locktype,
			   ساخت list_struct *rec)
{
	u32 rec_ptr;

	اگر (tdb_تالا(tdb, بالٹی(hash), locktype) == -1)
		واپس 0;
	اگر (!(rec_ptr = tdb_ڈھونڈو(tdb, کلید, hash, rec)))
		tdb_unlock(tdb, بالٹی(hash), locktype);
	واپس rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ساکن صحیح tdb_update_hash(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, u32 hash, TDB_ڈیٹا dbuf)
{
	ساخت list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	اگر (!(rec_ptr = tdb_ڈھونڈو(tdb, کلید, hash, &rec)))
		واپس -1;

	/* must be long enough key, data and tailer */
	اگر (rec.rec_len < کلید.dsize + dbuf.dsize + حجم(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		واپس -1;
	}

	اگر (tdb->methods->tdb_write(tdb, rec_ptr + حجم(rec) + rec.کلید_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		واپس -1;

	اگر (dbuf.dsize != rec.ڈیٹا_len) {
		/* update size */
		rec.ڈیٹا_len = dbuf.dsize;
		واپس tdb_rec_write(tdb, rec_ptr, &rec);
	}

	واپس 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_ڈیٹا tdb_fetch(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	tdb_off_t rec_ptr;
	ساخت list_struct rec;
	TDB_ڈیٹا ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	اگر (!(rec_ptr = tdb_ڈھونڈو_تالا_hash(tdb,کلید,hash,F_RDLCK,&rec)))
		واپس tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + حجم(rec) + rec.کلید_len,
				  rec.ڈیٹا_len);
	ret.dsize = rec.ڈیٹا_len;
	tdb_unlock(tdb, بالٹی(rec.بھرا_hash), F_RDLCK);
	واپس ret;
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

صحیح tdb_parse_ریکارڈ(ساخت tdb_context *tdb, TDB_ڈیٹا کلید,
		     صحیح (*parser)(TDB_ڈیٹا کلید, TDB_ڈیٹا ڈیٹا,
				   خالی *نجی_ڈیٹا),
		     خالی *نجی_ڈیٹا)
{
	tdb_off_t rec_ptr;
	ساخت list_struct rec;
	صحیح ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);

	اگر (!(rec_ptr = tdb_ڈھونڈو_تالا_hash(tdb,کلید,hash,F_RDLCK,&rec))) {
		واپس TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_ڈیٹا(tdb, کلید, rec_ptr + حجم(rec) + rec.کلید_len,
			     rec.ڈیٹا_len, parser, نجی_ڈیٹا);

	tdb_unlock(tdb, بالٹی(rec.بھرا_hash), F_RDLCK);

	واپس ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ساکن صحیح tdb_exists_hash(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, u32 hash)
{
	ساخت list_struct rec;

	اگر (tdb_ڈھونڈو_تالا_hash(tdb, کلید, hash, F_RDLCK, &rec) == 0)
		واپس 0;
	tdb_unlock(tdb, بالٹی(rec.بھرا_hash), F_RDLCK);
	واپس 1;
}

صحیح tdb_exists(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	واپس tdb_exists_hash(tdb, کلید, hash);
}

/* actually delete an entry in the database given the offset */
صحیح tdb_do_delete(ساخت tdb_context *tdb, tdb_off_t rec_ptr, ساخت list_struct*rec)
{
	tdb_off_t آخری_ptr, i;
	ساخت list_struct lastrec;

	اگر (tdb->read_only || tdb->traverse_read) واپس -1;

	اگر (tdb_write_تالا_ریکارڈ(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		واپس tdb_rec_write(tdb, rec_ptr, rec);
	}
	اگر (tdb_write_unlock_ریکارڈ(tdb, rec_ptr) != 0)
		واپس -1;

	/* find previous record in hash chain */
	اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(rec->بھرا_hash), &i) == -1)
		واپس -1;
	برائے (آخری_ptr = 0; i != rec_ptr; آخری_ptr = i, i = lastrec.اگلا)
		اگر (tdb_rec_read(tdb, i, &lastrec) == -1)
			واپس -1;

	/* unlink it: next ptr is at start of record. */
	اگر (آخری_ptr == 0)
		آخری_ptr = TDB_HASH_چوٹی(rec->بھرا_hash);
	اگر (tdb_ofs_write(tdb, آخری_ptr, &rec->اگلا) == -1)
		واپس -1;

	/* recover the space */
	اگر (tdb_free(tdb, rec_ptr, rec) == -1)
		واپس -1;
	واپس 0;
}

ساکن صحیح tdb_گنتی_dead(ساخت tdb_context *tdb, u32 hash)
{
	صحیح res = 0;
	tdb_off_t rec_ptr;
	ساخت list_struct rec;

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		واپس 0;

	جب (rec_ptr) {
		اگر (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			واپس 0;

		اگر (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.اگلا;
	}
	واپس res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ساکن صحیح tdb_purge_dead(ساخت tdb_context *tdb, u32 hash)
{
	صحیح res = -1;
	ساخت list_struct rec;
	tdb_off_t rec_ptr;

	اگر (tdb_تالا(tdb, -1, F_WRLCK) == -1) {
		واپس -1;
	}

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		جاؤ fail;

	جب (rec_ptr) {
		tdb_off_t اگلا;

		اگر (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			جاؤ fail;
		}

		اگلا = rec.اگلا;

		اگر (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			جاؤ fail;
		}
		rec_ptr = اگلا;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	واپس res;
}

/* delete an entry in the database given a key */
ساکن صحیح tdb_delete_hash(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, u32 hash)
{
	tdb_off_t rec_ptr;
	ساخت list_struct rec;
	صحیح ret;

	اگر (tdb->max_dead_اندراجات != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		اگر (tdb_تالا(tdb, بالٹی(hash), F_WRLCK) == -1)
			واپس -1;

		اگر (tdb_گنتی_dead(tdb, hash) >= tdb->max_dead_اندراجات) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		اگر (!(rec_ptr = tdb_ڈھونڈو(tdb, کلید, hash, &rec))) {
			tdb_unlock(tdb, بالٹی(hash), F_WRLCK);
			واپس -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	ورنہ {
		اگر (!(rec_ptr = tdb_ڈھونڈو_تالا_hash(tdb, کلید, hash, F_WRLCK,
						   &rec)))
			واپس -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	اگر (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	اگر (tdb_unlock(tdb, بالٹی(rec.بھرا_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	واپس ret;
}

صحیح tdb_delete(ساخت tdb_context *tdb, TDB_ڈیٹا کلید)
{
	u32 hash = tdb->hash_fn(&کلید);
	واپس tdb_delete_hash(tdb, کلید, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ساکن tdb_off_t tdb_ڈھونڈو_dead(ساخت tdb_context *tdb, u32 hash,
			       ساخت list_struct *r, tdb_len_t لمبائی)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1)
		واپس 0;

	/* keep looking until we find the right record */
	جب (rec_ptr) {
		اگر (tdb_rec_read(tdb, rec_ptr, r) == -1)
			واپس 0;

		اگر (TDB_DEAD(r) && r->rec_len >= لمبائی) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			واپس rec_ptr;
		}
		rec_ptr = r->اگلا;
	}
	واپس 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
صحیح tdb_store(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, صحیح flag)
{
	ساخت list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	حرف *p = تہی;
	صحیح ret = -1;

	اگر (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		واپس -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	اگر (tdb_تالا(tdb, بالٹی(hash), F_WRLCK) == -1)
		واپس -1;

	/* check for it existing, on insert. */
	اگر (flag == TDB_داخل_کرو) {
		اگر (tdb_exists_hash(tdb, کلید, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			جاؤ fail;
		}
	} ورنہ {
		/* first try in-place update, on modify or replace. */
		اگر (tdb_update_hash(tdb, کلید, hash, dbuf) == 0) {
			جاؤ مکمل;
		}
		اگر (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			جاؤ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	اگر (flag != TDB_داخل_کرو)
		tdb_delete_hash(tdb, کلید, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	اگر (!(p = (حرف *)حافظہ_دو(کلید.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		جاؤ fail;
	}

	حافظہ_نقل(p, کلید.dptr, کلید.dsize);
	اگر (dbuf.dsize)
		حافظہ_نقل(p+کلید.dsize, dbuf.dptr, dbuf.dsize);

	اگر (tdb->max_dead_اندراجات != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_ڈھونڈو_dead(
			tdb, hash, &rec,
			کلید.dsize + dbuf.dsize + حجم(tdb_off_t));

		اگر (rec_ptr != 0) {
			rec.کلید_len = کلید.dsize;
			rec.ڈیٹا_len = dbuf.dsize;
			rec.بھرا_hash = hash;
			rec.magic = TDB_MAGIC;
			اگر (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + حجم(rec),
				    p, کلید.dsize + dbuf.dsize) == -1) {
				جاؤ fail;
			}
			جاؤ مکمل;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	اگر (tdb_تالا(tdb, -1, F_WRLCK) == -1) {
		جاؤ fail;
	}

	اگر ((tdb->max_dead_اندراجات != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		جاؤ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, کلید.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	اگر (rec_ptr == 0) {
		جاؤ fail;
	}

	/* Read hash top into next ptr */
	اگر (tdb_ofs_read(tdb, TDB_HASH_چوٹی(hash), &rec.اگلا) == -1)
		جاؤ fail;

	rec.کلید_len = کلید.dsize;
	rec.ڈیٹا_len = dbuf.dsize;
	rec.بھرا_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	اگر (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+حجم(rec), p, کلید.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_چوٹی(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		جاؤ fail;
	}

 مکمل:
	ret = 0;
 fail:
	اگر (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	محفوظ_FREE(p);
	tdb_unlock(tdb, بالٹی(hash), F_WRLCK);
	واپس ret;
}


/* Append to an entry. Create if not exist. */
صحیح tdb_append(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا new_dbuf)
{
	u32 hash;
	TDB_ڈیٹا dbuf;
	صحیح ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&کلید);
	اگر (tdb_تالا(tdb, بالٹی(hash), F_WRLCK) == -1)
		واپس -1;

	dbuf = tdb_fetch(tdb, کلید);

	اگر (dbuf.dptr == تہی) {
		dbuf.dptr = (غیرعلامتی حرف *)حافظہ_دو(new_dbuf.dsize);
	} ورنہ {
		غیرعلامتی حرف *new_dptr = (غیرعلامتی حرف *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		اگر (new_dptr == تہی) {
			آزاد(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	اگر (dbuf.dptr == تہی) {
		tdb->ecode = TDB_ERR_OOM;
		جاؤ failed;
	}

	حافظہ_نقل(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, کلید, dbuf, 0);

failed:
	tdb_unlock(tdb, بالٹی(hash), F_WRLCK);
	محفوظ_FREE(dbuf.dptr);
	واپس ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ثابت حرف *tdb_name(ساخت tdb_context *tdb)
{
	واپس tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
صحیح tdb_fd(ساخت tdb_context *tdb)
{
	واپس tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ساخت tdb_context *tdb)
{
	واپس tdb->لوگارتھم.log_fn;
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
صحیح tdb_get_seqnum(ساخت tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	اگر (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		واپس 0;
	واپس seqnum;
}

صحیح tdb_hash_سائز(ساخت tdb_context *tdb)
{
	واپس tdb->سرخی.hash_سائز;
}

size_t tdb_map_سائز(ساخت tdb_context *tdb)
{
	واپس tdb->map_سائز;
}

صحیح tdb_get_flags(ساخت tdb_context *tdb)
{
	واپس tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
خالی tdb_enable_seqnum(ساخت tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ساکن ساخت tdb_context *tdbs = تہی;


/* This is from a hash algorithm suggested by Rogier Wolff */
ساکن غیرعلامتی صحیح default_tdb_hash(TDB_ڈیٹا *کلید)
{
	u32 قدر;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	برائے (قدر = 0, i=0; i < کلید->dsize; i++)
		قدر = قدر * 256 + کلید->dptr[i] + (قدر >> 24) * 241;

	واپس قدر;
}


/* initialise a new database with a specified hash size */
ساکن صحیح tdb_new_database(ساخت tdb_context *tdb, صحیح hash_سائز)
{
	ساخت tdb_سرخی *newdb;
	صحیح سائز, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	سائز = حجم(ساخت tdb_سرخی) + (hash_سائز+1)*حجم(tdb_off_t);
	اگر (!(newdb = (ساخت tdb_سرخی *)صفر_حافظہ_دو(سائز, 1)))
		واپس TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->نسخہ = TDB_نسخہ;
	newdb->hash_سائز = hash_سائز;
	اگر (tdb->flags & TDB_اندرونی) {
		tdb->map_سائز = سائز;
		tdb->map_ptr = (حرف *)newdb;
		حافظہ_نقل(&tdb->سرخی, newdb, حجم(tdb->سرخی));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		واپس 0;
	}
	اگر (lseek(tdb->fd, 0, SEEK_SET) == -1)
		جاؤ fail;

	اگر (ftruncate(tdb->fd, 0) == -1)
		جاؤ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	حافظہ_نقل(&tdb->سرخی, newdb, حجم(tdb->سرخی));
	/* Don't endian-convert the magic food! */
	حافظہ_نقل(newdb->magic_food, TDB_MAGIC_FOOD, لڑی_لمبائی(TDB_MAGIC_FOOD)+1);
	اگر (اكتب(tdb->fd, newdb, سائز) != سائز) {
		ret = -1;
	} ورنہ {
		ret = 0;
	}

  fail:
	محفوظ_FREE(newdb);
	واپس ret;
}



ساکن صحیح tdb_already_open(dev_t device,
			    ino_t ino)
{
	ساخت tdb_context *i;

	برائے (i = tdbs; i; i = i->اگلا) {
		اگر (i->device == device && i->inode == ino) {
			واپس 1;
		}
	}

	واپس 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ساخت tdb_context *tdb_open(ثابت حرف *name, صحیح hash_سائز, صحیح tdb_flags,
		      صحیح open_flags, mode_t منوال)
{
	واپس tdb_open_ex(name, hash_سائز, tdb_flags, open_flags, منوال, تہی, تہی);
}

/* a default logging function */
ساکن خالی null_log_fn(ساخت tdb_context *tdb, شمار tdb_debug_level level, ثابت حرف *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ساکن خالی null_log_fn(ساخت tdb_context *tdb EXT2FS_ATTR((غیر_مستعمل)),
			شمار tdb_debug_level level EXT2FS_ATTR((غیر_مستعمل)),
			ثابت حرف *fmt EXT2FS_ATTR((غیر_مستعمل)), ...)
{
}


ساخت tdb_context *tdb_open_ex(ثابت حرف *name, صحیح hash_سائز, صحیح tdb_flags,
				صحیح open_flags, mode_t منوال,
				ثابت ساخت tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ساخت tdb_context *tdb;
	ساخت stat st;
	صحیح rev = 0, locked = 0;
	غیرعلامتی حرف *vp;
	u32 vertest;

	اگر (!(tdb = (ساخت tdb_context *)صفر_حافظہ_دو(1, حجم *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		جاؤ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = تہی;
	tdb->map_ptr = تہی;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	اگر (log_ctx) {
		tdb->لوگارتھم = *log_ctx;
	} ورنہ {
		tdb->لوگارتھم.log_fn = null_log_fn;
		tdb->لوگارتھم.log_نجی = تہی;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->صفحہ_سائز = sysconf(_SC_PAGESIZE);
	اگر (tdb->صفحہ_سائز <= 0) {
		tdb->صفحہ_سائز = 0x2000;
	}

	اگر ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		جاؤ fail;
	}

	اگر (hash_سائز == 0)
		hash_سائز = DEFAULT_HASH_سائز;
	اگر ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_پہلا;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	اگر (tdb->flags & TDB_اندرونی) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_پہلا;
		اگر (tdb_new_database(tdb, hash_سائز) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			جاؤ fail;
		}
		جاؤ اندرونی_2;
	}

	اگر ((tdb->fd = open(name, open_flags, منوال)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		جاؤ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	اگر (tdb->methods->tdb_brlock(tdb, GLOBAL_تالا, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		جاؤ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	اگر ((tdb_flags & TDB_CLEAR_IF_پہلا) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, فعال_تالا, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		اگر (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			جاؤ fail; /* errno set by ftruncate */
		}
	}

	اگر (اقرأ(tdb->fd, &tdb->سرخی, حجم(tdb->سرخی)) != حجم(tdb->سرخی)
	    || memcmp(tdb->سرخی.magic_food, TDB_MAGIC_FOOD,
		      حجم(TDB_MAGIC_FOOD)) != 0
	    || (tdb->سرخی.نسخہ != TDB_نسخہ
		&& !(rev = (tdb->سرخی.نسخہ==TDB_BYTEREV(TDB_نسخہ))))) {
		/* its not a valid database - possibly initialise it */
		اگر (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_سائز) == -1) {
			errno = EIO; /* ie bad format or something */
			جاؤ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (غیرعلامتی حرف *)&tdb->سرخی.نسخہ;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_نسخہ) ? TDB_BIGENDIAN : 0;
	اگر (!rev)
		tdb->flags &= ~TDB_CONVERT;
	ورنہ {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->سرخی, حجم(tdb->سرخی));
	}
	اگر (fstat(tdb->fd, &st) == -1)
		جاؤ fail;

	اگر (tdb->سرخی.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		جاؤ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	اگر (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (صحیح)st.st_dev, (صحیح)st.st_ino));
		errno = EBUSY;
		جاؤ fail;
	}

	اگر (!(tdb->name = (حرف *)strdup(name))) {
		errno = ENOMEM;
		جاؤ fail;
	}

	tdb->map_سائز = st.st_سائز;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_اندراجات = 0;
	tdb_mmap(tdb);
	اگر (locked) {
		اگر (tdb->methods->tdb_brlock(tdb, فعال_تالا, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			جاؤ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	اگر (tdb_flags & TDB_CLEAR_IF_پہلا) {
		/* leave this lock in place to indicate it's in use */
		اگر (tdb->methods->tdb_brlock(tdb, فعال_تالا, F_RDLCK, F_SETLKW, 0, 1) == -1)
			جاؤ fail;
	}

	/* if needed, run recovery */
	اگر (tdb_transaction_recover(tdb) == -1) {
		جاؤ fail;
	}

 اندرونی_2:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	اگر (tdb->methods->tdb_brlock(tdb, GLOBAL_تالا, F_UNLCK, F_SETLKW, 0, 1) == -1)
		جاؤ fail;
	tdb->اگلا = tdbs;
	tdbs = tdb;
	واپس tdb;

 fail:
	{ صحیح save_errno = errno;

	اگر (!tdb)
		واپس تہی;

	اگر (tdb->map_ptr) {
		اگر (tdb->flags & TDB_اندرونی)
			محفوظ_FREE(tdb->map_ptr);
		ورنہ
			tdb_munmap(tdb);
	}
	محفوظ_FREE(tdb->name);
	اگر (tdb->fd != -1)
		اگر (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	محفوظ_FREE(tdb);
	errno = save_errno;
	واپس تہی;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

خالی tdb_سیٹ_max_dead(ساخت tdb_context *tdb, صحیح max_dead)
{
	tdb->max_dead_اندراجات = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
صحیح tdb_close(ساخت tdb_context *tdb)
{
	ساخت tdb_context **i;
	صحیح ret = 0;

	اگر (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	اگر (tdb->map_ptr) {
		اگر (tdb->flags & TDB_اندرونی)
			محفوظ_FREE(tdb->map_ptr);
		ورنہ
			tdb_munmap(tdb);
	}
	محفوظ_FREE(tdb->name);
	اگر (tdb->fd != -1)
		ret = close(tdb->fd);
	محفوظ_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	برائے (i = &tdbs; *i; i = &(*i)->اگلا) {
		اگر (*i == tdb) {
			*i = tdb->اگلا;
			توڑو;
		}
	}

	memset(tdb, 0, حجم(*tdb));
	محفوظ_FREE(tdb);

	واپس ret;
}

/* register a logging function */
خالی tdb_سیٹ_logging_function(ساخت tdb_context *tdb,
                              ثابت ساخت tdb_logging_context *log_ctx)
{
        tdb->لوگارتھم = *log_ctx;
}

خالی *tdb_get_logging_نجی(ساخت tdb_context *tdb)
{
	واپس tdb->لوگارتھم.log_نجی;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
صحیح tdb_reopen(ساخت tdb_context *tdb)
{
	ساخت stat st;

	اگر (tdb->flags & TDB_اندرونی) {
		واپس 0; /* Nothing to do. */
	}

	اگر (tdb->عدد_locks != 0 || tdb->global_تالا.گنتی) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		جاؤ fail;
	}

	اگر (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		جاؤ fail;
	}

	اگر (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		جاؤ fail;
	}
	اگر (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	اگر (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		جاؤ fail;
	}
	اگر ((tdb->flags & TDB_CLEAR_IF_پہلا) &&
	    (tdb->methods->tdb_brlock(tdb, فعال_تالا, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		جاؤ fail;
	}
	اگر (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		جاؤ fail;
	}
	اگر (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		جاؤ fail;
	}
	tdb_mmap(tdb);

	واپس 0;

fail:
	tdb_close(tdb);
	واپس -1;
}

/* reopen all tdb's */
صحیح tdb_reopen_all(صحیح والد_longlived)
{
	ساخت tdb_context *tdb;

	برائے (tdb=tdbs; tdb; tdb = tdb->اگلا) {
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
		اگر (والد_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_پہلا;
		}

		اگر (tdb_reopen(tdb) != 0)
			واپس -1;
	}

	واپس 0;
}

/**
 * Flush a database file from the page cache.
 **/
صحیح tdb_flush(ساخت tdb_context *tdb)
{
	اگر (tdb->fd != -1)
		واپس fsync(tdb->fd);
	واپس 0;
}
