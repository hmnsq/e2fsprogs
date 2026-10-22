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

#अगर_घोषित CONFIG_STAND_ALONE
#घोषणा HAVE_MMAP
#घोषणा HAVE_STRDUP
#घोषणा HAVE_SYS_MMAN_H
#घोषणा HAVE_UTIME_H
#घोषणा HAVE_UTIME
#पूर्ण_अगर
#अगर_अघोषित __FreeBSD__
#घोषणा _XOPEN_SOURCE 600
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <unistd.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <stdarg.h>
#समावेश <stddef.h>
#समावेश <errno.h>
#समावेश <string.h>
#अगर_घोषित HAVE_SYS_SELECT_H
#समावेश <sys/select.h>
#पूर्ण_अगर
#समावेश <sys/time.h>
#समावेश <sys/types.h>
#समावेश <time.h>
#अगर_घोषित HAVE_UTIME_H
#समावेश <utime.h>
#पूर्ण_अगर
#समावेश <sys/stat.h>
#समावेश <sys/file.h>
#समावेश <fcntl.h>

#अगर_घोषित HAVE_SYS_MMAN_H
#समावेश <sys/mman.h>
#पूर्ण_अगर

#अगर_घोषित __GNUC__
#घोषणा EXT2FS_ATTR(x) __attribute__(x)
#अन्यथा
#घोषणा EXT2FS_ATTR(x)
#पूर्ण_अगर

#अगर_अघोषित MAP_FILE
#घोषणा MAP_FILE 0
#पूर्ण_अगर

#अगर_अघोषित MAP_FAILED
#घोषणा MAP_FAILED ((व्योम *)-1)
#पूर्ण_अगर

#अगर_अघोषित HAVE_STRDUP
#घोषणा strdup rep_strdup
अटल अक्षर *rep_strdup(स्थिर अक्षर *s)
{
	अक्षर *ret;
	पूर्णांक लंबाई;

	अगर (!s)
		वापस शून्य;
	लंबाई = म_माप(s);
	ret = दो_स्मृति(लंबाई + 1);
	अगर (ret) {
		म_नकलन(ret, s, लंबाई);
		ret[लंबाई] = '\0';
	}
	वापस ret;
}
#पूर्ण_अगर

#अगर_अघोषित PRINTF_ATTRIBUTE
#अगर (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#घोषणा PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#अन्यथा
#घोषणा PRINTF_ATTRIBUTE(a1, a2)
#पूर्ण_अगर
#पूर्ण_अगर

#अगर defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
प्रकार पूर्णांक bool;
#पूर्ण_अगर

#समावेश "tdb.h"

अटल TDB_तथ्य tdb_null;

#अगर_अघोषित u32
#घोषणा u32 अचिन्हित
#पूर्ण_अगर

प्रकार u32 tdb_len_t;
प्रकार u32 tdb_off_t;

#अगर_अघोषित दुरत्व
#घोषणा दुरत्व(t,f) ((अचिन्हित पूर्णांक)&((t *)0)->f)
#पूर्ण_अगर

#घोषणा TDB_MAGIC_FOOD "TDB file\n"
#घोषणा TDB_VERSION (0x26011967 + 6)
#घोषणा TDB_MAGIC (0x26011999U)
#घोषणा TDB_FREE_MAGIC (~TDB_MAGIC)
#घोषणा TDB_DEAD_MAGIC (0xFEE1DEAD)
#घोषणा TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#घोषणा TDB_ALIGNMENT 4
#घोषणा MIN_REC_आकार (2*माप(काष्ठा list_struct) + TDB_ALIGNMENT)
#घोषणा DEFAULT_HASH_आकार 131
#घोषणा FREELIST_शीर्ष (माप(काष्ठा tdb_header))
#घोषणा TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#घोषणा TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#घोषणा TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#घोषणा TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#घोषणा TDB_HASH_शीर्ष(hash) (FREELIST_शीर्ष + (BUCKET(hash)+1)*माप(tdb_off_t))
#घोषणा TDB_HASHTABLE_आकार(tdb) ((tdb->header.hash_आकार+1)*माप(tdb_off_t))
#घोषणा TDB_तथ्य_START(hash_आकार) TDB_HASH_शीर्ष(hash_आकार-1)
#घोषणा TDB_RECOVERY_शीर्ष दुरत्व(काष्ठा tdb_header, recovery_start)
#घोषणा TDB_SEQNUM_OFS    दुरत्व(काष्ठा tdb_header, sequence_संख्या)
#घोषणा TDB_PAD_BYTE 0x42
#घोषणा TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#घोषणा TDB_LOG(x) tdb->log.log_fn x

/* lock offsets */
#घोषणा GLOBAL_LOCK      0
#घोषणा ACTIVE_LOCK      4
#घोषणा TRANSACTION_LOCK 8

/* free memory if the pointer is valid and zero the pointer */
#अगर_अघोषित SAFE_FREE
#घोषणा SAFE_FREE(x) करो { अगर ((x) != शून्य) {मुक्त(x); (x)=शून्य;} } जबतक(0)
#पूर्ण_अगर

#घोषणा BUCKET(hash) ((hash) % tdb->header.hash_आकार)

#घोषणा DOCONV() (tdb->flags & TDB_CONVERT)
#घोषणा CONVERT(x) (DOCONV() ? tdb_convert(&x, माप(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
काष्ठा list_struct {
	tdb_off_t अग्र; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t कुंजी_len; /* byte length of key */
	tdb_len_t तथ्य_len; /* byte length of data */
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
काष्ठा tdb_header {
	अक्षर magic_food[32]; /* for /etc/magic */
	u32 version; /* version of the code */
	u32 hash_आकार; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_संख्या; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

काष्ठा tdb_lock_type {
	पूर्णांक list;
	u32 गिनती;
	u32 ltype;
};

काष्ठा tdb_traverse_lock {
	काष्ठा tdb_traverse_lock *अग्र;
	u32 off;
	u32 hash;
	पूर्णांक lock_rw;
};


काष्ठा tdb_methods {
	पूर्णांक (*tdb_read)(काष्ठा tdb_context *, tdb_off_t , व्योम *, tdb_len_t , पूर्णांक );
	पूर्णांक (*tdb_write)(काष्ठा tdb_context *, tdb_off_t, स्थिर व्योम *, tdb_len_t);
	व्योम (*अग्र_hash_chain)(काष्ठा tdb_context *, u32 *);
	पूर्णांक (*tdb_oob)(काष्ठा tdb_context *, tdb_off_t , पूर्णांक );
	पूर्णांक (*tdb_expand_file)(काष्ठा tdb_context *, tdb_off_t , tdb_off_t );
	पूर्णांक (*tdb_brlock)(काष्ठा tdb_context *, tdb_off_t , पूर्णांक, पूर्णांक, पूर्णांक, माप_प्रकार);
};

काष्ठा tdb_context {
	अक्षर *name; /* the name of the database */
	व्योम *map_ptr; /* where it is currently mapped */
	पूर्णांक fd; /* open file descriptor for the database */
	tdb_len_t map_आकार; /* how much space has been mapped */
	पूर्णांक read_only; /* opened read-only */
	पूर्णांक traverse_read; /* read-only traversal */
	काष्ठा tdb_lock_type global_lock;
	पूर्णांक अंक_lockrecs;
	काष्ठा tdb_lock_type *lockrecs; /* only real locks, all with count>0 */
	क्रमागत TDB_ERROR ecode; /* error code for last tdb error */
	काष्ठा tdb_header header; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	काष्ठा tdb_traverse_lock travlocks; /* current traversal locks */
	काष्ठा tdb_context *अग्र; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	काष्ठा tdb_logging_context log;
	अचिन्हित पूर्णांक (*hash_fn)(TDB_तथ्य *कुंजी);
	पूर्णांक open_flags; /* flags used in the open - needed by reopen */
	अचिन्हित पूर्णांक अंक_locks; /* number of chain locks held */
	स्थिर काष्ठा tdb_methods *methods;
	काष्ठा tdb_transaction *transaction;
	पूर्णांक page_आकार;
	पूर्णांक max_dead_records;
	bool have_transaction_lock;
	tdb_len_t real_map_आकार; /* how much space has been mapped */
};


/*
  internal prototypes
*/
अटल पूर्णांक tdb_munmap(काष्ठा tdb_context *tdb);
अटल व्योम tdb_mmap(काष्ठा tdb_context *tdb);
अटल पूर्णांक tdb_lock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype);
पूर्णांक tdb_lock_nonblock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype);
अटल पूर्णांक tdb_unlock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype);
अटल पूर्णांक tdb_brlock(काष्ठा tdb_context *tdb, tdb_off_t offset, पूर्णांक rw_type, पूर्णांक lck_type, पूर्णांक probe, माप_प्रकार len);
अटल पूर्णांक tdb_transaction_lock(काष्ठा tdb_context *tdb, पूर्णांक ltype);
अटल पूर्णांक tdb_transaction_unlock(काष्ठा tdb_context *tdb);
अटल पूर्णांक tdb_brlock_upgrade(काष्ठा tdb_context *tdb, tdb_off_t offset, माप_प्रकार len);
अटल पूर्णांक tdb_write_lock_record(काष्ठा tdb_context *tdb, tdb_off_t off);
अटल पूर्णांक tdb_write_unlock_record(काष्ठा tdb_context *tdb, tdb_off_t off);
अटल पूर्णांक tdb_ofs_read(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
अटल पूर्णांक tdb_ofs_write(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
अटल व्योम *tdb_convert(व्योम *buf, u32 आकार);
अटल पूर्णांक tdb_free(काष्ठा tdb_context *tdb, tdb_off_t offset, काष्ठा list_struct *rec);
अटल tdb_off_t tdb_allocate(काष्ठा tdb_context *tdb, tdb_len_t लंबाई, काष्ठा list_struct *rec);
अटल पूर्णांक tdb_ofs_read(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
अटल पूर्णांक tdb_ofs_write(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_off_t *d);
अटल पूर्णांक tdb_lock_record(काष्ठा tdb_context *tdb, tdb_off_t off);
अटल पूर्णांक tdb_unlock_record(काष्ठा tdb_context *tdb, tdb_off_t off);
अटल पूर्णांक tdb_rec_read(काष्ठा tdb_context *tdb, tdb_off_t offset, काष्ठा list_struct *rec);
अटल पूर्णांक tdb_rec_write(काष्ठा tdb_context *tdb, tdb_off_t offset, काष्ठा list_struct *rec);
अटल पूर्णांक tdb_do_delete(काष्ठा tdb_context *tdb, tdb_off_t rec_ptr, काष्ठा list_struct *rec);
अटल अचिन्हित अक्षर *tdb_alloc_read(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_len_t len);
अटल पूर्णांक tdb_parse_तथ्य(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी,
		   tdb_off_t offset, tdb_len_t len,
		   पूर्णांक (*parser)(TDB_तथ्य कुंजी, TDB_तथ्य तथ्य,
				 व्योम *private_तथ्य),
		   व्योम *private_तथ्य);
अटल tdb_off_t tdb_find_lock_hash(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, u32 hash, पूर्णांक locktype,
			   काष्ठा list_struct *rec);
अटल व्योम tdb_io_init(काष्ठा tdb_context *tdb);
अटल पूर्णांक tdb_expand(काष्ठा tdb_context *tdb, tdb_off_t आकार);
अटल पूर्णांक tdb_rec_free_read(काष्ठा tdb_context *tdb, tdb_off_t off,
		      काष्ठा list_struct *rec);


/* file: error.c */

क्रमागत TDB_ERROR tdb_error(काष्ठा tdb_context *tdb)
{
	वापस tdb->ecode;
}

अटल काष्ठा tdb_errname {
	क्रमागत TDB_ERROR ecode; स्थिर अक्षर *estring;
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
स्थिर अक्षर *tdb_errorstr(काष्ठा tdb_context *tdb)
{
	u32 i;
	क्रम (i = 0; i < माप(emap) / माप(काष्ठा tdb_errname); i++)
		अगर (tdb->ecode == emap[i].ecode)
			वापस emap[i].estring;
	वापस "Invalid error code";
}

/* file: lock.c */

#घोषणा TDB_MARK_LOCK 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
पूर्णांक tdb_brlock(काष्ठा tdb_context *tdb, tdb_off_t offset,
	       पूर्णांक rw_type, पूर्णांक lck_type, पूर्णांक probe, माप_प्रकार len)
{
	काष्ठा flock fl;
	पूर्णांक ret;

	अगर (tdb->flags & TDB_NOLOCK) {
		वापस 0;
	}

	अगर ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		वापस -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = शुरू_से;
	fl.l_start = offset;
	fl.l_len = len;
	fl.l_pid = 0;

	करो {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} जबतक (ret == -1 && त्रुटि_सं == EINTR);

	अगर (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		अगर (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_LOCK;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, offset, rw_type, lck_type, (पूर्णांक)len));
		}
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}
	वापस 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
पूर्णांक tdb_brlock_upgrade(काष्ठा tdb_context *tdb, tdb_off_t offset, माप_प्रकार len)
{
	पूर्णांक गिनती = 1000;
	जबतक (गिनती--) {
		काष्ठा timeval tv;
		अगर (tdb_brlock(tdb, offset, F_WRLCK, F_SETLKW, 1, len) == 0) {
			वापस 0;
		}
		अगर (त्रुटि_सं != EDEADLK) {
			अवरोध;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, शून्य, शून्य, शून्य, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", offset));
	वापस -1;
}


/* lock a list in the database. list -1 is the alloc list */
अटल पूर्णांक _tdb_lock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype, पूर्णांक op)
{
	काष्ठा tdb_lock_type *new_lck;
	पूर्णांक i;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	अगर (tdb->global_lock.गिनती &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		वापस 0;
	}

	अगर (tdb->global_lock.गिनती) {
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	अगर (list < -1 || list >= (पूर्णांक)tdb->header.hash_आकार) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		वापस -1;
	}
	अगर (tdb->flags & TDB_NOLOCK)
		वापस 0;

	क्रम (i=0; i<tdb->अंक_lockrecs; i++) {
		अगर (tdb->lockrecs[i].list == list) {
			अगर (tdb->lockrecs[i].गिनती == 0) {
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
			tdb->lockrecs[i].गिनती++;
			वापस 0;
		}
	}

	new_lck = (काष्ठा tdb_lock_type *)पुनः_स्मृति(
		tdb->lockrecs,
		माप(*tdb->lockrecs) * (tdb->अंक_lockrecs+1));
	अगर (new_lck == शून्य) {
		त्रुटि_सं = ENOMEM;
		वापस -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	अगर (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_शीर्ष+4*list, ltype, op,
				     0, 1)) {
		वापस -1;
	}

	tdb->अंक_locks++;

	tdb->lockrecs[tdb->अंक_lockrecs].list = list;
	tdb->lockrecs[tdb->अंक_lockrecs].गिनती = 1;
	tdb->lockrecs[tdb->अंक_lockrecs].ltype = ltype;
	tdb->अंक_lockrecs += 1;

	वापस 0;
}

/* lock a list in the database. list -1 is the alloc list */
पूर्णांक tdb_lock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype)
{
	पूर्णांक ret;
	ret = _tdb_lock(tdb, list, ltype, F_SETLKW);
	अगर (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, म_त्रुटि(त्रुटि_सं)));
	}
	वापस ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
पूर्णांक tdb_lock_nonblock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype)
{
	वापस _tdb_lock(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
पूर्णांक tdb_unlock(काष्ठा tdb_context *tdb, पूर्णांक list, पूर्णांक ltype)
{
	पूर्णांक ret = -1;
	पूर्णांक i;
	काष्ठा tdb_lock_type *lck = शून्य;
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* a global lock allows us to avoid per chain locks */
	अगर (tdb->global_lock.गिनती &&
	    ((u32)ltype == tdb->global_lock.ltype || ltype == F_RDLCK)) {
		वापस 0;
	}

	अगर (tdb->global_lock.गिनती) {
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	अगर (tdb->flags & TDB_NOLOCK)
		वापस 0;

	/* Sanity checks */
	अगर (list < -1 || list >= (पूर्णांक)tdb->header.hash_आकार) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->header.hash_आकार));
		वापस ret;
	}

	क्रम (i=0; i<tdb->अंक_lockrecs; i++) {
		अगर (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			अवरोध;
		}
	}

	अगर ((lck == शून्य) || (lck->गिनती == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		वापस -1;
	}

	अगर (lck->गिनती > 1) {
		lck->गिनती--;
		वापस 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	अगर (mark_lock) {
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

	अगर (tdb->अंक_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->अंक_lockrecs-1];
	}
	tdb->अंक_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	अगर (tdb->अंक_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	अगर (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	वापस ret;
}

/*
  get the transaction lock
 */
पूर्णांक tdb_transaction_lock(काष्ठा tdb_context *tdb, पूर्णांक ltype)
{
	अगर (tdb->have_transaction_lock || tdb->global_lock.गिनती) {
		वापस 0;
	}
	अगर (tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		वापस -1;
	}
	tdb->have_transaction_lock = 1;
	वापस 0;
}

/*
  release the transaction lock
 */
पूर्णांक tdb_transaction_unlock(काष्ठा tdb_context *tdb)
{
	पूर्णांक ret;
	अगर (!tdb->have_transaction_lock) {
		वापस 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_LOCK, F_UNLCK, F_SETLKW, 0, 1);
	अगर (ret == 0) {
		tdb->have_transaction_lock = 0;
	}
	वापस ret;
}




/* lock/unlock entire database */
अटल पूर्णांक _tdb_lockall(काष्ठा tdb_context *tdb, पूर्णांक ltype, पूर्णांक op)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	अगर (tdb->read_only || tdb->traverse_read)
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);

	अगर (tdb->global_lock.गिनती && tdb->global_lock.ltype == (u32)ltype) {
		tdb->global_lock.गिनती++;
		वापस 0;
	}

	अगर (tdb->global_lock.गिनती) {
		/* a global lock of a different type exists */
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	अगर (tdb->अंक_locks != 0) {
		/* can't combine global and chain locks */
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	अगर (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शीर्ष, ltype, op,
				     0, 4*tdb->header.hash_आकार)) {
		अगर (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		}
		वापस -1;
	}

	tdb->global_lock.गिनती = 1;
	tdb->global_lock.ltype = ltype;

	वापस 0;
}



/* unlock entire db */
अटल पूर्णांक _tdb_unlockall(काष्ठा tdb_context *tdb, पूर्णांक ltype)
{
	bool mark_lock = ((ltype & TDB_MARK_LOCK) == TDB_MARK_LOCK);

	ltype &= ~TDB_MARK_LOCK;

	/* There are no locks on read-only dbs */
	अगर (tdb->read_only || tdb->traverse_read) {
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	अगर (tdb->global_lock.ltype != (u32)ltype ||
	    tdb->global_lock.गिनती == 0) {
		वापस TDB_ERRCODE(TDB_ERR_LOCK, -1);
	}

	अगर (tdb->global_lock.गिनती > 1) {
		tdb->global_lock.गिनती--;
		वापस 0;
	}

	अगर (!mark_lock &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW,
				     0, 4*tdb->header.hash_आकार)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		वापस -1;
	}

	tdb->global_lock.गिनती = 0;
	tdb->global_lock.ltype = 0;

	वापस 0;
}

/* lock entire database with write lock */
पूर्णांक tdb_lockall(काष्ठा tdb_context *tdb)
{
	वापस _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
पूर्णांक tdb_lockall_mark(काष्ठा tdb_context *tdb)
{
	वापस _tdb_lockall(tdb, F_WRLCK | TDB_MARK_LOCK, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
पूर्णांक tdb_lockall_unmark(काष्ठा tdb_context *tdb)
{
	वापस _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_LOCK);
}

/* lock entire database with write lock - nonblocking variant */
पूर्णांक tdb_lockall_nonblock(काष्ठा tdb_context *tdb)
{
	वापस _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
पूर्णांक tdb_unlockall(काष्ठा tdb_context *tdb)
{
	वापस _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
पूर्णांक tdb_lockall_read(काष्ठा tdb_context *tdb)
{
	वापस _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
पूर्णांक tdb_lockall_read_nonblock(काष्ठा tdb_context *tdb)
{
	वापस _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
पूर्णांक tdb_unlockall_read(काष्ठा tdb_context *tdb)
{
	वापस _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
पूर्णांक tdb_chainlock(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_lock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
पूर्णांक tdb_chainlock_nonblock(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_lock_nonblock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णांक tdb_chainlock_mark(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_lock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_WRLCK | TDB_MARK_LOCK);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णांक tdb_chainlock_unmark(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_WRLCK | TDB_MARK_LOCK);
}

पूर्णांक tdb_chainunlock(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_WRLCK);
}

पूर्णांक tdb_chainlock_read(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_lock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_RDLCK);
}

पूर्णांक tdb_chainunlock_read(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	वापस tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुंजी)), F_RDLCK);
}



/* record lock stops delete underneath */
पूर्णांक tdb_lock_record(काष्ठा tdb_context *tdb, tdb_off_t off)
{
	वापस off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णांक tdb_write_lock_record(काष्ठा tdb_context *tdb, tdb_off_t off)
{
	काष्ठा tdb_traverse_lock *i;
	क्रम (i = &tdb->travlocks; i; i = i->अग्र)
		अगर (i->off == off)
			वापस -1;
	वापस tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णांक tdb_write_unlock_record(काष्ठा tdb_context *tdb, tdb_off_t off)
{
	वापस tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
पूर्णांक tdb_unlock_record(काष्ठा tdb_context *tdb, tdb_off_t off)
{
	काष्ठा tdb_traverse_lock *i;
	u32 गिनती = 0;

	अगर (off == 0)
		वापस 0;
	क्रम (i = &tdb->travlocks; i; i = i->अग्र)
		अगर (i->off == off)
			गिनती++;
	वापस (गिनती == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
अटल पूर्णांक tdb_oob(काष्ठा tdb_context *tdb, tdb_off_t len, पूर्णांक probe)
{
	काष्ठा stat st;
	अगर (len <= tdb->map_आकार)
		वापस 0;
	अगर (tdb->flags & TDB_INTERNAL) {
		अगर (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (पूर्णांक)len, (पूर्णांक)tdb->map_आकार));
		}
		वापस TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	अगर (ख_स्थिति(tdb->fd, &st) == -1) {
		वापस TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	अगर (st.st_आकार < (off_t)len) {
		अगर (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (पूर्णांक)len, (पूर्णांक)st.st_आकार));
		}
		वापस TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	अगर (tdb_munmap(tdb) == -1)
		वापस TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_आकार = st.st_आकार;
	tdb_mmap(tdb);
	वापस 0;
}

/* write a lump of data at a specified offset */
अटल पूर्णांक tdb_write(काष्ठा tdb_context *tdb, tdb_off_t off,
		     स्थिर व्योम *buf, tdb_len_t len)
{
	अगर (len == 0) {
		वापस 0;
	}

	अगर (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		वापस -1;
	}

	अगर (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		वापस -1;

	अगर (tdb->map_ptr) {
		स_नकल(off + (अक्षर *)tdb->map_ptr, buf, len);
	} अन्यथा अगर (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, म_त्रुटि(त्रुटि_सं)));
		वापस TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	वापस 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
व्योम *tdb_convert(व्योम *buf, u32 आकार)
{
	u32 i, *p = (u32 *)buf;
	क्रम (i = 0; i < आकार / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	वापस buf;
}


/* read a lump of data at a specified offset, maybe convert */
अटल पूर्णांक tdb_read(काष्ठा tdb_context *tdb, tdb_off_t off, व्योम *buf,
		    tdb_len_t len, पूर्णांक cv)
{
	अगर (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		वापस -1;
	}

	अगर (tdb->map_ptr) {
		स_नकल(buf, off + (अक्षर *)tdb->map_ptr, len);
	} अन्यथा {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		अगर (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (पूर्णांक)off, (पूर्णांक)len, (पूर्णांक)ret, म_त्रुटि(त्रुटि_सं),
				 (पूर्णांक)tdb->map_आकार));
			वापस TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	अगर (cv) {
		tdb_convert(buf, len);
	}
	वापस 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
अटल व्योम tdb_अग्र_hash_chain(काष्ठा tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	अगर (tdb->map_ptr) {
		क्रम (;h < tdb->header.hash_आकार;h++) {
			अगर (0 != *(u32 *)(TDB_HASH_शीर्ष(h) + (अचिन्हित अक्षर *)tdb->map_ptr)) {
				अवरोध;
			}
		}
	} अन्यथा {
		u32 off=0;
		क्रम (;h < tdb->header.hash_आकार;h++) {
			अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(h), &off) != 0 || off != 0) {
				अवरोध;
			}
		}
	}
	(*chain) = h;
}


पूर्णांक tdb_munmap(काष्ठा tdb_context *tdb)
{
	अगर (tdb->flags & TDB_INTERNAL)
		वापस 0;

#अगर_घोषित HAVE_MMAP
	अगर (tdb->map_ptr) {
		पूर्णांक ret = munmap(tdb->map_ptr, tdb->real_map_आकार);
		अगर (ret != 0)
			वापस ret;
		tdb->real_map_आकार = 0;
	}
#पूर्ण_अगर
	tdb->map_ptr = शून्य;
	वापस 0;
}

व्योम tdb_mmap(काष्ठा tdb_context *tdb)
{
	अगर (tdb->flags & TDB_INTERNAL)
		वापस;

#अगर_घोषित HAVE_MMAP
	अगर (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(शून्य, tdb->map_आकार,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		अगर (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_आकार = 0;
			tdb->map_ptr = शून्य;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_आकार, म_त्रुटि(त्रुटि_सं)));
		}
		tdb->real_map_आकार = tdb->map_आकार;
	} अन्यथा {
		tdb->map_ptr = शून्य;
	}
#अन्यथा
	tdb->map_ptr = शून्य;
#पूर्ण_अगर
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
अटल पूर्णांक tdb_expand_file(काष्ठा tdb_context *tdb, tdb_off_t आकार, tdb_off_t addition)
{
	अक्षर buf[1024];

	अगर (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		वापस -1;
	}

	अगर (ftruncate(tdb->fd, आकार+addition) == -1) {
		अक्षर b = 0;
		अगर (pwrite(tdb->fd,  &b, 1, (आकार+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 आकार+addition, म_त्रुटि(त्रुटि_सं)));
			वापस -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	स_रखो(buf, TDB_PAD_BYTE, माप(buf));
	जबतक (addition) {
		पूर्णांक n = addition>माप(buf)?माप(buf):addition;
		पूर्णांक ret = pwrite(tdb->fd, buf, n, आकार);
		अगर (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, म_त्रुटि(त्रुटि_सं)));
			वापस -1;
		}
		addition -= n;
		आकार += n;
	}
	वापस 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
पूर्णांक tdb_expand(काष्ठा tdb_context *tdb, tdb_off_t आकार)
{
	काष्ठा list_struct rec;
	tdb_off_t offset;

	अगर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		वापस -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	आकार = TDB_ALIGN(tdb->map_आकार + आकार*10, tdb->page_आकार) - tdb->map_आकार;

	अगर (!(tdb->flags & TDB_INTERNAL))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	अगर (!(tdb->flags & TDB_INTERNAL)) {
		अगर (tdb->methods->tdb_expand_file(tdb, tdb->map_आकार, आकार) != 0)
			जाओ fail;
	}

	tdb->map_आकार += आकार;

	अगर (tdb->flags & TDB_INTERNAL) {
		अक्षर *new_map_ptr = (अक्षर *)पुनः_स्मृति(tdb->map_ptr,
						    tdb->map_आकार);
		अगर (!new_map_ptr) {
			tdb->map_आकार -= आकार;
			जाओ fail;
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
	स_रखो(&rec,'\0',माप(rec));
	rec.rec_len = आकार - माप(rec);

	/* link it into the free list */
	offset = tdb->map_आकार - आकार;
	अगर (tdb_free(tdb, offset, &rec) == -1)
		जाओ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	वापस 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	वापस -1;
}

/* read/write a tdb_off_t */
पूर्णांक tdb_ofs_read(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	वापस tdb->methods->tdb_read(tdb, offset, (अक्षर*)d, माप(*d), DOCONV());
}

पूर्णांक tdb_ofs_write(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_off_t *d)
{
	tdb_off_t off = *d;
	वापस tdb->methods->tdb_write(tdb, offset, CONVERT(off), माप(*d));
}


/* read a lump of data, allocating the space for it */
अचिन्हित अक्षर *tdb_alloc_read(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_len_t len)
{
	अचिन्हित अक्षर *buf;

	/* some systems don't like zero length malloc */
	अगर (len == 0) {
		len = 1;
	}

	अगर (!(buf = (अचिन्हित अक्षर *)दो_स्मृति(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, म_त्रुटि(त्रुटि_सं)));
		वापस TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	अगर (tdb->methods->tdb_read(tdb, offset, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		वापस शून्य;
	}
	वापस buf;
}

/* Give a piece of tdb data to a parser */

पूर्णांक tdb_parse_तथ्य(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी,
		   tdb_off_t offset, tdb_len_t len,
		   पूर्णांक (*parser)(TDB_तथ्य कुंजी, TDB_तथ्य तथ्य,
				 व्योम *private_तथ्य),
		   व्योम *private_तथ्य)
{
	TDB_तथ्य तथ्य;
	पूर्णांक परिणाम;

	तथ्य.dsize = len;

	अगर ((tdb->transaction == शून्य) && (tdb->map_ptr != शून्य)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		अगर (tdb->methods->tdb_oob(tdb, offset+len, 0) != 0) {
			वापस -1;
		}
		तथ्य.dptr = offset + (अचिन्हित अक्षर *)tdb->map_ptr;
		वापस parser(कुंजी, तथ्य, private_तथ्य);
	}

	अगर (!(तथ्य.dptr = tdb_alloc_read(tdb, offset, len))) {
		वापस -1;
	}

	परिणाम = parser(कुंजी, तथ्य, private_तथ्य);
	मुक्त(तथ्य.dptr);
	वापस परिणाम;
}

/* read/write a record */
पूर्णांक tdb_rec_read(काष्ठा tdb_context *tdb, tdb_off_t offset, काष्ठा list_struct *rec)
{
	अगर (tdb->methods->tdb_read(tdb, offset, rec, माप(*rec),DOCONV()) == -1)
		वापस -1;
	अगर (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, offset));
		वापस TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	वापस tdb->methods->tdb_oob(tdb, rec->अग्र+माप(*rec), 0);
}

पूर्णांक tdb_rec_write(काष्ठा tdb_context *tdb, tdb_off_t offset, काष्ठा list_struct *rec)
{
	काष्ठा list_struct r = *rec;
	वापस tdb->methods->tdb_write(tdb, offset, CONVERT(r), माप(r));
}

अटल स्थिर काष्ठा tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_अग्र_hash_chain,
	tdb_oob,
	tdb_expand_file,
	tdb_brlock
};

/*
  initialise the default methods table
*/
व्योम tdb_io_init(काष्ठा tdb_context *tdb)
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

काष्ठा tdb_transaction_el {
	काष्ठा tdb_transaction_el *अग्र, *पूर्व;
	tdb_off_t offset;
	tdb_len_t लंबाई;
	अचिन्हित अक्षर *तथ्य;
};

/*
  hold the context of any current transaction
*/
काष्ठा tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	स्थिर काष्ठा tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	काष्ठा tdb_transaction_el *elements, *elements_last;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	पूर्णांक transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	पूर्णांक nesting;

	/* old file size before transaction */
	tdb_len_t old_map_आकार;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
अटल पूर्णांक transaction_read(काष्ठा tdb_context *tdb, tdb_off_t off, व्योम *buf,
			    tdb_len_t len, पूर्णांक cv)
{
	काष्ठा tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	क्रम (el=tdb->transaction->elements_last;el;el=el->पूर्व) {
		tdb_len_t partial;

		अगर (off+len <= el->offset) {
			जारी;
		}
		अगर (off >= el->offset + el->लंबाई) {
			जारी;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		अगर (off < el->offset) {
			partial = el->offset - off;
			अगर (transaction_read(tdb, off, buf, partial, cv) != 0) {
				जाओ fail;
			}
			len -= partial;
			off += partial;
			buf = (व्योम *)(partial + (अक्षर *)buf);
		}
		अगर (off + len <= el->offset + el->लंबाई) {
			partial = len;
		} अन्यथा {
			partial = el->offset + el->लंबाई - off;
		}
		स_नकल(buf, el->तथ्य + (off - el->offset), partial);
		अगर (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (व्योम *)(partial + (अक्षर *)buf);

		अगर (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			जाओ fail;
		}

		वापस 0;
	}

	/* its not in the transaction elements - do a real read */
	वापस tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	वापस -1;
}


/*
  write while in a transaction
*/
अटल पूर्णांक transaction_write(काष्ठा tdb_context *tdb, tdb_off_t off,
			     स्थिर व्योम *buf, tdb_len_t len)
{
	काष्ठा tdb_transaction_el *el, *best_el=शून्य;

	अगर (len == 0) {
		वापस 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	अगर (len == माप(tdb_off_t) && off >= FREELIST_शीर्ष &&
	    off < FREELIST_शीर्ष+TDB_HASHTABLE_आकार(tdb)) {
		u32 chain = (off-FREELIST_शीर्ष) / माप(tdb_off_t);
		स_नकल(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	क्रम (el=tdb->transaction->elements_last;el;el=el->पूर्व) {
		tdb_len_t partial;

		अगर (best_el == शून्य && off == el->offset+el->लंबाई) {
			best_el = el;
		}

		अगर (off+len <= el->offset) {
			जारी;
		}
		अगर (off >= el->offset + el->लंबाई) {
			जारी;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		अगर (off < el->offset) {
			partial = el->offset - off;
			अगर (transaction_write(tdb, off, buf, partial) != 0) {
				जाओ fail;
			}
			len -= partial;
			off += partial;
			buf = (स्थिर व्योम *)(partial + (स्थिर अक्षर *)buf);
		}
		अगर (off + len <= el->offset + el->लंबाई) {
			partial = len;
		} अन्यथा {
			partial = el->offset + el->लंबाई - off;
		}
		स_नकल(el->तथ्य + (off - el->offset), buf, partial);
		len -= partial;
		off += partial;
		buf = (स्थिर व्योम *)(partial + (स्थिर अक्षर *)buf);

		अगर (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			जाओ fail;
		}

		वापस 0;
	}

	/* see if we can append the new entry to an existing entry */
	अगर (best_el && best_el->offset + best_el->लंबाई == off &&
	    (off+len < tdb->transaction->old_map_आकार ||
	     off > tdb->transaction->old_map_आकार)) {
		अचिन्हित अक्षर *तथ्य = best_el->तथ्य;
		el = best_el;
		el->तथ्य = (अचिन्हित अक्षर *)पुनः_स्मृति(el->तथ्य,
						    el->लंबाई + len);
		अगर (el->तथ्य == शून्य) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->तथ्य = तथ्य;
			वापस -1;
		}
		अगर (buf) {
			स_नकल(el->तथ्य + el->लंबाई, buf, len);
		} अन्यथा {
			स_रखो(el->तथ्य + el->लंबाई, TDB_PAD_BYTE, len);
		}
		el->लंबाई += len;
		वापस 0;
	}

	/* add a new entry at the end of the list */
	el = (काष्ठा tdb_transaction_el *)दो_स्मृति(माप(*el));
	अगर (el == शून्य) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		वापस -1;
	}
	el->अग्र = शून्य;
	el->पूर्व = tdb->transaction->elements_last;
	el->offset = off;
	el->लंबाई = len;
	el->तथ्य = (अचिन्हित अक्षर *)दो_स्मृति(len);
	अगर (el->तथ्य == शून्य) {
		मुक्त(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		वापस -1;
	}
	अगर (buf) {
		स_नकल(el->तथ्य, buf, len);
	} अन्यथा {
		स_रखो(el->तथ्य, TDB_PAD_BYTE, len);
	}
	अगर (el->पूर्व) {
		el->पूर्व->अग्र = el;
	} अन्यथा {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_last = el;
	वापस 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	वापस -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
अटल व्योम transaction_अग्र_hash_chain(काष्ठा tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	क्रम (;h < tdb->header.hash_आकार;h++) {
		/* the +1 takes account of the freelist */
		अगर (0 != tdb->transaction->hash_heads[h+1]) {
			अवरोध;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
अटल पूर्णांक transaction_oob(काष्ठा tdb_context *tdb, tdb_off_t len,
			   पूर्णांक probe EXT2FS_ATTR((unused)))
{
	अगर (len <= tdb->map_आकार) {
		वापस 0;
	}
	वापस TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
अटल पूर्णांक transaction_expand_file(काष्ठा tdb_context *tdb, tdb_off_t आकार,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	अगर (transaction_write(tdb, आकार, शून्य, addition) != 0) {
		वापस -1;
	}

	वापस 0;
}

/*
  brlock during a transaction - ignore them
*/
अटल पूर्णांक transaction_brlock(काष्ठा tdb_context *tdb EXT2FS_ATTR((unused)),
			      tdb_off_t offset EXT2FS_ATTR((unused)),
			      पूर्णांक rw_type EXT2FS_ATTR((unused)),
			      पूर्णांक lck_type EXT2FS_ATTR((unused)),
			      पूर्णांक probe EXT2FS_ATTR((unused)),
			      माप_प्रकार len EXT2FS_ATTR((unused)))
{
	वापस 0;
}

अटल स्थिर काष्ठा tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_अग्र_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
पूर्णांक tdb_transaction_start(काष्ठा tdb_context *tdb)
{
	/* some sanity checks */
	अगर (tdb->read_only || (tdb->flags & TDB_INTERNAL) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		वापस -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	अगर (tdb->transaction != शून्य) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		वापस 0;
	}

	अगर (tdb->अंक_locks != 0 || tdb->global_lock.गिनती) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_LOCK;
		वापस -1;
	}

	अगर (tdb->travlocks.अग्र != शून्य) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_LOCK;
		वापस -1;
	}

	tdb->transaction = (काष्ठा tdb_transaction *)
		सुस्मृति(माप(काष्ठा tdb_transaction), 1);
	अगर (tdb->transaction == शून्य) {
		tdb->ecode = TDB_ERR_OOM;
		वापस -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	अगर (tdb_transaction_lock(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		वापस -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	अगर (tdb_brlock(tdb, FREELIST_शीर्ष, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		जाओ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		सुस्मृति(tdb->header.hash_आकार+1, माप(u32));
	अगर (tdb->transaction->hash_heads == शून्य) {
		tdb->ecode = TDB_ERR_OOM;
		जाओ fail;
	}
	अगर (tdb->methods->tdb_read(tdb, FREELIST_शीर्ष, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_आकार(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		जाओ fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);
	tdb->transaction->old_map_आकार = tdb->map_आकार;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	अगर (transaction_write(tdb, FREELIST_शीर्ष, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_आकार(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		जाओ fail;
	}

	वापस 0;

fail:
	tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	SAFE_FREE(tdb->transaction->hash_heads);
	SAFE_FREE(tdb->transaction);
	वापस -1;
}


/*
  cancel the current transaction
*/
पूर्णांक tdb_transaction_cancel(काष्ठा tdb_context *tdb)
{
	अगर (tdb->transaction == शून्य) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		वापस -1;
	}

	अगर (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		वापस 0;
	}

	tdb->map_आकार = tdb->transaction->old_map_आकार;

	/* free all the transaction elements */
	जबतक (tdb->transaction->elements) {
		काष्ठा tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->अग्र;
		मुक्त(el->तथ्य);
		मुक्त(el);
	}

	/* remove any global lock created during the transaction */
	अगर (tdb->global_lock.गिनती != 0) {
		tdb_brlock(tdb, FREELIST_शीर्ष, F_UNLCK, F_SETLKW, 0, 4*tdb->header.hash_आकार);
		tdb->global_lock.गिनती = 0;
	}

	/* remove any locks created during the transaction */
	अगर (tdb->अंक_locks != 0) {
		पूर्णांक i;
		क्रम (i=0;i<tdb->अंक_lockrecs;i++) {
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

	वापस 0;
}

/*
  sync to disk
*/
अटल पूर्णांक transaction_sync(काष्ठा tdb_context *tdb, tdb_off_t offset, tdb_len_t लंबाई)
{
	अगर (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		वापस -1;
	}
#अगर defined(HAVE_MSYNC) && defined(MS_SYNC)
	अगर (tdb->map_ptr) {
		tdb_off_t moffset = offset & ~(tdb->page_आकार-1);
		अगर (msync(moffset + (अक्षर *)tdb->map_ptr,
			  लंबाई + (offset - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 म_त्रुटि(त्रुटि_सं)));
			वापस -1;
		}
	}
#पूर्ण_अगर
	वापस 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
अटल tdb_len_t tdb_recovery_आकार(काष्ठा tdb_context *tdb)
{
	काष्ठा tdb_transaction_el *el;
	tdb_len_t recovery_आकार = 0;

	recovery_आकार = माप(u32);
	क्रम (el=tdb->transaction->elements;el;el=el->अग्र) {
		अगर (el->offset >= tdb->transaction->old_map_आकार) {
			जारी;
		}
		recovery_आकार += 2*माप(tdb_off_t) + el->लंबाई;
	}

	वापस recovery_आकार;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
अटल पूर्णांक tdb_recovery_allocate(काष्ठा tdb_context *tdb,
				 tdb_len_t *recovery_आकार,
				 tdb_off_t *recovery_offset,
				 tdb_len_t *recovery_max_आकार)
{
	काष्ठा list_struct rec;
	स्थिर काष्ठा tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_शीर्ष;

	अगर (tdb_ofs_read(tdb, TDB_RECOVERY_शीर्ष, &recovery_शीर्ष) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		वापस -1;
	}

	rec.rec_len = 0;

	अगर (recovery_शीर्ष != 0 &&
	    methods->tdb_read(tdb, recovery_शीर्ष, &rec, माप(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		वापस -1;
	}

	*recovery_आकार = tdb_recovery_आकार(tdb);

	अगर (recovery_शीर्ष != 0 && *recovery_आकार <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_आकार = rec.rec_len;
		*recovery_offset = recovery_शीर्ष;
		वापस 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	अगर (recovery_शीर्ष != 0) {
		अगर (tdb_free(tdb, recovery_शीर्ष, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			वापस -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_आकार = tdb_recovery_आकार(tdb);

	/* round up to a multiple of page size */
	*recovery_max_आकार = TDB_ALIGN(माप(rec) + *recovery_आकार, tdb->page_आकार) - माप(rec);
	*recovery_offset = tdb->map_आकार;
	recovery_शीर्ष = *recovery_offset;

	अगर (methods->tdb_expand_file(tdb, tdb->transaction->old_map_आकार,
				     (tdb->map_आकार - tdb->transaction->old_map_आकार) +
				     माप(rec) + *recovery_max_आकार) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		वापस -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_आकार = tdb->map_आकार;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_शीर्ष);
	अगर (methods->tdb_write(tdb, TDB_RECOVERY_शीर्ष,
			       &recovery_शीर्ष, माप(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		वापस -1;
	}

	वापस 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
अटल पूर्णांक transaction_setup_recovery(काष्ठा tdb_context *tdb,
				      tdb_off_t *magic_offset)
{
	काष्ठा tdb_transaction_el *el;
	tdb_len_t recovery_आकार;
	अचिन्हित अक्षर *तथ्य, *p;
	स्थिर काष्ठा tdb_methods *methods = tdb->transaction->io_methods;
	काष्ठा list_struct *rec;
	tdb_off_t recovery_offset, recovery_max_आकार;
	tdb_off_t old_map_आकार = tdb->transaction->old_map_आकार;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	अगर (tdb_recovery_allocate(tdb, &recovery_आकार,
				  &recovery_offset, &recovery_max_आकार) == -1) {
		वापस -1;
	}

	तथ्य = (अचिन्हित अक्षर *)दो_स्मृति(recovery_आकार + माप(*rec));
	अगर (तथ्य == शून्य) {
		tdb->ecode = TDB_ERR_OOM;
		वापस -1;
	}

	rec = (काष्ठा list_struct *)तथ्य;
	स_रखो(rec, 0, माप(*rec));

	rec->magic    = 0;
	rec->तथ्य_len = recovery_आकार;
	rec->rec_len  = recovery_max_आकार;
	rec->कुंजी_len  = old_map_आकार;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = तथ्य + माप(*rec);
	क्रम (el=tdb->transaction->elements;el;el=el->अग्र) {
		अगर (el->offset >= old_map_आकार) {
			जारी;
		}
		अगर (el->offset + el->लंबाई > tdb->transaction->old_map_आकार) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			मुक्त(तथ्य);
			tdb->ecode = TDB_ERR_CORRUPT;
			वापस -1;
		}
		स_नकल(p, &el->offset, 4);
		स_नकल(p+4, &el->लंबाई, 4);
		अगर (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		अगर (methods->tdb_read(tdb, el->offset, p + 8, el->लंबाई, 0) != 0) {
			मुक्त(तथ्य);
			tdb->ecode = TDB_ERR_IO;
			वापस -1;
		}
		p += 8 + el->लंबाई;
	}

	/* and the tailer */
	tailer = माप(*rec) + recovery_max_आकार;
	स_नकल(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	अगर (methods->tdb_write(tdb, recovery_offset, तथ्य, माप(*rec) + recovery_आकार) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		मुक्त(तथ्य);
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	अगर (transaction_sync(tdb, recovery_offset, माप(*rec) + recovery_आकार) == -1) {
		मुक्त(तथ्य);
		वापस -1;
	}

	मुक्त(तथ्य);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_offset = recovery_offset + दुरत्व(काष्ठा list_struct, magic);

	अगर (methods->tdb_write(tdb, *magic_offset, &magic, माप(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	/* ensure the recovery magic marker is on disk */
	अगर (transaction_sync(tdb, *magic_offset, माप(magic)) == -1) {
		वापस -1;
	}

	वापस 0;
}

/*
  commit the current transaction
*/
पूर्णांक tdb_transaction_commit(काष्ठा tdb_context *tdb)
{
	स्थिर काष्ठा tdb_methods *methods;
	tdb_off_t magic_offset = 0;
	u32 zero = 0;

	अगर (tdb->transaction == शून्य) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		वापस -1;
	}

	अगर (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		वापस -1;
	}

	अगर (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		वापस 0;
	}

	/* check for a null transaction */
	अगर (tdb->transaction->elements == शून्य) {
		tdb_transaction_cancel(tdb);
		वापस 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	अगर (tdb->अंक_locks || tdb->global_lock.गिनती) {
		tdb->ecode = TDB_ERR_LOCK;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		वापस -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	अगर (tdb_brlock_upgrade(tdb, FREELIST_शीर्ष, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		वापस -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	अगर (tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_LOCK;
		tdb_transaction_cancel(tdb);
		वापस -1;
	}

	अगर (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		अगर (transaction_setup_recovery(tdb, &magic_offset) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			वापस -1;
		}
	}

	/* expand the file to the new size if needed */
	अगर (tdb->map_आकार != tdb->transaction->old_map_आकार) {
		अगर (methods->tdb_expand_file(tdb, tdb->transaction->old_map_आकार,
					     tdb->map_आकार -
					     tdb->transaction->old_map_आकार) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			वापस -1;
		}
		tdb->map_आकार = tdb->transaction->old_map_आकार;
		methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);
	}

	/* perform all the writes */
	जबतक (tdb->transaction->elements) {
		काष्ठा tdb_transaction_el *el = tdb->transaction->elements;

		अगर (methods->tdb_write(tdb, el->offset, el->तथ्य, el->लंबाई) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			वापस -1;
		}
		tdb->transaction->elements = el->अग्र;
		मुक्त(el->तथ्य);
		मुक्त(el);
	}

	अगर (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		अगर (transaction_sync(tdb, 0, tdb->map_आकार) == -1) {
			वापस -1;
		}

		/* remove the recovery marker */
		अगर (methods->tdb_write(tdb, magic_offset, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			वापस -1;
		}

		/* ensure the recovery marker has been removed on disk */
		अगर (transaction_sync(tdb, magic_offset, 4) == -1) {
			वापस -1;
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
#अगर_घोषित HAVE_UTIME
	utime(tdb->name, शून्य);
#पूर्ण_अगर

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	वापस 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
पूर्णांक tdb_transaction_recover(काष्ठा tdb_context *tdb)
{
	tdb_off_t recovery_शीर्ष, recovery_eof;
	अचिन्हित अक्षर *तथ्य, *p;
	u32 zero = 0;
	काष्ठा list_struct rec;

	/* find the recovery area */
	अगर (tdb_ofs_read(tdb, TDB_RECOVERY_शीर्ष, &recovery_शीर्ष) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	अगर (recovery_शीर्ष == 0) {
		/* we have never allocated a recovery record */
		वापस 0;
	}

	/* read the recovery record */
	अगर (tdb->methods->tdb_read(tdb, recovery_शीर्ष, &rec,
				   माप(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	अगर (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		वापस 0;
	}

	अगर (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		वापस -1;
	}

	recovery_eof = rec.कुंजी_len;

	तथ्य = (अचिन्हित अक्षर *)दो_स्मृति(rec.तथ्य_len);
	अगर (तथ्य == शून्य) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		वापस -1;
	}

	/* read the full recovery data */
	अगर (tdb->methods->tdb_read(tdb, recovery_शीर्ष + माप(rec), तथ्य,
				   rec.तथ्य_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		मुक्त(तथ्य);
		वापस -1;
	}

	/* recover the file data */
	p = तथ्य;
	जबतक (p+8 < तथ्य + rec.तथ्य_len) {
		u32 ofs, len;
		अगर (DOCONV()) {
			tdb_convert(p, 8);
		}
		स_नकल(&ofs, p, 4);
		स_नकल(&len, p+4, 4);

		अगर (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			मुक्त(तथ्य);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			वापस -1;
		}
		p += 8 + len;
	}

	मुक्त(तथ्य);

	अगर (transaction_sync(tdb, 0, tdb->map_आकार) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	अगर (recovery_eof <= recovery_शीर्ष) {
		अगर (tdb_ofs_write(tdb, TDB_RECOVERY_शीर्ष, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			वापस -1;
		}
	}

	/* remove the recovery magic */
	अगर (tdb_ofs_write(tdb, recovery_शीर्ष + दुरत्व(काष्ठा list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	अगर (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}
	tdb->map_आकार = recovery_eof;
	tdb_mmap(tdb);

	अगर (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		वापस -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	वापस 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
अटल पूर्णांक tdb_rec_free_read(काष्ठा tdb_context *tdb, tdb_off_t off, काष्ठा list_struct *rec)
{
	अगर (tdb->methods->tdb_read(tdb, off, rec, माप(*rec),DOCONV()) == -1)
		वापस -1;

	अगर (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		अगर (tdb->methods->tdb_write(tdb, off, rec, माप(*rec)) == -1)
			वापस -1;
	}

	अगर (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		वापस TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	अगर (tdb->methods->tdb_oob(tdb, rec->अग्र+माप(*rec), 0) != 0)
		वापस -1;
	वापस 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
अटल पूर्णांक remove_from_freelist(काष्ठा tdb_context *tdb, tdb_off_t off, tdb_off_t अग्र)
{
	tdb_off_t last_ptr, i;

	/* read in the freelist top */
	last_ptr = FREELIST_शीर्ष;
	जबतक (tdb_ofs_read(tdb, last_ptr, &i) != -1 && i != 0) {
		अगर (i == off) {
			/* We've found it! */
			वापस tdb_ofs_write(tdb, last_ptr, &अग्र);
		}
		/* Follow chain (next offset is at start of record) */
		last_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	वापस TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
अटल पूर्णांक update_tailer(काष्ठा tdb_context *tdb, tdb_off_t offset,
			 स्थिर काष्ठा list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = माप(*rec) + rec->rec_len;
	वापस tdb_ofs_write(tdb, offset + totalsize - माप(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
पूर्णांक tdb_free(काष्ठा tdb_context *tdb, tdb_off_t offset, काष्ठा list_struct *rec)
{
	tdb_off_t दायाँ, बायाँ;

	/* Allocation and tailer lock */
	अगर (tdb_lock(tdb, -1, F_WRLCK) != 0)
		वापस -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	अगर (update_tailer(tdb, offset, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		जाओ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	दायाँ = offset + माप(*rec) + rec->rec_len;
	अगर (दायाँ + माप(*rec) <= tdb->map_आकार) {
		काष्ठा list_struct r;

		अगर (tdb->methods->tdb_read(tdb, दायाँ, &r, माप(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", दायाँ));
			जाओ बायाँ;
		}

		/* If it's free, expand to include it. */
		अगर (r.magic == TDB_FREE_MAGIC) {
			अगर (remove_from_freelist(tdb, दायाँ, r.अग्र) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", दायाँ));
				जाओ बायाँ;
			}
			rec->rec_len += माप(r) + r.rec_len;
		}
	}

बायाँ:
	/* Look left */
	बायाँ = offset - माप(tdb_off_t);
	अगर (बायाँ > TDB_तथ्य_START(tdb->header.hash_आकार)) {
		काष्ठा list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		अगर (tdb_ofs_read(tdb, बायाँ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", बायाँ));
			जाओ update;
		}

		/* it could be uninitialised data */
		अगर (leftsize == 0 || leftsize == TDB_PAD_U32) {
			जाओ update;
		}

		बायाँ = offset - leftsize;

		/* Now read in record */
		अगर (tdb->methods->tdb_read(tdb, बायाँ, &l, माप(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", बायाँ, leftsize));
			जाओ update;
		}

		/* If it's free, expand to include it. */
		अगर (l.magic == TDB_FREE_MAGIC) {
			अगर (remove_from_freelist(tdb, बायाँ, l.अग्र) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", बायाँ));
				जाओ update;
			} अन्यथा {
				offset = बायाँ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	अगर (update_tailer(tdb, offset, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", offset));
		जाओ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	अगर (tdb_ofs_read(tdb, FREELIST_शीर्ष, &rec->अग्र) == -1 ||
	    tdb_rec_write(tdb, offset, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_शीर्ष, &offset) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", offset));
		जाओ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	वापस 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	वापस -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
अटल tdb_off_t tdb_allocate_ofs(काष्ठा tdb_context *tdb, tdb_len_t लंबाई, tdb_off_t rec_ptr,
				काष्ठा list_struct *rec, tdb_off_t last_ptr)
{
	काष्ठा list_struct newrec;
	tdb_off_t newrec_ptr;

	स_रखो(&newrec, '\0', माप(newrec));

	/* found it - now possibly split it up  */
	अगर (rec->rec_len > लंबाई + MIN_REC_आकार) {
		/* Length of left piece */
		लंबाई = TDB_ALIGN(लंबाई, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (माप(*rec) + लंबाई);
		newrec_ptr = rec_ptr + माप(*rec) + लंबाई;

		/* And left record is shortened */
		rec->rec_len = लंबाई;
	} अन्यथा {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	अगर (tdb_ofs_write(tdb, last_ptr, &rec->अग्र) == -1) {
		वापस 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	अगर (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		वापस 0;
	}

	/* Did we create new block? */
	अगर (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		अगर (update_tailer(tdb, rec_ptr, rec) == -1) {
			वापस 0;
		}

		/* Free new record */
		अगर (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			वापस 0;
		}
	}

	/* all done - return the new record offset */
	वापस rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(काष्ठा tdb_context *tdb, tdb_len_t लंबाई, काष्ठा list_struct *rec)
{
	tdb_off_t rec_ptr, last_ptr, newrec_ptr;
	काष्ठा {
		tdb_off_t rec_ptr, last_ptr;
		tdb_len_t rec_len;
	} bestfit;

	अगर (tdb_lock(tdb, -1, F_WRLCK) == -1)
		वापस 0;

	/* Extra bytes required for tailer */
	लंबाई += माप(tdb_off_t);

 again:
	last_ptr = FREELIST_शीर्ष;

	/* read in the freelist top */
	अगर (tdb_ofs_read(tdb, FREELIST_शीर्ष, &rec_ptr) == -1)
		जाओ fail;

	bestfit.rec_ptr = 0;
	bestfit.last_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	जबतक (rec_ptr) {
		अगर (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			जाओ fail;
		}

		अगर (rec->rec_len >= लंबाई) {
			अगर (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.last_ptr = last_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				अगर (bestfit.rec_len < 2*लंबाई) {
					अवरोध;
				}
			}
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec->अग्र;
	}

	अगर (bestfit.rec_ptr != 0) {
		अगर (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			जाओ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, लंबाई, bestfit.rec_ptr, rec, bestfit.last_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		वापस newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	अगर (tdb_expand(tdb, लंबाई + माप(*rec)) == 0)
		जाओ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	वापस 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

अटल पूर्णांक seen_जोड़ो(काष्ठा tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_तथ्य कुंजी, तथ्य;

	स_रखो(&तथ्य, '\0', माप(तथ्य));
	कुंजी.dptr = (अचिन्हित अक्षर *)&rec_ptr;
	कुंजी.dsize = माप(rec_ptr);
	वापस tdb_store(mem_tdb, कुंजी, तथ्य, TDB_जोड़ो);
}

पूर्णांक tdb_validate_freelist(काष्ठा tdb_context *tdb, पूर्णांक *pnum_entries)
{
	काष्ठा tdb_context *mem_tdb = शून्य;
	काष्ठा list_struct rec;
	tdb_off_t rec_ptr, last_ptr;
	पूर्णांक ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->header.hash_आकार,
				TDB_INTERNAL, O_RDWR, 0600);
	अगर (!mem_tdb) {
		वापस -1;
	}

	अगर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		वापस 0;
	}

	last_ptr = FREELIST_शीर्ष;

	/* Store the FREELIST_TOP record. */
	अगर (seen_जोड़ो(mem_tdb, last_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		जाओ fail;
	}

	/* read in the freelist top */
	अगर (tdb_ofs_read(tdb, FREELIST_शीर्ष, &rec_ptr) == -1) {
		जाओ fail;
	}

	जबतक (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		अगर (seen_जोड़ो(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			जाओ fail;
		}

		अगर (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			जाओ fail;
		}

		/* move to the next record */
		last_ptr = rec_ptr;
		rec_ptr = rec.अग्र;
		*pnum_entries += 1;
	}

	ret = 0;

  fail:

	tdb_close(mem_tdb);
	tdb_unlock(tdb, -1, F_WRLCK);
	वापस ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
अटल पूर्णांक tdb_अग्र_lock(काष्ठा tdb_context *tdb, काष्ठा tdb_traverse_lock *tlock,
			 काष्ठा list_struct *rec)
{
	पूर्णांक want_अग्र = (tlock->off != 0);

	/* Lock each chain from the start one. */
	क्रम (; tlock->hash < tdb->header.hash_आकार; tlock->hash++) {
		अगर (!tlock->off && tlock->hash != 0) {
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
			tdb->methods->अग्र_hash_chain(tdb, &tlock->hash);
			अगर (tlock->hash == tdb->header.hash_आकार) {
				जारी;
			}
		}

		अगर (tdb_lock(tdb, tlock->hash, tlock->lock_rw) == -1)
			वापस -1;

		/* No previous record?  Start at top of chain. */
		अगर (!tlock->off) {
			अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(tlock->hash),
				     &tlock->off) == -1)
				जाओ fail;
		} अन्यथा {
			/* Otherwise unlock the previous record. */
			अगर (tdb_unlock_record(tdb, tlock->off) != 0)
				जाओ fail;
		}

		अगर (want_अग्र) {
			/* We have offset of old record: grab next */
			अगर (tdb_rec_read(tdb, tlock->off, rec) == -1)
				जाओ fail;
			tlock->off = rec->अग्र;
		}

		/* Iterate through chain */
		जबतक( tlock->off) {
			tdb_off_t current;
			अगर (tdb_rec_read(tdb, tlock->off, rec) == -1)
				जाओ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			अगर (tlock->off == rec->अग्र) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				जाओ fail;
			}

			अगर (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				अगर (tdb_lock_record(tdb, tlock->off) != 0)
					जाओ fail;
				वापस tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->अग्र;
			अगर (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				जाओ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->lock_rw);
		want_अग्र = 0;
	}
	/* We finished iteration without finding anything */
	वापस TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	अगर (tdb_unlock(tdb, tlock->hash, tlock->lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	वापस -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
अटल पूर्णांक tdb_traverse_internal(काष्ठा tdb_context *tdb,
				 tdb_traverse_func fn, व्योम *private_तथ्य,
				 काष्ठा tdb_traverse_lock *tl)
{
	TDB_तथ्य कुंजी, dbuf;
	काष्ठा list_struct rec;
	पूर्णांक ret, गिनती = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->अग्र = tdb->travlocks.अग्र;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.अग्र = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	जबतक ((ret = tdb_अग्र_lock(tdb, tl, &rec)) > 0) {
		गिनती++;
		/* now read the full record */
		कुंजी.dptr = tdb_alloc_read(tdb, tl->off + माप(rec),
					  rec.कुंजी_len + rec.तथ्य_len);
		अगर (!कुंजी.dptr) {
			ret = -1;
			अगर (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0)
				जाओ out;
			अगर (tdb_unlock_record(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			जाओ out;
		}
		कुंजी.dsize = rec.कुंजी_len;
		dbuf.dptr = कुंजी.dptr + rec.कुंजी_len;
		dbuf.dsize = rec.तथ्य_len;

		/* Drop chain lock, call out */
		अगर (tdb_unlock(tdb, tl->hash, tl->lock_rw) != 0) {
			ret = -1;
			SAFE_FREE(कुंजी.dptr);
			जाओ out;
		}
		अगर (fn && fn(tdb, कुंजी, dbuf, private_तथ्य)) {
			/* They want us to terminate traversal */
			ret = गिनती;
			अगर (tdb_unlock_record(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(कुंजी.dptr);
			जाओ out;
		}
		SAFE_FREE(कुंजी.dptr);
	}
out:
	tdb->travlocks.अग्र = tl->अग्र;
	अगर (ret < 0)
		वापस -1;
	अन्यथा
		वापस गिनती;
}


/*
  a write style traverse - temporarily marks the db read only
*/
पूर्णांक tdb_traverse_read(काष्ठा tdb_context *tdb,
		      tdb_traverse_func fn, व्योम *private_तथ्य)
{
	काष्ठा tdb_traverse_lock tl = { शून्य, 0, 0, F_RDLCK };
	पूर्णांक ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	अगर (tdb_transaction_lock(tdb, F_RDLCK)) {
		वापस -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_internal(tdb, fn, private_तथ्य, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	वापस ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
पूर्णांक tdb_traverse(काष्ठा tdb_context *tdb,
		 tdb_traverse_func fn, व्योम *private_तथ्य)
{
	काष्ठा tdb_traverse_lock tl = { शून्य, 0, 0, F_WRLCK };
	पूर्णांक ret;

	अगर (tdb->read_only || tdb->traverse_read) {
		वापस tdb_traverse_read(tdb, fn, private_तथ्य);
	}

	अगर (tdb_transaction_lock(tdb, F_WRLCK)) {
		वापस -1;
	}

	ret = tdb_traverse_internal(tdb, fn, private_तथ्य, &tl);

	tdb_transaction_unlock(tdb);

	वापस ret;
}


/* find the first entry in the database and return its key */
TDB_तथ्य tdb_firstkey(काष्ठा tdb_context *tdb)
{
	TDB_तथ्य कुंजी;
	काष्ठा list_struct rec;

	/* release any old lock */
	अगर (tdb_unlock_record(tdb, tdb->travlocks.off) != 0)
		वापस tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.lock_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	अगर (tdb_अग्र_lock(tdb, &tdb->travlocks, &rec) <= 0)
		वापस tdb_null;
	/* now read the key */
	कुंजी.dsize = rec.कुंजी_len;
	कुंजी.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+माप(rec),कुंजी.dsize);

	/* Unlock the hash chain of the record we just read. */
	अगर (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	वापस कुंजी;
}

/* find the next entry in the database, returning its key */
TDB_तथ्य tdb_nextkey(काष्ठा tdb_context *tdb, TDB_तथ्य oldkey)
{
	u32 oldhash;
	TDB_तथ्य कुंजी = tdb_null;
	काष्ठा list_struct rec;
	अचिन्हित अक्षर *k = शून्य;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	अगर (tdb->travlocks.off) {
		अगर (tdb_lock(tdb,tdb->travlocks.hash,tdb->travlocks.lock_rw))
			वापस tdb_null;
		अगर (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+माप(rec),
					    rec.कुंजी_len))
		    || स_भेद(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			अगर (tdb_unlock_record(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				वापस tdb_null;
			}
			अगर (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0) {
				SAFE_FREE(k);
				वापस tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	अगर (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_lock_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.lock_rw, &rec);
		अगर (!tdb->travlocks.off)
			वापस tdb_null;
		tdb->travlocks.hash = BUCKET(rec.full_hash);
		अगर (tdb_lock_record(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", म_त्रुटि(त्रुटि_सं)));
			वापस tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	अगर (tdb_अग्र_lock(tdb, &tdb->travlocks, &rec) > 0) {
		कुंजी.dsize = rec.कुंजी_len;
		कुंजी.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+माप(rec),
					  कुंजी.dsize);
		/* Unlock the chain of this new record */
		अगर (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.lock_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	अगर (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.lock_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	वापस कुंजी;
}

/* file: dump.c */

अटल tdb_off_t tdb_dump_record(काष्ठा tdb_context *tdb, पूर्णांक hash,
				 tdb_off_t offset)
{
	काष्ठा list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	अगर (tdb->methods->tdb_read(tdb, offset, (अक्षर *)&rec,
				   माप(rec), DOCONV()) == -1) {
		म_लिखो("ERROR: failed to read record at %u\n", offset);
		वापस 0;
	}

	म_लिखो(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, offset, rec.अग्र, rec.rec_len, rec.कुंजी_len, rec.तथ्य_len,
	       rec.full_hash, rec.magic);

	tailer_ofs = offset + माप(rec) + rec.rec_len - माप(tdb_off_t);

	अगर (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		म_लिखो("ERROR: failed to read tailer at %u\n", tailer_ofs);
		वापस rec.अग्र;
	}

	अगर (tailer != rec.rec_len + माप(rec)) {
		म_लिखो("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(अचिन्हित पूर्णांक)tailer, (अचिन्हित पूर्णांक)(rec.rec_len + माप(rec)));
	}
	वापस rec.अग्र;
}

अटल पूर्णांक tdb_dump_chain(काष्ठा tdb_context *tdb, पूर्णांक i)
{
	tdb_off_t rec_ptr, शीर्ष;

	शीर्ष = TDB_HASH_शीर्ष(i);

	अगर (tdb_lock(tdb, i, F_WRLCK) != 0)
		वापस -1;

	अगर (tdb_ofs_read(tdb, शीर्ष, &rec_ptr) == -1)
		वापस tdb_unlock(tdb, i, F_WRLCK);

	अगर (rec_ptr)
		म_लिखो("hash=%d\n", i);

	जबतक (rec_ptr) {
		rec_ptr = tdb_dump_record(tdb, i, rec_ptr);
	}

	वापस tdb_unlock(tdb, i, F_WRLCK);
}

व्योम tdb_dump_all(काष्ठा tdb_context *tdb)
{
	पूर्णांक i;
	क्रम (i = 0; i < (पूर्णांक)tdb->header.hash_आकार; i++) {
		tdb_dump_chain(tdb, i);
	}
	म_लिखो("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

पूर्णांक tdb_printfreelist(काष्ठा tdb_context *tdb)
{
	पूर्णांक ret;
	दीर्घ योग_free = 0;
	tdb_off_t offset, rec_ptr;
	काष्ठा list_struct rec;

	अगर ((ret = tdb_lock(tdb, -1, F_WRLCK)) != 0)
		वापस ret;

	offset = FREELIST_शीर्ष;

	/* read in the freelist top */
	अगर (tdb_ofs_read(tdb, offset, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		वापस 0;
	}

	म_लिखो("freelist top=[0x%08x]\n", rec_ptr );
	जबतक (rec_ptr) {
		अगर (tdb->methods->tdb_read(tdb, rec_ptr, (अक्षर *)&rec,
					   माप(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			वापस -1;
		}

		अगर (rec.magic != TDB_FREE_MAGIC) {
			म_लिखो("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			वापस -1;
		}

		म_लिखो("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		योग_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.अग्र;
	}
	म_लिखो("total rec_len = [0x%08x (%d)]\n", (पूर्णांक)योग_free,
               (पूर्णांक)योग_free);

	वापस tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
व्योम tdb_increment_seqnum_nonblock(काष्ठा tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	अगर (!(tdb->flags & TDB_SEQNUM)) {
		वापस;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	अगर (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		वापस;
	seqnum++;
	(व्योम) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
अटल व्योम tdb_increment_seqnum(काष्ठा tdb_context *tdb)
{
	अगर (!(tdb->flags & TDB_SEQNUM)) {
		वापस;
	}

	अगर (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		वापस;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

अटल पूर्णांक tdb_कुंजी_compare(TDB_तथ्य कुंजी, TDB_तथ्य तथ्य,
			   व्योम *private_तथ्य EXT2FS_ATTR((unused)))
{
	वापस स_भेद(तथ्य.dptr, कुंजी.dptr, तथ्य.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
अटल tdb_off_t tdb_find(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, u32 hash,
			काष्ठा list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		वापस 0;

	/* keep looking until we find the right record */
	जबतक (rec_ptr) {
		अगर (tdb_rec_read(tdb, rec_ptr, r) == -1)
			वापस 0;

		अगर (!TDB_DEAD(r) && hash==r->full_hash
		    && कुंजी.dsize==r->कुंजी_len
		    && tdb_parse_तथ्य(tdb, कुंजी, rec_ptr + माप(*r),
				      r->कुंजी_len, tdb_कुंजी_compare,
				      शून्य) == 0) {
			वापस rec_ptr;
		}
		rec_ptr = r->अग्र;
	}
	वापस TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_lock_hash(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, u32 hash, पूर्णांक locktype,
			   काष्ठा list_struct *rec)
{
	u32 rec_ptr;

	अगर (tdb_lock(tdb, BUCKET(hash), locktype) == -1)
		वापस 0;
	अगर (!(rec_ptr = tdb_find(tdb, कुंजी, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	वापस rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
अटल पूर्णांक tdb_update_hash(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, u32 hash, TDB_तथ्य dbuf)
{
	काष्ठा list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	अगर (!(rec_ptr = tdb_find(tdb, कुंजी, hash, &rec)))
		वापस -1;

	/* must be long enough key, data and tailer */
	अगर (rec.rec_len < कुंजी.dsize + dbuf.dsize + माप(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		वापस -1;
	}

	अगर (tdb->methods->tdb_write(tdb, rec_ptr + माप(rec) + rec.कुंजी_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		वापस -1;

	अगर (dbuf.dsize != rec.तथ्य_len) {
		/* update size */
		rec.तथ्य_len = dbuf.dsize;
		वापस tdb_rec_write(tdb, rec_ptr, &rec);
	}

	वापस 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_तथ्य tdb_fetch(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	tdb_off_t rec_ptr;
	काष्ठा list_struct rec;
	TDB_तथ्य ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुंजी);
	अगर (!(rec_ptr = tdb_find_lock_hash(tdb,कुंजी,hash,F_RDLCK,&rec)))
		वापस tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + माप(rec) + rec.कुंजी_len,
				  rec.तथ्य_len);
	ret.dsize = rec.तथ्य_len;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	वापस ret;
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

पूर्णांक tdb_parse_record(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी,
		     पूर्णांक (*parser)(TDB_तथ्य कुंजी, TDB_तथ्य तथ्य,
				   व्योम *private_तथ्य),
		     व्योम *private_तथ्य)
{
	tdb_off_t rec_ptr;
	काष्ठा list_struct rec;
	पूर्णांक ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुंजी);

	अगर (!(rec_ptr = tdb_find_lock_hash(tdb,कुंजी,hash,F_RDLCK,&rec))) {
		वापस TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_तथ्य(tdb, कुंजी, rec_ptr + माप(rec) + rec.कुंजी_len,
			     rec.तथ्य_len, parser, private_तथ्य);

	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);

	वापस ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
अटल पूर्णांक tdb_exists_hash(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, u32 hash)
{
	काष्ठा list_struct rec;

	अगर (tdb_find_lock_hash(tdb, कुंजी, hash, F_RDLCK, &rec) == 0)
		वापस 0;
	tdb_unlock(tdb, BUCKET(rec.full_hash), F_RDLCK);
	वापस 1;
}

पूर्णांक tdb_exists(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	u32 hash = tdb->hash_fn(&कुंजी);
	वापस tdb_exists_hash(tdb, कुंजी, hash);
}

/* actually delete an entry in the database given the offset */
पूर्णांक tdb_do_delete(काष्ठा tdb_context *tdb, tdb_off_t rec_ptr, काष्ठा list_struct*rec)
{
	tdb_off_t last_ptr, i;
	काष्ठा list_struct lastrec;

	अगर (tdb->read_only || tdb->traverse_read) वापस -1;

	अगर (tdb_write_lock_record(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		वापस tdb_rec_write(tdb, rec_ptr, rec);
	}
	अगर (tdb_write_unlock_record(tdb, rec_ptr) != 0)
		वापस -1;

	/* find previous record in hash chain */
	अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(rec->full_hash), &i) == -1)
		वापस -1;
	क्रम (last_ptr = 0; i != rec_ptr; last_ptr = i, i = lastrec.अग्र)
		अगर (tdb_rec_read(tdb, i, &lastrec) == -1)
			वापस -1;

	/* unlink it: next ptr is at start of record. */
	अगर (last_ptr == 0)
		last_ptr = TDB_HASH_शीर्ष(rec->full_hash);
	अगर (tdb_ofs_write(tdb, last_ptr, &rec->अग्र) == -1)
		वापस -1;

	/* recover the space */
	अगर (tdb_free(tdb, rec_ptr, rec) == -1)
		वापस -1;
	वापस 0;
}

अटल पूर्णांक tdb_गिनती_dead(काष्ठा tdb_context *tdb, u32 hash)
{
	पूर्णांक res = 0;
	tdb_off_t rec_ptr;
	काष्ठा list_struct rec;

	/* read in the hash top */
	अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		वापस 0;

	जबतक (rec_ptr) {
		अगर (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			वापस 0;

		अगर (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.अग्र;
	}
	वापस res;
}

/*
 * Purge all DEAD records from a hash chain
 */
अटल पूर्णांक tdb_purge_dead(काष्ठा tdb_context *tdb, u32 hash)
{
	पूर्णांक res = -1;
	काष्ठा list_struct rec;
	tdb_off_t rec_ptr;

	अगर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		वापस -1;
	}

	/* read in the hash top */
	अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		जाओ fail;

	जबतक (rec_ptr) {
		tdb_off_t अग्र;

		अगर (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			जाओ fail;
		}

		अग्र = rec.अग्र;

		अगर (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			जाओ fail;
		}
		rec_ptr = अग्र;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	वापस res;
}

/* delete an entry in the database given a key */
अटल पूर्णांक tdb_delete_hash(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, u32 hash)
{
	tdb_off_t rec_ptr;
	काष्ठा list_struct rec;
	पूर्णांक ret;

	अगर (tdb->max_dead_records != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		अगर (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
			वापस -1;

		अगर (tdb_गिनती_dead(tdb, hash) >= tdb->max_dead_records) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		अगर (!(rec_ptr = tdb_find(tdb, कुंजी, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			वापस -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	अन्यथा {
		अगर (!(rec_ptr = tdb_find_lock_hash(tdb, कुंजी, hash, F_WRLCK,
						   &rec)))
			वापस -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	अगर (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	अगर (tdb_unlock(tdb, BUCKET(rec.full_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	वापस ret;
}

पूर्णांक tdb_delete(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी)
{
	u32 hash = tdb->hash_fn(&कुंजी);
	वापस tdb_delete_hash(tdb, कुंजी, hash);
}

/*
 * See if we have a dead record around with enough space
 */
अटल tdb_off_t tdb_find_dead(काष्ठा tdb_context *tdb, u32 hash,
			       काष्ठा list_struct *r, tdb_len_t लंबाई)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1)
		वापस 0;

	/* keep looking until we find the right record */
	जबतक (rec_ptr) {
		अगर (tdb_rec_read(tdb, rec_ptr, r) == -1)
			वापस 0;

		अगर (TDB_DEAD(r) && r->rec_len >= लंबाई) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			वापस rec_ptr;
		}
		rec_ptr = r->अग्र;
	}
	वापस 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
पूर्णांक tdb_store(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, पूर्णांक flag)
{
	काष्ठा list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	अक्षर *p = शून्य;
	पूर्णांक ret = -1;

	अगर (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		वापस -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुंजी);
	अगर (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		वापस -1;

	/* check for it existing, on insert. */
	अगर (flag == TDB_जोड़ो) {
		अगर (tdb_exists_hash(tdb, कुंजी, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			जाओ fail;
		}
	} अन्यथा {
		/* first try in-place update, on modify or replace. */
		अगर (tdb_update_hash(tdb, कुंजी, hash, dbuf) == 0) {
			जाओ done;
		}
		अगर (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			जाओ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	अगर (flag != TDB_जोड़ो)
		tdb_delete_hash(tdb, कुंजी, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	अगर (!(p = (अक्षर *)दो_स्मृति(कुंजी.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		जाओ fail;
	}

	स_नकल(p, कुंजी.dptr, कुंजी.dsize);
	अगर (dbuf.dsize)
		स_नकल(p+कुंजी.dsize, dbuf.dptr, dbuf.dsize);

	अगर (tdb->max_dead_records != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			कुंजी.dsize + dbuf.dsize + माप(tdb_off_t));

		अगर (rec_ptr != 0) {
			rec.कुंजी_len = कुंजी.dsize;
			rec.तथ्य_len = dbuf.dsize;
			rec.full_hash = hash;
			rec.magic = TDB_MAGIC;
			अगर (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + माप(rec),
				    p, कुंजी.dsize + dbuf.dsize) == -1) {
				जाओ fail;
			}
			जाओ done;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	अगर (tdb_lock(tdb, -1, F_WRLCK) == -1) {
		जाओ fail;
	}

	अगर ((tdb->max_dead_records != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		जाओ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, कुंजी.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	अगर (rec_ptr == 0) {
		जाओ fail;
	}

	/* Read hash top into next ptr */
	अगर (tdb_ofs_read(tdb, TDB_HASH_शीर्ष(hash), &rec.अग्र) == -1)
		जाओ fail;

	rec.कुंजी_len = कुंजी.dsize;
	rec.तथ्य_len = dbuf.dsize;
	rec.full_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	अगर (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+माप(rec), p, कुंजी.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_शीर्ष(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		जाओ fail;
	}

 done:
	ret = 0;
 fail:
	अगर (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	वापस ret;
}


/* Append to an entry. Create if not exist. */
पूर्णांक tdb_append(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, TDB_तथ्य new_dbuf)
{
	u32 hash;
	TDB_तथ्य dbuf;
	पूर्णांक ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुंजी);
	अगर (tdb_lock(tdb, BUCKET(hash), F_WRLCK) == -1)
		वापस -1;

	dbuf = tdb_fetch(tdb, कुंजी);

	अगर (dbuf.dptr == शून्य) {
		dbuf.dptr = (अचिन्हित अक्षर *)दो_स्मृति(new_dbuf.dsize);
	} अन्यथा {
		अचिन्हित अक्षर *new_dptr = (अचिन्हित अक्षर *)पुनः_स्मृति(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		अगर (new_dptr == शून्य) {
			मुक्त(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	अगर (dbuf.dptr == शून्य) {
		tdb->ecode = TDB_ERR_OOM;
		जाओ failed;
	}

	स_नकल(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, कुंजी, dbuf, 0);

failed:
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	SAFE_FREE(dbuf.dptr);
	वापस ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
स्थिर अक्षर *tdb_name(काष्ठा tdb_context *tdb)
{
	वापस tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
पूर्णांक tdb_fd(काष्ठा tdb_context *tdb)
{
	वापस tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(काष्ठा tdb_context *tdb)
{
	वापस tdb->log.log_fn;
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
पूर्णांक tdb_get_seqnum(काष्ठा tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	अगर (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		वापस 0;
	वापस seqnum;
}

पूर्णांक tdb_hash_आकार(काष्ठा tdb_context *tdb)
{
	वापस tdb->header.hash_आकार;
}

माप_प्रकार tdb_map_आकार(काष्ठा tdb_context *tdb)
{
	वापस tdb->map_आकार;
}

पूर्णांक tdb_get_flags(काष्ठा tdb_context *tdb)
{
	वापस tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
व्योम tdb_enable_seqnum(काष्ठा tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
अटल काष्ठा tdb_context *tdbs = शून्य;


/* This is from a hash algorithm suggested by Rogier Wolff */
अटल अचिन्हित पूर्णांक default_tdb_hash(TDB_तथ्य *कुंजी)
{
	u32 मूल्य;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	क्रम (मूल्य = 0, i=0; i < कुंजी->dsize; i++)
		मूल्य = मूल्य * 256 + कुंजी->dptr[i] + (मूल्य >> 24) * 241;

	वापस मूल्य;
}


/* initialise a new database with a specified hash size */
अटल पूर्णांक tdb_new_database(काष्ठा tdb_context *tdb, पूर्णांक hash_आकार)
{
	काष्ठा tdb_header *newdb;
	पूर्णांक आकार, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	आकार = माप(काष्ठा tdb_header) + (hash_आकार+1)*माप(tdb_off_t);
	अगर (!(newdb = (काष्ठा tdb_header *)सुस्मृति(आकार, 1)))
		वापस TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->version = TDB_VERSION;
	newdb->hash_आकार = hash_आकार;
	अगर (tdb->flags & TDB_INTERNAL) {
		tdb->map_आकार = आकार;
		tdb->map_ptr = (अक्षर *)newdb;
		स_नकल(&tdb->header, newdb, माप(tdb->header));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		वापस 0;
	}
	अगर (lseek(tdb->fd, 0, शुरू_से) == -1)
		जाओ fail;

	अगर (ftruncate(tdb->fd, 0) == -1)
		जाओ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	स_नकल(&tdb->header, newdb, माप(tdb->header));
	/* Don't endian-convert the magic food! */
	स_नकल(newdb->magic_food, TDB_MAGIC_FOOD, म_माप(TDB_MAGIC_FOOD)+1);
	अगर (ग_लिखो(tdb->fd, newdb, आकार) != आकार) {
		ret = -1;
	} अन्यथा {
		ret = 0;
	}

  fail:
	SAFE_FREE(newdb);
	वापस ret;
}



अटल पूर्णांक tdb_already_open(dev_t device,
			    ino_t ino)
{
	काष्ठा tdb_context *i;

	क्रम (i = tdbs; i; i = i->अग्र) {
		अगर (i->device == device && i->inode == ino) {
			वापस 1;
		}
	}

	वापस 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
काष्ठा tdb_context *tdb_open(स्थिर अक्षर *name, पूर्णांक hash_आकार, पूर्णांक tdb_flags,
		      पूर्णांक open_flags, mode_t mode)
{
	वापस tdb_open_ex(name, hash_आकार, tdb_flags, open_flags, mode, शून्य, शून्य);
}

/* a default logging function */
अटल व्योम null_log_fn(काष्ठा tdb_context *tdb, क्रमागत tdb_debug_level level, स्थिर अक्षर *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
अटल व्योम null_log_fn(काष्ठा tdb_context *tdb EXT2FS_ATTR((unused)),
			क्रमागत tdb_debug_level level EXT2FS_ATTR((unused)),
			स्थिर अक्षर *fmt EXT2FS_ATTR((unused)), ...)
{
}


काष्ठा tdb_context *tdb_open_ex(स्थिर अक्षर *name, पूर्णांक hash_आकार, पूर्णांक tdb_flags,
				पूर्णांक open_flags, mode_t mode,
				स्थिर काष्ठा tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	काष्ठा tdb_context *tdb;
	काष्ठा stat st;
	पूर्णांक rev = 0, locked = 0;
	अचिन्हित अक्षर *vp;
	u32 vertest;

	अगर (!(tdb = (काष्ठा tdb_context *)सुस्मृति(1, माप *tdb))) {
		/* Can't log this */
		त्रुटि_सं = ENOMEM;
		जाओ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = शून्य;
	tdb->map_ptr = शून्य;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	अगर (log_ctx) {
		tdb->log = *log_ctx;
	} अन्यथा {
		tdb->log.log_fn = null_log_fn;
		tdb->log.log_private = शून्य;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_आकार = sysconf(_SC_PAGESIZE);
	अगर (tdb->page_आकार <= 0) {
		tdb->page_आकार = 0x2000;
	}

	अगर ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		त्रुटि_सं = EINVAL;
		जाओ fail;
	}

	अगर (hash_आकार == 0)
		hash_आकार = DEFAULT_HASH_आकार;
	अगर ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	अगर (tdb->flags & TDB_INTERNAL) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		अगर (tdb_new_database(tdb, hash_आकार) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			जाओ fail;
		}
		जाओ internal;
	}

	अगर ((tdb->fd = खोलो(name, open_flags, mode)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, म_त्रुटि(त्रुटि_सं)));
		जाओ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	अगर (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, म_त्रुटि(त्रुटि_सं)));
		जाओ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	अगर ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		अगर (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, म_त्रुटि(त्रुटि_सं)));
			जाओ fail; /* errno set by ftruncate */
		}
	}

	अगर (पढ़ो(tdb->fd, &tdb->header, माप(tdb->header)) != माप(tdb->header)
	    || स_भेद(tdb->header.magic_food, TDB_MAGIC_FOOD,
		      माप(TDB_MAGIC_FOOD)) != 0
	    || (tdb->header.version != TDB_VERSION
		&& !(rev = (tdb->header.version==TDB_BYTEREV(TDB_VERSION))))) {
		/* its not a valid database - possibly initialise it */
		अगर (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_आकार) == -1) {
			त्रुटि_सं = EIO; /* ie bad format or something */
			जाओ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (अचिन्हित अक्षर *)&tdb->header.version;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_VERSION) ? TDB_BIGENDIAN : 0;
	अगर (!rev)
		tdb->flags &= ~TDB_CONVERT;
	अन्यथा {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->header, माप(tdb->header));
	}
	अगर (ख_स्थिति(tdb->fd, &st) == -1)
		जाओ fail;

	अगर (tdb->header.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		जाओ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	अगर (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (पूर्णांक)st.st_dev, (पूर्णांक)st.st_ino));
		त्रुटि_सं = EBUSY;
		जाओ fail;
	}

	अगर (!(tdb->name = (अक्षर *)strdup(name))) {
		त्रुटि_सं = ENOMEM;
		जाओ fail;
	}

	tdb->map_आकार = st.st_आकार;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_records = 0;
	tdb_mmap(tdb);
	अगर (locked) {
		अगर (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, म_त्रुटि(त्रुटि_सं)));
			जाओ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	अगर (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		अगर (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)
			जाओ fail;
	}

	/* if needed, run recovery */
	अगर (tdb_transaction_recover(tdb) == -1) {
		जाओ fail;
	}

 internal:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	अगर (tdb->methods->tdb_brlock(tdb, GLOBAL_LOCK, F_UNLCK, F_SETLKW, 0, 1) == -1)
		जाओ fail;
	tdb->अग्र = tdbs;
	tdbs = tdb;
	वापस tdb;

 fail:
	{ पूर्णांक save_errno = त्रुटि_सं;

	अगर (!tdb)
		वापस शून्य;

	अगर (tdb->map_ptr) {
		अगर (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		अन्यथा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	अगर (tdb->fd != -1)
		अगर (बंद(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	त्रुटि_सं = save_errno;
	वापस शून्य;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

व्योम tdb_set_max_dead(काष्ठा tdb_context *tdb, पूर्णांक max_dead)
{
	tdb->max_dead_records = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
पूर्णांक tdb_close(काष्ठा tdb_context *tdb)
{
	काष्ठा tdb_context **i;
	पूर्णांक ret = 0;

	अगर (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	अगर (tdb->map_ptr) {
		अगर (tdb->flags & TDB_INTERNAL)
			SAFE_FREE(tdb->map_ptr);
		अन्यथा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	अगर (tdb->fd != -1)
		ret = बंद(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	क्रम (i = &tdbs; *i; i = &(*i)->अग्र) {
		अगर (*i == tdb) {
			*i = tdb->अग्र;
			अवरोध;
		}
	}

	स_रखो(tdb, 0, माप(*tdb));
	SAFE_FREE(tdb);

	वापस ret;
}

/* register a logging function */
व्योम tdb_set_logging_function(काष्ठा tdb_context *tdb,
                              स्थिर काष्ठा tdb_logging_context *log_ctx)
{
        tdb->log = *log_ctx;
}

व्योम *tdb_get_logging_private(काष्ठा tdb_context *tdb)
{
	वापस tdb->log.log_private;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
पूर्णांक tdb_reopen(काष्ठा tdb_context *tdb)
{
	काष्ठा stat st;

	अगर (tdb->flags & TDB_INTERNAL) {
		वापस 0; /* Nothing to do. */
	}

	अगर (tdb->अंक_locks != 0 || tdb->global_lock.गिनती) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		जाओ fail;
	}

	अगर (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		जाओ fail;
	}

	अगर (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		जाओ fail;
	}
	अगर (बंद(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = खोलो(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	अगर (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		जाओ fail;
	}
	अगर ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, ACTIVE_LOCK, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		जाओ fail;
	}
	अगर (ख_स्थिति(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", म_त्रुटि(त्रुटि_सं)));
		जाओ fail;
	}
	अगर (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		जाओ fail;
	}
	tdb_mmap(tdb);

	वापस 0;

fail:
	tdb_close(tdb);
	वापस -1;
}

/* reopen all tdb's */
पूर्णांक tdb_reopen_all(पूर्णांक parent_longlived)
{
	काष्ठा tdb_context *tdb;

	क्रम (tdb=tdbs; tdb; tdb = tdb->अग्र) {
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
		अगर (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		अगर (tdb_reopen(tdb) != 0)
			वापस -1;
	}

	वापस 0;
}

/**
 * Flush a database file from the page cache.
 **/
पूर्णांक tdb_flush(काष्ठा tdb_context *tdb)
{
	अगर (tdb->fd != -1)
		वापस fsync(tdb->fd);
	वापस 0;
}
