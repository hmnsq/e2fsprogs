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

#यदि_परिभाषितम् CONFIG_STAND_ALONE
#परिभाषय HAVE_MMAP
#परिभाषय HAVE_STRDUP
#परिभाषय HAVE_SYS_MMAN_H
#परिभाषय HAVE_UTIME_H
#परिभाषय HAVE_UTIME
#यदि_अन्तः
#यदि_अपरिभाषितम् __FreeBSD__
#परिभाषय _XOPEN_SOURCE 600
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <string.h>
#यदि_परिभाषितम् HAVE_SYS_SELECT_H
#अन्तर्भाव <sys/select.h>
#यदि_अन्तः
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <time.h>
#यदि_परिभाषितम् HAVE_UTIME_H
#अन्तर्भाव <utime.h>
#यदि_अन्तः
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/file.h>
#अन्तर्भाव <fcntl.h>

#यदि_परिभाषितम् HAVE_SYS_MMAN_H
#अन्तर्भाव <sys/mman.h>
#यदि_अन्तः

#यदि_परिभाषितम् __GNUC__
#परिभाषय EXT2FS_ATTR(x) __attribute__(x)
#अन्यथा
#परिभाषय EXT2FS_ATTR(x)
#यदि_अन्तः

#यदि_अपरिभाषितम् MAP_FILE
#परिभाषय MAP_FILE 0
#यदि_अन्तः

#यदि_अपरिभाषितम् MAP_FAILED
#परिभाषय MAP_FAILED ((शून्य *)-1)
#यदि_अन्तः

#यदि_अपरिभाषितम् HAVE_STRDUP
#परिभाषय strdup rep_strdup
स्थिर वर्ण *rep_strdup(ध्रुव वर्ण *s)
{
	वर्ण *ret;
	पूर्णाङ्क दीर्घता;

	यदि (!s)
		प्रत्यागम रिक्त;
	दीर्घता = वाक्यदैर्घ्यम्(s);
	ret = स्मृतिं_देहि(दीर्घता + 1);
	यदि (ret) {
		strncpy(ret, s, दीर्घता);
		ret[दीर्घता] = '\0';
	}
	प्रत्यागम ret;
}
#यदि_अन्तः

#यदि_अपरिभाषितम् PRINTF_ATTRIBUTE
#यदि (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#परिभाषय PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#अन्यथा
#परिभाषय PRINTF_ATTRIBUTE(a1, a2)
#यदि_अन्तः
#यदि_अन्तः

#यदि defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
प्रकारनिर्देश पूर्णाङ्क bool;
#यदि_अन्तः

#अन्तर्भाव "tdb.h"

स्थिर TDB_दत्त tdb_null;

#यदि_अपरिभाषितम् u32
#परिभाषय u32 अचिह्नित
#यदि_अन्तः

प्रकारनिर्देश u32 tdb_len_t;
प्रकारनिर्देश u32 tdb_off_t;

#यदि_अपरिभाषितम् offsetof
#परिभाषय offsetof(t,f) ((अचिह्नित पूर्णाङ्क)&((t *)0)->f)
#यदि_अन्तः

#परिभाषय TDB_MAGIC_FOOD "TDB file\n"
#परिभाषय TDB_संस्करणम् (0x26011967 + 6)
#परिभाषय TDB_MAGIC (0x26011999U)
#परिभाषय TDB_FREE_MAGIC (~TDB_MAGIC)
#परिभाषय TDB_DEAD_MAGIC (0xFEE1DEAD)
#परिभाषय TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#परिभाषय TDB_ALIGNMENT 4
#परिभाषय MIN_REC_आकार (2*परिमाण(संरचना list_struct) + TDB_ALIGNMENT)
#परिभाषय DEFAULT_HASH_आकार 131
#परिभाषय FREELIST_शिखरम् (परिमाण(संरचना tdb_शीर्षकम्))
#परिभाषय TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#परिभाषय TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#परिभाषय TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#परिभाषय TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#परिभाषय TDB_HASH_शिखरम्(hash) (FREELIST_शिखरम् + (BUCKET(hash)+1)*परिमाण(tdb_off_t))
#परिभाषय TDB_HASHTABLE_आकार(tdb) ((tdb->शीर्षकम्.hash_आकार+1)*परिमाण(tdb_off_t))
#परिभाषय TDB_दत्त_START(hash_आकार) TDB_HASH_शिखरम्(hash_आकार-1)
#परिभाषय TDB_RECOVERY_शीर्ष offsetof(संरचना tdb_शीर्षकम्, recovery_start)
#परिभाषय TDB_SEQNUM_OFS    offsetof(संरचना tdb_शीर्षकम्, sequence_सङ्ख्या)
#परिभाषय TDB_PAD_BYTE 0x42
#परिभाषय TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#परिभाषय TDB_LOG(x) tdb->लघुगणकः.log_fn x

/* lock offsets */
#परिभाषय GLOBAL_तालकम्      0
#परिभाषय सक्रिय_तालकम्      4
#परिभाषय TRANSACTION_तालकम् 8

/* free memory if the pointer is valid and zero the pointer */
#यदि_अपरिभाषितम् SAFE_FREE
#परिभाषय SAFE_FREE(x) कुरु { यदि ((x) != रिक्त) {मुक्त(x); (x)=रिक्त;} } यावत्(0)
#यदि_अन्तः

#परिभाषय BUCKET(hash) ((hash) % tdb->शीर्षकम्.hash_आकार)

#परिभाषय DOCONV() (tdb->flags & TDB_CONVERT)
#परिभाषय CONVERT(x) (DOCONV() ? tdb_convert(&x, परिमाण(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
संरचना list_struct {
	tdb_off_t अग्र; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t कुञ्चिका_len; /* byte length of key */
	tdb_len_t दत्त_len; /* byte length of data */
	u32 पूर्णम्_hash; /* the full 32 bit hash of the key */
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
संरचना tdb_शीर्षकम् {
	वर्ण magic_food[32]; /* for /etc/magic */
	u32 संस्करणम्; /* version of the code */
	u32 hash_आकार; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_start; /* offset of transaction recovery region */
	tdb_off_t sequence_सङ्ख्या; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

संरचना tdb_तालकम्_type {
	पूर्णाङ्क list;
	u32 गणनम्;
	u32 ltype;
};

संरचना tdb_traverse_तालकम् {
	संरचना tdb_traverse_तालकम् *अग्र;
	u32 off;
	u32 hash;
	पूर्णाङ्क तालकम्_rw;
};


संरचना tdb_methods {
	पूर्णाङ्क (*tdb_read)(संरचना tdb_context *, tdb_off_t , शून्य *, tdb_len_t , पूर्णाङ्क );
	पूर्णाङ्क (*tdb_write)(संरचना tdb_context *, tdb_off_t, ध्रुव शून्य *, tdb_len_t);
	शून्य (*अग्र_hash_chain)(संरचना tdb_context *, u32 *);
	पूर्णाङ्क (*tdb_oob)(संरचना tdb_context *, tdb_off_t , पूर्णाङ्क );
	पूर्णाङ्क (*tdb_expand_file)(संरचना tdb_context *, tdb_off_t , tdb_off_t );
	पूर्णाङ्क (*tdb_brlock)(संरचना tdb_context *, tdb_off_t , पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क, size_t);
};

संरचना tdb_context {
	वर्ण *name; /* the name of the database */
	शून्य *map_ptr; /* where it is currently mapped */
	पूर्णाङ्क fd; /* open file descriptor for the database */
	tdb_len_t map_आकार; /* how much space has been mapped */
	पूर्णाङ्क read_only; /* opened read-only */
	पूर्णाङ्क traverse_read; /* read-only traversal */
	संरचना tdb_तालकम्_type global_तालकम्;
	पूर्णाङ्क अङ्कः_lockrecs;
	संरचना tdb_तालकम्_type *lockrecs; /* only real locks, all with count>0 */
	गणना TDB_ERROR ecode; /* error code for last tdb error */
	संरचना tdb_शीर्षकम् शीर्षकम्; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	संरचना tdb_traverse_तालकम् travlocks; /* current traversal locks */
	संरचना tdb_context *अग्र; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	संरचना tdb_logging_context लघुगणकः;
	अचिह्नित पूर्णाङ्क (*hash_fn)(TDB_दत्त *कुञ्चिका);
	पूर्णाङ्क open_flags; /* flags used in the open - needed by reopen */
	अचिह्नित पूर्णाङ्क अङ्कः_locks; /* number of chain locks held */
	ध्रुव संरचना tdb_methods *methods;
	संरचना tdb_transaction *transaction;
	पूर्णाङ्क page_आकार;
	पूर्णाङ्क max_dead_लेखाः;
	bool have_transaction_तालकम्;
	tdb_len_t real_map_आकार; /* how much space has been mapped */
};


/*
  internal prototypes
*/
स्थिर पूर्णाङ्क tdb_munmap(संरचना tdb_context *tdb);
स्थिर शून्य tdb_mmap(संरचना tdb_context *tdb);
स्थिर पूर्णाङ्क tdb_तालकम्(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype);
पूर्णाङ्क tdb_तालकम्_nonblock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype);
स्थिर पूर्णाङ्क tdb_unlock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype);
स्थिर पूर्णाङ्क tdb_brlock(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, पूर्णाङ्क rw_type, पूर्णाङ्क lck_type, पूर्णाङ्क probe, size_t len);
स्थिर पूर्णाङ्क tdb_transaction_तालकम्(संरचना tdb_context *tdb, पूर्णाङ्क ltype);
स्थिर पूर्णाङ्क tdb_transaction_unlock(संरचना tdb_context *tdb);
स्थिर पूर्णाङ्क tdb_brlock_upgrade(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, size_t len);
स्थिर पूर्णाङ्क tdb_write_तालकम्_लेखः(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_write_unlock_लेखः(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_off_t *d);
स्थिर पूर्णाङ्क tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_off_t *d);
स्थिर शून्य *tdb_convert(शून्य *buf, u32 आकार);
स्थिर पूर्णाङ्क tdb_free(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, संरचना list_struct *rec);
स्थिर tdb_off_t tdb_allocate(संरचना tdb_context *tdb, tdb_len_t दीर्घता, संरचना list_struct *rec);
स्थिर पूर्णाङ्क tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_off_t *d);
स्थिर पूर्णाङ्क tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_off_t *d);
स्थिर पूर्णाङ्क tdb_तालकम्_लेखः(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_unlock_लेखः(संरचना tdb_context *tdb, tdb_off_t off);
स्थिर पूर्णाङ्क tdb_rec_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, संरचना list_struct *rec);
स्थिर पूर्णाङ्क tdb_rec_write(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, संरचना list_struct *rec);
स्थिर पूर्णाङ्क tdb_do_delete(संरचना tdb_context *tdb, tdb_off_t rec_ptr, संरचना list_struct *rec);
स्थिर अचिह्नित वर्ण *tdb_alloc_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_len_t len);
स्थिर पूर्णाङ्क tdb_parse_दत्त(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका,
		   tdb_off_t विस्थापनम्, tdb_len_t len,
		   पूर्णाङ्क (*parser)(TDB_दत्त कुञ्चिका, TDB_दत्त दत्त,
				 शून्य *वैयक्तिकम्_दत्त),
		   शून्य *वैयक्तिकम्_दत्त);
स्थिर tdb_off_t tdb_find_तालकम्_hash(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, u32 hash, पूर्णाङ्क locktype,
			   संरचना list_struct *rec);
स्थिर शून्य tdb_io_init(संरचना tdb_context *tdb);
स्थिर पूर्णाङ्क tdb_expand(संरचना tdb_context *tdb, tdb_off_t आकार);
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
	     {TDB_ERR_तालकम्, "Locking error"},
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
		यदि (tdb->ecode == emap[i].ecode)
			प्रत्यागम emap[i].estring;
	प्रत्यागम "Invalid error code";
}

/* file: lock.c */

#परिभाषय TDB_MARK_तालकम् 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
पूर्णाङ्क tdb_brlock(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्,
	       पूर्णाङ्क rw_type, पूर्णाङ्क lck_type, पूर्णाङ्क probe, size_t len)
{
	संरचना flock fl;
	पूर्णाङ्क ret;

	यदि (tdb->flags & TDB_NOLOCK) {
		प्रत्यागम 0;
	}

	यदि ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_start = विस्थापनम्;
	fl.l_len = len;
	fl.l_pid = 0;

	कुरु {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} यावत् (ret == -1 && errno == EINTR);

	यदि (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		यदि (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_तालकम्;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, विस्थापनम्, rw_type, lck_type, (पूर्णाङ्क)len));
		}
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}
	प्रत्यागम 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
पूर्णाङ्क tdb_brlock_upgrade(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, size_t len)
{
	पूर्णाङ्क गणनम् = 1000;
	यावत् (गणनम्--) {
		संरचना timeval tv;
		यदि (tdb_brlock(tdb, विस्थापनम्, F_WRLCK, F_SETLKW, 1, len) == 0) {
			प्रत्यागम 0;
		}
		यदि (errno != EDEADLK) {
			भङ्ग;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, रिक्त, रिक्त, रिक्त, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", विस्थापनम्));
	प्रत्यागम -1;
}


/* lock a list in the database. list -1 is the alloc list */
स्थिर पूर्णाङ्क tdb_तालकम्_2(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype, पूर्णाङ्क op)
{
	संरचना tdb_तालकम्_type *new_lck;
	पूर्णाङ्क i;
	bool mark_तालकम् = ((ltype & TDB_MARK_तालकम्) == TDB_MARK_तालकम्);

	ltype &= ~TDB_MARK_तालकम्;

	/* a global lock allows us to avoid per chain locks */
	यदि (tdb->global_तालकम्.गणनम् &&
	    ((u32)ltype == tdb->global_तालकम्.ltype || ltype == F_RDLCK)) {
		प्रत्यागम 0;
	}

	यदि (tdb->global_तालकम्.गणनम्) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}

	यदि (list < -1 || list >= (पूर्णाङ्क)tdb->शीर्षकम्.hash_आकार) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		प्रत्यागम -1;
	}
	यदि (tdb->flags & TDB_NOLOCK)
		प्रत्यागम 0;

	प्रति (i=0; i<tdb->अङ्कः_lockrecs; i++) {
		यदि (tdb->lockrecs[i].list == list) {
			यदि (tdb->lockrecs[i].गणनम् == 0) {
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

	new_lck = (संरचना tdb_तालकम्_type *)realloc(
		tdb->lockrecs,
		परिमाण(*tdb->lockrecs) * (tdb->अङ्कः_lockrecs+1));
	यदि (new_lck == रिक्त) {
		errno = ENOMEM;
		प्रत्यागम -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	यदि (!mark_तालकम् &&
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
पूर्णाङ्क tdb_तालकम्(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype)
{
	पूर्णाङ्क ret;
	ret = tdb_तालकम्_2(tdb, list, ltype, F_SETLKW);
	यदि (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	प्रत्यागम ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
पूर्णाङ्क tdb_तालकम्_nonblock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype)
{
	प्रत्यागम tdb_तालकम्_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
पूर्णाङ्क tdb_unlock(संरचना tdb_context *tdb, पूर्णाङ्क list, पूर्णाङ्क ltype)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क i;
	संरचना tdb_तालकम्_type *lck = रिक्त;
	bool mark_तालकम् = ((ltype & TDB_MARK_तालकम्) == TDB_MARK_तालकम्);

	ltype &= ~TDB_MARK_तालकम्;

	/* a global lock allows us to avoid per chain locks */
	यदि (tdb->global_तालकम्.गणनम् &&
	    ((u32)ltype == tdb->global_तालकम्.ltype || ltype == F_RDLCK)) {
		प्रत्यागम 0;
	}

	यदि (tdb->global_तालकम्.गणनम्) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}

	यदि (tdb->flags & TDB_NOLOCK)
		प्रत्यागम 0;

	/* Sanity checks */
	यदि (list < -1 || list >= (पूर्णाङ्क)tdb->शीर्षकम्.hash_आकार) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->शीर्षकम्.hash_आकार));
		प्रत्यागम ret;
	}

	प्रति (i=0; i<tdb->अङ्कः_lockrecs; i++) {
		यदि (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			भङ्ग;
		}
	}

	यदि ((lck == रिक्त) || (lck->गणनम् == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		प्रत्यागम -1;
	}

	यदि (lck->गणनम् > 1) {
		lck->गणनम्--;
		प्रत्यागम 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	यदि (mark_तालकम्) {
		ret = 0;
	} अन्यथा {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_शिखरम्+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->अङ्कः_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	यदि (tdb->अङ्कः_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->अङ्कः_lockrecs-1];
	}
	tdb->अङ्कः_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	यदि (tdb->अङ्कः_lockrecs == 0) {
		SAFE_FREE(tdb->lockrecs);
	}

	यदि (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	प्रत्यागम ret;
}

/*
  get the transaction lock
 */
पूर्णाङ्क tdb_transaction_तालकम्(संरचना tdb_context *tdb, पूर्णाङ्क ltype)
{
	यदि (tdb->have_transaction_तालकम् || tdb->global_तालकम्.गणनम्) {
		प्रत्यागम 0;
	}
	यदि (tdb->methods->tdb_brlock(tdb, TRANSACTION_तालकम्, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_तालकम्;
		प्रत्यागम -1;
	}
	tdb->have_transaction_तालकम् = 1;
	प्रत्यागम 0;
}

/*
  release the transaction lock
 */
पूर्णाङ्क tdb_transaction_unlock(संरचना tdb_context *tdb)
{
	पूर्णाङ्क ret;
	यदि (!tdb->have_transaction_तालकम्) {
		प्रत्यागम 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_तालकम्, F_UNLCK, F_SETLKW, 0, 1);
	यदि (ret == 0) {
		tdb->have_transaction_तालकम् = 0;
	}
	प्रत्यागम ret;
}




/* lock/unlock entire database */
स्थिर पूर्णाङ्क _tdb_lockall(संरचना tdb_context *tdb, पूर्णाङ्क ltype, पूर्णाङ्क op)
{
	bool mark_तालकम् = ((ltype & TDB_MARK_तालकम्) == TDB_MARK_तालकम्);

	ltype &= ~TDB_MARK_तालकम्;

	/* There are no locks on read-only dbs */
	यदि (tdb->read_only || tdb->traverse_read)
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);

	यदि (tdb->global_तालकम्.गणनम् && tdb->global_तालकम्.ltype == (u32)ltype) {
		tdb->global_तालकम्.गणनम्++;
		प्रत्यागम 0;
	}

	यदि (tdb->global_तालकम्.गणनम्) {
		/* a global lock of a different type exists */
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}

	यदि (tdb->अङ्कः_locks != 0) {
		/* can't combine global and chain locks */
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}

	यदि (!mark_तालकम् &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शिखरम्, ltype, op,
				     0, 4*tdb->शीर्षकम्.hash_आकार)) {
		यदि (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		प्रत्यागम -1;
	}

	tdb->global_तालकम्.गणनम् = 1;
	tdb->global_तालकम्.ltype = ltype;

	प्रत्यागम 0;
}



/* unlock entire db */
स्थिर पूर्णाङ्क _tdb_unlockall(संरचना tdb_context *tdb, पूर्णाङ्क ltype)
{
	bool mark_तालकम् = ((ltype & TDB_MARK_तालकम्) == TDB_MARK_तालकम्);

	ltype &= ~TDB_MARK_तालकम्;

	/* There are no locks on read-only dbs */
	यदि (tdb->read_only || tdb->traverse_read) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}

	यदि (tdb->global_तालकम्.ltype != (u32)ltype ||
	    tdb->global_तालकम्.गणनम् == 0) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_तालकम्, -1);
	}

	यदि (tdb->global_तालकम्.गणनम् > 1) {
		tdb->global_तालकम्.गणनम्--;
		प्रत्यागम 0;
	}

	यदि (!mark_तालकम् &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_शिखरम्, F_UNLCK, F_SETLKW,
				     0, 4*tdb->शीर्षकम्.hash_आकार)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		प्रत्यागम -1;
	}

	tdb->global_तालकम्.गणनम् = 0;
	tdb->global_तालकम्.ltype = 0;

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
	प्रत्यागम _tdb_lockall(tdb, F_WRLCK | TDB_MARK_तालकम्, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
पूर्णाङ्क tdb_lockall_unmark(संरचना tdb_context *tdb)
{
	प्रत्यागम _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_तालकम्);
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
पूर्णाङ्क tdb_chainlock(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_तालकम्(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
पूर्णाङ्क tdb_chainlock_nonblock(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_तालकम्_nonblock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णाङ्क tdb_chainlock_mark(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_तालकम्(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK | TDB_MARK_तालकम्);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
पूर्णाङ्क tdb_chainlock_unmark(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK | TDB_MARK_तालकम्);
}

पूर्णाङ्क tdb_chainunlock(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_WRLCK);
}

पूर्णाङ्क tdb_chainlock_read(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_तालकम्(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_RDLCK);
}

पूर्णाङ्क tdb_chainunlock_read(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	प्रत्यागम tdb_unlock(tdb, BUCKET(tdb->hash_fn(&कुञ्चिका)), F_RDLCK);
}



/* record lock stops delete underneath */
पूर्णाङ्क tdb_तालकम्_लेखः(संरचना tdb_context *tdb, tdb_off_t off)
{
	प्रत्यागम off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णाङ्क tdb_write_तालकम्_लेखः(संरचना tdb_context *tdb, tdb_off_t off)
{
	संरचना tdb_traverse_तालकम् *i;
	प्रति (i = &tdb->travlocks; i; i = i->अग्र)
		यदि (i->off == off)
			प्रत्यागम -1;
	प्रत्यागम tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
पूर्णाङ्क tdb_write_unlock_लेखः(संरचना tdb_context *tdb, tdb_off_t off)
{
	प्रत्यागम tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
पूर्णाङ्क tdb_unlock_लेखः(संरचना tdb_context *tdb, tdb_off_t off)
{
	संरचना tdb_traverse_तालकम् *i;
	u32 गणनम् = 0;

	यदि (off == 0)
		प्रत्यागम 0;
	प्रति (i = &tdb->travlocks; i; i = i->अग्र)
		यदि (i->off == off)
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
	यदि (len <= tdb->map_आकार)
		प्रत्यागम 0;
	यदि (tdb->flags & TDB_आन्तरिकम्) {
		यदि (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (पूर्णाङ्क)len, (पूर्णाङ्क)tdb->map_आकार));
		}
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	यदि (fstat(tdb->fd, &st) == -1) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	यदि (st.st_आकार < (off_t)len) {
		यदि (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (पूर्णाङ्क)len, (पूर्णाङ्क)st.st_आकार));
		}
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	यदि (tdb_munmap(tdb) == -1)
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_आकार = st.st_आकार;
	tdb_mmap(tdb);
	प्रत्यागम 0;
}

/* write a lump of data at a specified offset */
स्थिर पूर्णाङ्क tdb_write(संरचना tdb_context *tdb, tdb_off_t off,
		     ध्रुव शून्य *buf, tdb_len_t len)
{
	यदि (len == 0) {
		प्रत्यागम 0;
	}

	यदि (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	यदि (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		प्रत्यागम -1;

	यदि (tdb->map_ptr) {
		स्मृतिं_प्रतिलिख(off + (वर्ण *)tdb->map_ptr, buf, len);
	} अन्यथा यदि (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	प्रत्यागम 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
शून्य *tdb_convert(शून्य *buf, u32 आकार)
{
	u32 i, *p = (u32 *)buf;
	प्रति (i = 0; i < आकार / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	प्रत्यागम buf;
}


/* read a lump of data at a specified offset, maybe convert */
स्थिर पूर्णाङ्क tdb_read(संरचना tdb_context *tdb, tdb_off_t off, शून्य *buf,
		    tdb_len_t len, पूर्णाङ्क cv)
{
	यदि (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		प्रत्यागम -1;
	}

	यदि (tdb->map_ptr) {
		स्मृतिं_प्रतिलिख(buf, off + (वर्ण *)tdb->map_ptr, len);
	} अन्यथा {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		यदि (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (पूर्णाङ्क)off, (पूर्णाङ्क)len, (पूर्णाङ्क)ret, strerror(errno),
				 (पूर्णाङ्क)tdb->map_आकार));
			प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	यदि (cv) {
		tdb_convert(buf, len);
	}
	प्रत्यागम 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
स्थिर शून्य tdb_अग्र_hash_chain(संरचना tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	यदि (tdb->map_ptr) {
		प्रति (;h < tdb->शीर्षकम्.hash_आकार;h++) {
			यदि (0 != *(u32 *)(TDB_HASH_शिखरम्(h) + (अचिह्नित वर्ण *)tdb->map_ptr)) {
				भङ्ग;
			}
		}
	} अन्यथा {
		u32 off=0;
		प्रति (;h < tdb->शीर्षकम्.hash_आकार;h++) {
			यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(h), &off) != 0 || off != 0) {
				भङ्ग;
			}
		}
	}
	(*chain) = h;
}


पूर्णाङ्क tdb_munmap(संरचना tdb_context *tdb)
{
	यदि (tdb->flags & TDB_आन्तरिकम्)
		प्रत्यागम 0;

#यदि_परिभाषितम् HAVE_MMAP
	यदि (tdb->map_ptr) {
		पूर्णाङ्क ret = munmap(tdb->map_ptr, tdb->real_map_आकार);
		यदि (ret != 0)
			प्रत्यागम ret;
		tdb->real_map_आकार = 0;
	}
#यदि_अन्तः
	tdb->map_ptr = रिक्त;
	प्रत्यागम 0;
}

शून्य tdb_mmap(संरचना tdb_context *tdb)
{
	यदि (tdb->flags & TDB_आन्तरिकम्)
		प्रत्यागम;

#यदि_परिभाषितम् HAVE_MMAP
	यदि (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(रिक्त, tdb->map_आकार,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		यदि (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_आकार = 0;
			tdb->map_ptr = रिक्त;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_आकार, strerror(errno)));
		}
		tdb->real_map_आकार = tdb->map_आकार;
	} अन्यथा {
		tdb->map_ptr = रिक्त;
	}
#अन्यथा
	tdb->map_ptr = रिक्त;
#यदि_अन्तः
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
स्थिर पूर्णाङ्क tdb_expand_file(संरचना tdb_context *tdb, tdb_off_t आकार, tdb_off_t addition)
{
	वर्ण buf[1024];

	यदि (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	यदि (ftruncate(tdb->fd, आकार+addition) == -1) {
		वर्ण b = 0;
		यदि (pwrite(tdb->fd,  &b, 1, (आकार+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 आकार+addition, strerror(errno)));
			प्रत्यागम -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	स्मृतिं_पूरय(buf, TDB_PAD_BYTE, परिमाण(buf));
	यावत् (addition) {
		पूर्णाङ्क n = addition>परिमाण(buf)?परिमाण(buf):addition;
		पूर्णाङ्क ret = pwrite(tdb->fd, buf, n, आकार);
		यदि (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			प्रत्यागम -1;
		}
		addition -= n;
		आकार += n;
	}
	प्रत्यागम 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
पूर्णाङ्क tdb_expand(संरचना tdb_context *tdb, tdb_off_t आकार)
{
	संरचना list_struct rec;
	tdb_off_t विस्थापनम्;

	यदि (tdb_तालकम्(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		प्रत्यागम -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	आकार = TDB_ALIGN(tdb->map_आकार + आकार*10, tdb->page_आकार) - tdb->map_आकार;

	यदि (!(tdb->flags & TDB_आन्तरिकम्))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	यदि (!(tdb->flags & TDB_आन्तरिकम्)) {
		यदि (tdb->methods->tdb_expand_file(tdb, tdb->map_आकार, आकार) != 0)
			गच्छ fail;
	}

	tdb->map_आकार += आकार;

	यदि (tdb->flags & TDB_आन्तरिकम्) {
		वर्ण *new_map_ptr = (वर्ण *)realloc(tdb->map_ptr,
						    tdb->map_आकार);
		यदि (!new_map_ptr) {
			tdb->map_आकार -= आकार;
			गच्छ fail;
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
	स्मृतिं_पूरय(&rec,'\0',परिमाण(rec));
	rec.rec_len = आकार - परिमाण(rec);

	/* link it into the free list */
	विस्थापनम् = tdb->map_आकार - आकार;
	यदि (tdb_free(tdb, विस्थापनम्, &rec) == -1)
		गच्छ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम -1;
}

/* read/write a tdb_off_t */
पूर्णाङ्क tdb_ofs_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_off_t *d)
{
	प्रत्यागम tdb->methods->tdb_read(tdb, विस्थापनम्, (वर्ण*)d, परिमाण(*d), DOCONV());
}

पूर्णाङ्क tdb_ofs_write(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_off_t *d)
{
	tdb_off_t off = *d;
	प्रत्यागम tdb->methods->tdb_write(tdb, विस्थापनम्, CONVERT(off), परिमाण(*d));
}


/* read a lump of data, allocating the space for it */
अचिह्नित वर्ण *tdb_alloc_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_len_t len)
{
	अचिह्नित वर्ण *buf;

	/* some systems don't like zero length malloc */
	यदि (len == 0) {
		len = 1;
	}

	यदि (!(buf = (अचिह्नित वर्ण *)स्मृतिं_देहि(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	यदि (tdb->methods->tdb_read(tdb, विस्थापनम्, buf, len, 0) == -1) {
		SAFE_FREE(buf);
		प्रत्यागम रिक्त;
	}
	प्रत्यागम buf;
}

/* Give a piece of tdb data to a parser */

पूर्णाङ्क tdb_parse_दत्त(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका,
		   tdb_off_t विस्थापनम्, tdb_len_t len,
		   पूर्णाङ्क (*parser)(TDB_दत्त कुञ्चिका, TDB_दत्त दत्त,
				 शून्य *वैयक्तिकम्_दत्त),
		   शून्य *वैयक्तिकम्_दत्त)
{
	TDB_दत्त दत्त;
	पूर्णाङ्क परिणाम;

	दत्त.dsize = len;

	यदि ((tdb->transaction == रिक्त) && (tdb->map_ptr != रिक्त)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		यदि (tdb->methods->tdb_oob(tdb, विस्थापनम्+len, 0) != 0) {
			प्रत्यागम -1;
		}
		दत्त.dptr = विस्थापनम् + (अचिह्नित वर्ण *)tdb->map_ptr;
		प्रत्यागम parser(कुञ्चिका, दत्त, वैयक्तिकम्_दत्त);
	}

	यदि (!(दत्त.dptr = tdb_alloc_read(tdb, विस्थापनम्, len))) {
		प्रत्यागम -1;
	}

	परिणाम = parser(कुञ्चिका, दत्त, वैयक्तिकम्_दत्त);
	मुक्त(दत्त.dptr);
	प्रत्यागम परिणाम;
}

/* read/write a record */
पूर्णाङ्क tdb_rec_read(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, संरचना list_struct *rec)
{
	यदि (tdb->methods->tdb_read(tdb, विस्थापनम्, rec, परिमाण(*rec),DOCONV()) == -1)
		प्रत्यागम -1;
	यदि (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, विस्थापनम्));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	प्रत्यागम tdb->methods->tdb_oob(tdb, rec->अग्र+परिमाण(*rec), 0);
}

पूर्णाङ्क tdb_rec_write(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, संरचना list_struct *rec)
{
	संरचना list_struct r = *rec;
	प्रत्यागम tdb->methods->tdb_write(tdb, विस्थापनम्, CONVERT(r), परिमाण(r));
}

स्थिर ध्रुव संरचना tdb_methods io_methods = {
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
	संरचना tdb_transaction_el *अग्र, *पूर्व;
	tdb_off_t विस्थापनम्;
	tdb_len_t दीर्घता;
	अचिह्नित वर्ण *दत्त;
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
	संरचना tdb_transaction_el *elements, *elements_चरमः;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	पूर्णाङ्क transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	पूर्णाङ्क nesting;

	/* old file size before transaction */
	tdb_len_t old_map_आकार;
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
	प्रति (el=tdb->transaction->elements_चरमः;el;el=el->पूर्व) {
		tdb_len_t partial;

		यदि (off+len <= el->विस्थापनम्) {
			अनुवर्त;
		}
		यदि (off >= el->विस्थापनम् + el->दीर्घता) {
			अनुवर्त;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		यदि (off < el->विस्थापनम्) {
			partial = el->विस्थापनम् - off;
			यदि (transaction_read(tdb, off, buf, partial, cv) != 0) {
				गच्छ fail;
			}
			len -= partial;
			off += partial;
			buf = (शून्य *)(partial + (वर्ण *)buf);
		}
		यदि (off + len <= el->विस्थापनम् + el->दीर्घता) {
			partial = len;
		} अन्यथा {
			partial = el->विस्थापनम् + el->दीर्घता - off;
		}
		स्मृतिं_प्रतिलिख(buf, el->दत्त + (off - el->विस्थापनम्), partial);
		यदि (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (शून्य *)(partial + (वर्ण *)buf);

		यदि (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
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

	यदि (len == 0) {
		प्रत्यागम 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	यदि (len == परिमाण(tdb_off_t) && off >= FREELIST_शिखरम् &&
	    off < FREELIST_शिखरम्+TDB_HASHTABLE_आकार(tdb)) {
		u32 chain = (off-FREELIST_शिखरम्) / परिमाण(tdb_off_t);
		स्मृतिं_प्रतिलिख(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	प्रति (el=tdb->transaction->elements_चरमः;el;el=el->पूर्व) {
		tdb_len_t partial;

		यदि (best_el == रिक्त && off == el->विस्थापनम्+el->दीर्घता) {
			best_el = el;
		}

		यदि (off+len <= el->विस्थापनम्) {
			अनुवर्त;
		}
		यदि (off >= el->विस्थापनम् + el->दीर्घता) {
			अनुवर्त;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		यदि (off < el->विस्थापनम्) {
			partial = el->विस्थापनम् - off;
			यदि (transaction_write(tdb, off, buf, partial) != 0) {
				गच्छ fail;
			}
			len -= partial;
			off += partial;
			buf = (ध्रुव शून्य *)(partial + (ध्रुव वर्ण *)buf);
		}
		यदि (off + len <= el->विस्थापनम् + el->दीर्घता) {
			partial = len;
		} अन्यथा {
			partial = el->विस्थापनम् + el->दीर्घता - off;
		}
		स्मृतिं_प्रतिलिख(el->दत्त + (off - el->विस्थापनम्), buf, partial);
		len -= partial;
		off += partial;
		buf = (ध्रुव शून्य *)(partial + (ध्रुव वर्ण *)buf);

		यदि (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			गच्छ fail;
		}

		प्रत्यागम 0;
	}

	/* see if we can append the new entry to an existing entry */
	यदि (best_el && best_el->विस्थापनम् + best_el->दीर्घता == off &&
	    (off+len < tdb->transaction->old_map_आकार ||
	     off > tdb->transaction->old_map_आकार)) {
		अचिह्नित वर्ण *दत्त = best_el->दत्त;
		el = best_el;
		el->दत्त = (अचिह्नित वर्ण *)realloc(el->दत्त,
						    el->दीर्घता + len);
		यदि (el->दत्त == रिक्त) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->दत्त = दत्त;
			प्रत्यागम -1;
		}
		यदि (buf) {
			स्मृतिं_प्रतिलिख(el->दत्त + el->दीर्घता, buf, len);
		} अन्यथा {
			स्मृतिं_पूरय(el->दत्त + el->दीर्घता, TDB_PAD_BYTE, len);
		}
		el->दीर्घता += len;
		प्रत्यागम 0;
	}

	/* add a new entry at the end of the list */
	el = (संरचना tdb_transaction_el *)स्मृतिं_देहि(परिमाण(*el));
	यदि (el == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		प्रत्यागम -1;
	}
	el->अग्र = रिक्त;
	el->पूर्व = tdb->transaction->elements_चरमः;
	el->विस्थापनम् = off;
	el->दीर्घता = len;
	el->दत्त = (अचिह्नित वर्ण *)स्मृतिं_देहि(len);
	यदि (el->दत्त == रिक्त) {
		मुक्त(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		प्रत्यागम -1;
	}
	यदि (buf) {
		स्मृतिं_प्रतिलिख(el->दत्त, buf, len);
	} अन्यथा {
		स्मृतिं_पूरय(el->दत्त, TDB_PAD_BYTE, len);
	}
	यदि (el->पूर्व) {
		el->पूर्व->अग्र = el;
	} अन्यथा {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_चरमः = el;
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
स्थिर शून्य transaction_अग्र_hash_chain(संरचना tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	प्रति (;h < tdb->शीर्षकम्.hash_आकार;h++) {
		/* the +1 takes account of the freelist */
		यदि (0 != tdb->transaction->hash_heads[h+1]) {
			भङ्ग;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
स्थिर पूर्णाङ्क transaction_oob(संरचना tdb_context *tdb, tdb_off_t len,
			   पूर्णाङ्क probe EXT2FS_ATTR((अप्रयुक्तम्)))
{
	यदि (len <= tdb->map_आकार) {
		प्रत्यागम 0;
	}
	प्रत्यागम TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
स्थिर पूर्णाङ्क transaction_expand_file(संरचना tdb_context *tdb, tdb_off_t आकार,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	यदि (transaction_write(tdb, आकार, रिक्त, addition) != 0) {
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

/*
  brlock during a transaction - ignore them
*/
स्थिर पूर्णाङ्क transaction_brlock(संरचना tdb_context *tdb EXT2FS_ATTR((अप्रयुक्तम्)),
			      tdb_off_t विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			      पूर्णाङ्क rw_type EXT2FS_ATTR((अप्रयुक्तम्)),
			      पूर्णाङ्क lck_type EXT2FS_ATTR((अप्रयुक्तम्)),
			      पूर्णाङ्क probe EXT2FS_ATTR((अप्रयुक्तम्)),
			      size_t len EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम 0;
}

स्थिर ध्रुव संरचना tdb_methods transaction_methods = {
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
पूर्णाङ्क tdb_transaction_start(संरचना tdb_context *tdb)
{
	/* some sanity checks */
	यदि (tdb->read_only || (tdb->flags & TDB_आन्तरिकम्) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		प्रत्यागम -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	यदि (tdb->transaction != रिक्त) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		प्रत्यागम 0;
	}

	यदि (tdb->अङ्कः_locks != 0 || tdb->global_तालकम्.गणनम्) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_तालकम्;
		प्रत्यागम -1;
	}

	यदि (tdb->travlocks.अग्र != रिक्त) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_तालकम्;
		प्रत्यागम -1;
	}

	tdb->transaction = (संरचना tdb_transaction *)
		शून्यस्मृतिं_देहि(परिमाण(संरचना tdb_transaction), 1);
	यदि (tdb->transaction == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		प्रत्यागम -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	यदि (tdb_transaction_तालकम्(tdb, F_WRLCK) == -1) {
		SAFE_FREE(tdb->transaction);
		प्रत्यागम -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	यदि (tdb_brlock(tdb, FREELIST_शिखरम्, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_तालकम्;
		गच्छ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		शून्यस्मृतिं_देहि(tdb->शीर्षकम्.hash_आकार+1, परिमाण(u32));
	यदि (tdb->transaction->hash_heads == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		गच्छ fail;
	}
	यदि (tdb->methods->tdb_read(tdb, FREELIST_शिखरम्, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_आकार(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		गच्छ fail;
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
	यदि (transaction_write(tdb, FREELIST_शिखरम्, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_आकार(tdb)) != 0) {
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
	यदि (tdb->transaction == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		प्रत्यागम -1;
	}

	यदि (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		प्रत्यागम 0;
	}

	tdb->map_आकार = tdb->transaction->old_map_आकार;

	/* free all the transaction elements */
	यावत् (tdb->transaction->elements) {
		संरचना tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->अग्र;
		मुक्त(el->दत्त);
		मुक्त(el);
	}

	/* remove any global lock created during the transaction */
	यदि (tdb->global_तालकम्.गणनम् != 0) {
		tdb_brlock(tdb, FREELIST_शिखरम्, F_UNLCK, F_SETLKW, 0, 4*tdb->शीर्षकम्.hash_आकार);
		tdb->global_तालकम्.गणनम् = 0;
	}

	/* remove any locks created during the transaction */
	यदि (tdb->अङ्कः_locks != 0) {
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
स्थिर पूर्णाङ्क transaction_sync(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, tdb_len_t दीर्घता)
{
	यदि (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		प्रत्यागम -1;
	}
#यदि defined(HAVE_MSYNC) && defined(MS_SYNC)
	यदि (tdb->map_ptr) {
		tdb_off_t moffset = विस्थापनम् & ~(tdb->page_आकार-1);
		यदि (msync(moffset + (वर्ण *)tdb->map_ptr,
			  दीर्घता + (विस्थापनम् - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			प्रत्यागम -1;
		}
	}
#यदि_अन्तः
	प्रत्यागम 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
स्थिर tdb_len_t tdb_recovery_आकार(संरचना tdb_context *tdb)
{
	संरचना tdb_transaction_el *el;
	tdb_len_t recovery_आकार = 0;

	recovery_आकार = परिमाण(u32);
	प्रति (el=tdb->transaction->elements;el;el=el->अग्र) {
		यदि (el->विस्थापनम् >= tdb->transaction->old_map_आकार) {
			अनुवर्त;
		}
		recovery_आकार += 2*परिमाण(tdb_off_t) + el->दीर्घता;
	}

	प्रत्यागम recovery_आकार;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
स्थिर पूर्णाङ्क tdb_recovery_allocate(संरचना tdb_context *tdb,
				 tdb_len_t *recovery_आकार,
				 tdb_off_t *recovery_विस्थापनम्,
				 tdb_len_t *recovery_max_आकार)
{
	संरचना list_struct rec;
	ध्रुव संरचना tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_शीर्ष;

	यदि (tdb_ofs_read(tdb, TDB_RECOVERY_शीर्ष, &recovery_शीर्ष) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		प्रत्यागम -1;
	}

	rec.rec_len = 0;

	यदि (recovery_शीर्ष != 0 &&
	    methods->tdb_read(tdb, recovery_शीर्ष, &rec, परिमाण(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		प्रत्यागम -1;
	}

	*recovery_आकार = tdb_recovery_आकार(tdb);

	यदि (recovery_शीर्ष != 0 && *recovery_आकार <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_आकार = rec.rec_len;
		*recovery_विस्थापनम् = recovery_शीर्ष;
		प्रत्यागम 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	यदि (recovery_शीर्ष != 0) {
		यदि (tdb_free(tdb, recovery_शीर्ष, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			प्रत्यागम -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_आकार = tdb_recovery_आकार(tdb);

	/* round up to a multiple of page size */
	*recovery_max_आकार = TDB_ALIGN(परिमाण(rec) + *recovery_आकार, tdb->page_आकार) - परिमाण(rec);
	*recovery_विस्थापनम् = tdb->map_आकार;
	recovery_शीर्ष = *recovery_विस्थापनम्;

	यदि (methods->tdb_expand_file(tdb, tdb->transaction->old_map_आकार,
				     (tdb->map_आकार - tdb->transaction->old_map_आकार) +
				     परिमाण(rec) + *recovery_max_आकार) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		प्रत्यागम -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_आकार = tdb->map_आकार;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_शीर्ष);
	यदि (methods->tdb_write(tdb, TDB_RECOVERY_शीर्ष,
			       &recovery_शीर्ष, परिमाण(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
स्थिर पूर्णाङ्क transaction_सज्जता_recovery(संरचना tdb_context *tdb,
				      tdb_off_t *magic_विस्थापनम्)
{
	संरचना tdb_transaction_el *el;
	tdb_len_t recovery_आकार;
	अचिह्नित वर्ण *दत्त, *p;
	ध्रुव संरचना tdb_methods *methods = tdb->transaction->io_methods;
	संरचना list_struct *rec;
	tdb_off_t recovery_विस्थापनम्, recovery_max_आकार;
	tdb_off_t old_map_आकार = tdb->transaction->old_map_आकार;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	यदि (tdb_recovery_allocate(tdb, &recovery_आकार,
				  &recovery_विस्थापनम्, &recovery_max_आकार) == -1) {
		प्रत्यागम -1;
	}

	दत्त = (अचिह्नित वर्ण *)स्मृतिं_देहि(recovery_आकार + परिमाण(*rec));
	यदि (दत्त == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		प्रत्यागम -1;
	}

	rec = (संरचना list_struct *)दत्त;
	स्मृतिं_पूरय(rec, 0, परिमाण(*rec));

	rec->magic    = 0;
	rec->दत्त_len = recovery_आकार;
	rec->rec_len  = recovery_max_आकार;
	rec->कुञ्चिका_len  = old_map_आकार;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = दत्त + परिमाण(*rec);
	प्रति (el=tdb->transaction->elements;el;el=el->अग्र) {
		यदि (el->विस्थापनम् >= old_map_आकार) {
			अनुवर्त;
		}
		यदि (el->विस्थापनम् + el->दीर्घता > tdb->transaction->old_map_आकार) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			मुक्त(दत्त);
			tdb->ecode = TDB_ERR_CORRUPT;
			प्रत्यागम -1;
		}
		स्मृतिं_प्रतिलिख(p, &el->विस्थापनम्, 4);
		स्मृतिं_प्रतिलिख(p+4, &el->दीर्घता, 4);
		यदि (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		यदि (methods->tdb_read(tdb, el->विस्थापनम्, p + 8, el->दीर्घता, 0) != 0) {
			मुक्त(दत्त);
			tdb->ecode = TDB_ERR_IO;
			प्रत्यागम -1;
		}
		p += 8 + el->दीर्घता;
	}

	/* and the tailer */
	tailer = परिमाण(*rec) + recovery_max_आकार;
	स्मृतिं_प्रतिलिख(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	यदि (methods->tdb_write(tdb, recovery_विस्थापनम्, दत्त, परिमाण(*rec) + recovery_आकार) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		मुक्त(दत्त);
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	यदि (transaction_sync(tdb, recovery_विस्थापनम्, परिमाण(*rec) + recovery_आकार) == -1) {
		मुक्त(दत्त);
		प्रत्यागम -1;
	}

	मुक्त(दत्त);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_विस्थापनम् = recovery_विस्थापनम् + offsetof(संरचना list_struct, magic);

	यदि (methods->tdb_write(tdb, *magic_विस्थापनम्, &magic, परिमाण(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* ensure the recovery magic marker is on disk */
	यदि (transaction_sync(tdb, *magic_विस्थापनम्, परिमाण(magic)) == -1) {
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
	tdb_off_t magic_विस्थापनम् = 0;
	u32 zero = 0;

	यदि (tdb->transaction == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		प्रत्यागम -1;
	}

	यदि (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		प्रत्यागम -1;
	}

	यदि (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		प्रत्यागम 0;
	}

	/* check for a null transaction */
	यदि (tdb->transaction->elements == रिक्त) {
		tdb_transaction_cancel(tdb);
		प्रत्यागम 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	यदि (tdb->अङ्कः_locks || tdb->global_तालकम्.गणनम्) {
		tdb->ecode = TDB_ERR_तालकम्;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		प्रत्यागम -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	यदि (tdb_brlock_upgrade(tdb, FREELIST_शिखरम्, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_तालकम्;
		tdb_transaction_cancel(tdb);
		प्रत्यागम -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	यदि (tdb_brlock(tdb, GLOBAL_तालकम्, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_तालकम्;
		tdb_transaction_cancel(tdb);
		प्रत्यागम -1;
	}

	यदि (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		यदि (transaction_सज्जता_recovery(tdb, &magic_विस्थापनम्) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_तालकम्, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			प्रत्यागम -1;
		}
	}

	/* expand the file to the new size if needed */
	यदि (tdb->map_आकार != tdb->transaction->old_map_आकार) {
		यदि (methods->tdb_expand_file(tdb, tdb->transaction->old_map_आकार,
					     tdb->map_आकार -
					     tdb->transaction->old_map_आकार) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_तालकम्, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			प्रत्यागम -1;
		}
		tdb->map_आकार = tdb->transaction->old_map_आकार;
		methods->tdb_oob(tdb, tdb->map_आकार + 1, 1);
	}

	/* perform all the writes */
	यावत् (tdb->transaction->elements) {
		संरचना tdb_transaction_el *el = tdb->transaction->elements;

		यदि (methods->tdb_write(tdb, el->विस्थापनम्, el->दत्त, el->दीर्घता) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_तालकम्, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			प्रत्यागम -1;
		}
		tdb->transaction->elements = el->अग्र;
		मुक्त(el->दत्त);
		मुक्त(el);
	}

	यदि (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		यदि (transaction_sync(tdb, 0, tdb->map_आकार) == -1) {
			प्रत्यागम -1;
		}

		/* remove the recovery marker */
		यदि (methods->tdb_write(tdb, magic_विस्थापनम्, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			प्रत्यागम -1;
		}

		/* ensure the recovery marker has been removed on disk */
		यदि (transaction_sync(tdb, magic_विस्थापनम्, 4) == -1) {
			प्रत्यागम -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_तालकम्, F_UNLCK, F_SETLKW, 0, 1);

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
#यदि_परिभाषितम् HAVE_UTIME
	utime(tdb->name, रिक्त);
#यदि_अन्तः

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
	tdb_off_t recovery_शीर्ष, recovery_eof;
	अचिह्नित वर्ण *दत्त, *p;
	u32 zero = 0;
	संरचना list_struct rec;

	/* find the recovery area */
	यदि (tdb_ofs_read(tdb, TDB_RECOVERY_शीर्ष, &recovery_शीर्ष) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	यदि (recovery_शीर्ष == 0) {
		/* we have never allocated a recovery record */
		प्रत्यागम 0;
	}

	/* read the recovery record */
	यदि (tdb->methods->tdb_read(tdb, recovery_शीर्ष, &rec,
				   परिमाण(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	यदि (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		प्रत्यागम 0;
	}

	यदि (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		प्रत्यागम -1;
	}

	recovery_eof = rec.कुञ्चिका_len;

	दत्त = (अचिह्नित वर्ण *)स्मृतिं_देहि(rec.दत्त_len);
	यदि (दत्त == रिक्त) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		प्रत्यागम -1;
	}

	/* read the full recovery data */
	यदि (tdb->methods->tdb_read(tdb, recovery_शीर्ष + परिमाण(rec), दत्त,
				   rec.दत्त_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		मुक्त(दत्त);
		प्रत्यागम -1;
	}

	/* recover the file data */
	p = दत्त;
	यावत् (p+8 < दत्त + rec.दत्त_len) {
		u32 ofs, len;
		यदि (DOCONV()) {
			tdb_convert(p, 8);
		}
		स्मृतिं_प्रतिलिख(&ofs, p, 4);
		स्मृतिं_प्रतिलिख(&len, p+4, 4);

		यदि (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			मुक्त(दत्त);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			प्रत्यागम -1;
		}
		p += 8 + len;
	}

	मुक्त(दत्त);

	यदि (transaction_sync(tdb, 0, tdb->map_आकार) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	यदि (recovery_eof <= recovery_शीर्ष) {
		यदि (tdb_ofs_write(tdb, TDB_RECOVERY_शीर्ष, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			प्रत्यागम -1;
		}
	}

	/* remove the recovery magic */
	यदि (tdb_ofs_write(tdb, recovery_शीर्ष + offsetof(संरचना list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	यदि (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		प्रत्यागम -1;
	}
	tdb->map_आकार = recovery_eof;
	tdb_mmap(tdb);

	यदि (transaction_sync(tdb, 0, recovery_eof) == -1) {
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
	यदि (tdb->methods->tdb_read(tdb, off, rec, परिमाण(*rec),DOCONV()) == -1)
		प्रत्यागम -1;

	यदि (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		यदि (tdb->methods->tdb_write(tdb, off, rec, परिमाण(*rec)) == -1)
			प्रत्यागम -1;
	}

	यदि (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		प्रत्यागम TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	यदि (tdb->methods->tdb_oob(tdb, rec->अग्र+परिमाण(*rec), 0) != 0)
		प्रत्यागम -1;
	प्रत्यागम 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
स्थिर पूर्णाङ्क remove_from_freelist(संरचना tdb_context *tdb, tdb_off_t off, tdb_off_t अग्र)
{
	tdb_off_t चरमः_ptr, i;

	/* read in the freelist top */
	चरमः_ptr = FREELIST_शिखरम्;
	यावत् (tdb_ofs_read(tdb, चरमः_ptr, &i) != -1 && i != 0) {
		यदि (i == off) {
			/* We've found it! */
			प्रत्यागम tdb_ofs_write(tdb, चरमः_ptr, &अग्र);
		}
		/* Follow chain (next offset is at start of record) */
		चरमः_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	प्रत्यागम TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
स्थिर पूर्णाङ्क update_tailer(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्,
			 ध्रुव संरचना list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = परिमाण(*rec) + rec->rec_len;
	प्रत्यागम tdb_ofs_write(tdb, विस्थापनम् + totalsize - परिमाण(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
पूर्णाङ्क tdb_free(संरचना tdb_context *tdb, tdb_off_t विस्थापनम्, संरचना list_struct *rec)
{
	tdb_off_t दक्षिणः, वामः;

	/* Allocation and tailer lock */
	यदि (tdb_तालकम्(tdb, -1, F_WRLCK) != 0)
		प्रत्यागम -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	यदि (update_tailer(tdb, विस्थापनम्, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		गच्छ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	दक्षिणः = विस्थापनम् + परिमाण(*rec) + rec->rec_len;
	यदि (दक्षिणः + परिमाण(*rec) <= tdb->map_आकार) {
		संरचना list_struct r;

		यदि (tdb->methods->tdb_read(tdb, दक्षिणः, &r, परिमाण(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", दक्षिणः));
			गच्छ वामः;
		}

		/* If it's free, expand to include it. */
		यदि (r.magic == TDB_FREE_MAGIC) {
			यदि (remove_from_freelist(tdb, दक्षिणः, r.अग्र) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", दक्षिणः));
				गच्छ वामः;
			}
			rec->rec_len += परिमाण(r) + r.rec_len;
		}
	}

वामः:
	/* Look left */
	वामः = विस्थापनम् - परिमाण(tdb_off_t);
	यदि (वामः > TDB_दत्त_START(tdb->शीर्षकम्.hash_आकार)) {
		संरचना list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		यदि (tdb_ofs_read(tdb, वामः, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", वामः));
			गच्छ update;
		}

		/* it could be uninitialised data */
		यदि (leftsize == 0 || leftsize == TDB_PAD_U32) {
			गच्छ update;
		}

		वामः = विस्थापनम् - leftsize;

		/* Now read in record */
		यदि (tdb->methods->tdb_read(tdb, वामः, &l, परिमाण(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", वामः, leftsize));
			गच्छ update;
		}

		/* If it's free, expand to include it. */
		यदि (l.magic == TDB_FREE_MAGIC) {
			यदि (remove_from_freelist(tdb, वामः, l.अग्र) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", वामः));
				गच्छ update;
			} अन्यथा {
				विस्थापनम् = वामः;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	यदि (update_tailer(tdb, विस्थापनम्, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", विस्थापनम्));
		गच्छ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	यदि (tdb_ofs_read(tdb, FREELIST_शिखरम्, &rec->अग्र) == -1 ||
	    tdb_rec_write(tdb, विस्थापनम्, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_शिखरम्, &विस्थापनम्) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", विस्थापनम्));
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
स्थिर tdb_off_t tdb_allocate_ofs(संरचना tdb_context *tdb, tdb_len_t दीर्घता, tdb_off_t rec_ptr,
				संरचना list_struct *rec, tdb_off_t चरमः_ptr)
{
	संरचना list_struct newrec;
	tdb_off_t newrec_ptr;

	स्मृतिं_पूरय(&newrec, '\0', परिमाण(newrec));

	/* found it - now possibly split it up  */
	यदि (rec->rec_len > दीर्घता + MIN_REC_आकार) {
		/* Length of left piece */
		दीर्घता = TDB_ALIGN(दीर्घता, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (परिमाण(*rec) + दीर्घता);
		newrec_ptr = rec_ptr + परिमाण(*rec) + दीर्घता;

		/* And left record is shortened */
		rec->rec_len = दीर्घता;
	} अन्यथा {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	यदि (tdb_ofs_write(tdb, चरमः_ptr, &rec->अग्र) == -1) {
		प्रत्यागम 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	यदि (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		प्रत्यागम 0;
	}

	/* Did we create new block? */
	यदि (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		यदि (update_tailer(tdb, rec_ptr, rec) == -1) {
			प्रत्यागम 0;
		}

		/* Free new record */
		यदि (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
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
tdb_off_t tdb_allocate(संरचना tdb_context *tdb, tdb_len_t दीर्घता, संरचना list_struct *rec)
{
	tdb_off_t rec_ptr, चरमः_ptr, newrec_ptr;
	संरचना {
		tdb_off_t rec_ptr, चरमः_ptr;
		tdb_len_t rec_len;
	} bestfit;

	यदि (tdb_तालकम्(tdb, -1, F_WRLCK) == -1)
		प्रत्यागम 0;

	/* Extra bytes required for tailer */
	दीर्घता += परिमाण(tdb_off_t);

 again:
	चरमः_ptr = FREELIST_शिखरम्;

	/* read in the freelist top */
	यदि (tdb_ofs_read(tdb, FREELIST_शिखरम्, &rec_ptr) == -1)
		गच्छ fail;

	bestfit.rec_ptr = 0;
	bestfit.चरमः_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	यावत् (rec_ptr) {
		यदि (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			गच्छ fail;
		}

		यदि (rec->rec_len >= दीर्घता) {
			यदि (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.चरमः_ptr = चरमः_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				यदि (bestfit.rec_len < 2*दीर्घता) {
					भङ्ग;
				}
			}
		}

		/* move to the next record */
		चरमः_ptr = rec_ptr;
		rec_ptr = rec->अग्र;
	}

	यदि (bestfit.rec_ptr != 0) {
		यदि (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			गच्छ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, दीर्घता, bestfit.rec_ptr, rec, bestfit.चरमः_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		प्रत्यागम newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	यदि (tdb_expand(tdb, दीर्घता + परिमाण(*rec)) == 0)
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

स्थिर पूर्णाङ्क आलोकितम्_निवेशय(संरचना tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_दत्त कुञ्चिका, दत्त;

	स्मृतिं_पूरय(&दत्त, '\0', परिमाण(दत्त));
	कुञ्चिका.dptr = (अचिह्नित वर्ण *)&rec_ptr;
	कुञ्चिका.dsize = परिमाण(rec_ptr);
	प्रत्यागम tdb_store(mem_tdb, कुञ्चिका, दत्त, TDB_निवेशय);
}

पूर्णाङ्क tdb_validate_freelist(संरचना tdb_context *tdb, पूर्णाङ्क *pnum_entries)
{
	संरचना tdb_context *mem_tdb = रिक्त;
	संरचना list_struct rec;
	tdb_off_t rec_ptr, चरमः_ptr;
	पूर्णाङ्क ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->शीर्षकम्.hash_आकार,
				TDB_आन्तरिकम्, O_RDWR, 0600);
	यदि (!mem_tdb) {
		प्रत्यागम -1;
	}

	यदि (tdb_तालकम्(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		प्रत्यागम 0;
	}

	चरमः_ptr = FREELIST_शिखरम्;

	/* Store the FREELIST_TOP record. */
	यदि (आलोकितम्_निवेशय(mem_tdb, चरमः_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		गच्छ fail;
	}

	/* read in the freelist top */
	यदि (tdb_ofs_read(tdb, FREELIST_शिखरम्, &rec_ptr) == -1) {
		गच्छ fail;
	}

	यावत् (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		यदि (आलोकितम्_निवेशय(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			गच्छ fail;
		}

		यदि (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			गच्छ fail;
		}

		/* move to the next record */
		चरमः_ptr = rec_ptr;
		rec_ptr = rec.अग्र;
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
स्थिर पूर्णाङ्क tdb_अग्र_तालकम्(संरचना tdb_context *tdb, संरचना tdb_traverse_तालकम् *tlock,
			 संरचना list_struct *rec)
{
	पूर्णाङ्क want_अग्र = (tlock->off != 0);

	/* Lock each chain from the start one. */
	प्रति (; tlock->hash < tdb->शीर्षकम्.hash_आकार; tlock->hash++) {
		यदि (!tlock->off && tlock->hash != 0) {
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
			यदि (tlock->hash == tdb->शीर्षकम्.hash_आकार) {
				अनुवर्त;
			}
		}

		यदि (tdb_तालकम्(tdb, tlock->hash, tlock->तालकम्_rw) == -1)
			प्रत्यागम -1;

		/* No previous record?  Start at top of chain. */
		यदि (!tlock->off) {
			यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(tlock->hash),
				     &tlock->off) == -1)
				गच्छ fail;
		} अन्यथा {
			/* Otherwise unlock the previous record. */
			यदि (tdb_unlock_लेखः(tdb, tlock->off) != 0)
				गच्छ fail;
		}

		यदि (want_अग्र) {
			/* We have offset of old record: grab next */
			यदि (tdb_rec_read(tdb, tlock->off, rec) == -1)
				गच्छ fail;
			tlock->off = rec->अग्र;
		}

		/* Iterate through chain */
		यावत्( tlock->off) {
			tdb_off_t current;
			यदि (tdb_rec_read(tdb, tlock->off, rec) == -1)
				गच्छ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			यदि (tlock->off == rec->अग्र) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				गच्छ fail;
			}

			यदि (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				यदि (tdb_तालकम्_लेखः(tdb, tlock->off) != 0)
					गच्छ fail;
				प्रत्यागम tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->अग्र;
			यदि (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				गच्छ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->तालकम्_rw);
		want_अग्र = 0;
	}
	/* We finished iteration without finding anything */
	प्रत्यागम TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	यदि (tdb_unlock(tdb, tlock->hash, tlock->तालकम्_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	प्रत्यागम -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
स्थिर पूर्णाङ्क tdb_traverse_आन्तरिकम्(संरचना tdb_context *tdb,
				 tdb_traverse_func fn, शून्य *वैयक्तिकम्_दत्त,
				 संरचना tdb_traverse_तालकम् *tl)
{
	TDB_दत्त कुञ्चिका, dbuf;
	संरचना list_struct rec;
	पूर्णाङ्क ret, गणनम् = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->अग्र = tdb->travlocks.अग्र;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.अग्र = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	यावत् ((ret = tdb_अग्र_तालकम्(tdb, tl, &rec)) > 0) {
		गणनम्++;
		/* now read the full record */
		कुञ्चिका.dptr = tdb_alloc_read(tdb, tl->off + परिमाण(rec),
					  rec.कुञ्चिका_len + rec.दत्त_len);
		यदि (!कुञ्चिका.dptr) {
			ret = -1;
			यदि (tdb_unlock(tdb, tl->hash, tl->तालकम्_rw) != 0)
				गच्छ बहिर्गमनम्;
			यदि (tdb_unlock_लेखः(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			गच्छ बहिर्गमनम्;
		}
		कुञ्चिका.dsize = rec.कुञ्चिका_len;
		dbuf.dptr = कुञ्चिका.dptr + rec.कुञ्चिका_len;
		dbuf.dsize = rec.दत्त_len;

		/* Drop chain lock, call out */
		यदि (tdb_unlock(tdb, tl->hash, tl->तालकम्_rw) != 0) {
			ret = -1;
			SAFE_FREE(कुञ्चिका.dptr);
			गच्छ बहिर्गमनम्;
		}
		यदि (fn && fn(tdb, कुञ्चिका, dbuf, वैयक्तिकम्_दत्त)) {
			/* They want us to terminate traversal */
			ret = गणनम्;
			यदि (tdb_unlock_लेखः(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			SAFE_FREE(कुञ्चिका.dptr);
			गच्छ बहिर्गमनम्;
		}
		SAFE_FREE(कुञ्चिका.dptr);
	}
बहिर्गमनम्:
	tdb->travlocks.अग्र = tl->अग्र;
	यदि (ret < 0)
		प्रत्यागम -1;
	अन्यथा
		प्रत्यागम गणनम्;
}


/*
  a write style traverse - temporarily marks the db read only
*/
पूर्णाङ्क tdb_traverse_read(संरचना tdb_context *tdb,
		      tdb_traverse_func fn, शून्य *वैयक्तिकम्_दत्त)
{
	संरचना tdb_traverse_तालकम् tl = { रिक्त, 0, 0, F_RDLCK };
	पूर्णाङ्क ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	यदि (tdb_transaction_तालकम्(tdb, F_RDLCK)) {
		प्रत्यागम -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_आन्तरिकम्(tdb, fn, वैयक्तिकम्_दत्त, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	प्रत्यागम ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
पूर्णाङ्क tdb_traverse(संरचना tdb_context *tdb,
		 tdb_traverse_func fn, शून्य *वैयक्तिकम्_दत्त)
{
	संरचना tdb_traverse_तालकम् tl = { रिक्त, 0, 0, F_WRLCK };
	पूर्णाङ्क ret;

	यदि (tdb->read_only || tdb->traverse_read) {
		प्रत्यागम tdb_traverse_read(tdb, fn, वैयक्तिकम्_दत्त);
	}

	यदि (tdb_transaction_तालकम्(tdb, F_WRLCK)) {
		प्रत्यागम -1;
	}

	ret = tdb_traverse_आन्तरिकम्(tdb, fn, वैयक्तिकम्_दत्त, &tl);

	tdb_transaction_unlock(tdb);

	प्रत्यागम ret;
}


/* find the first entry in the database and return its key */
TDB_दत्त tdb_firstkey(संरचना tdb_context *tdb)
{
	TDB_दत्त कुञ्चिका;
	संरचना list_struct rec;

	/* release any old lock */
	यदि (tdb_unlock_लेखः(tdb, tdb->travlocks.off) != 0)
		प्रत्यागम tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.तालकम्_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	यदि (tdb_अग्र_तालकम्(tdb, &tdb->travlocks, &rec) <= 0)
		प्रत्यागम tdb_null;
	/* now read the key */
	कुञ्चिका.dsize = rec.कुञ्चिका_len;
	कुञ्चिका.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+परिमाण(rec),कुञ्चिका.dsize);

	/* Unlock the hash chain of the record we just read. */
	यदि (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.तालकम्_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	प्रत्यागम कुञ्चिका;
}

/* find the next entry in the database, returning its key */
TDB_दत्त tdb_nextkey(संरचना tdb_context *tdb, TDB_दत्त oldkey)
{
	u32 oldhash;
	TDB_दत्त कुञ्चिका = tdb_null;
	संरचना list_struct rec;
	अचिह्नित वर्ण *k = रिक्त;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	यदि (tdb->travlocks.off) {
		यदि (tdb_तालकम्(tdb,tdb->travlocks.hash,tdb->travlocks.तालकम्_rw))
			प्रत्यागम tdb_null;
		यदि (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+परिमाण(rec),
					    rec.कुञ्चिका_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			यदि (tdb_unlock_लेखः(tdb, tdb->travlocks.off) != 0) {
				SAFE_FREE(k);
				प्रत्यागम tdb_null;
			}
			यदि (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.तालकम्_rw) != 0) {
				SAFE_FREE(k);
				प्रत्यागम tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		SAFE_FREE(k);
	}

	यदि (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_find_तालकम्_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.तालकम्_rw, &rec);
		यदि (!tdb->travlocks.off)
			प्रत्यागम tdb_null;
		tdb->travlocks.hash = BUCKET(rec.पूर्णम्_hash);
		यदि (tdb_तालकम्_लेखः(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			प्रत्यागम tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	यदि (tdb_अग्र_तालकम्(tdb, &tdb->travlocks, &rec) > 0) {
		कुञ्चिका.dsize = rec.कुञ्चिका_len;
		कुञ्चिका.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+परिमाण(rec),
					  कुञ्चिका.dsize);
		/* Unlock the chain of this new record */
		यदि (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.तालकम्_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	यदि (tdb_unlock(tdb, BUCKET(oldhash), tdb->travlocks.तालकम्_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	प्रत्यागम कुञ्चिका;
}

/* file: dump.c */

स्थिर tdb_off_t tdb_dump_लेखः(संरचना tdb_context *tdb, पूर्णाङ्क hash,
				 tdb_off_t विस्थापनम्)
{
	संरचना list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	यदि (tdb->methods->tdb_read(tdb, विस्थापनम्, (वर्ण *)&rec,
				   परिमाण(rec), DOCONV()) == -1) {
		प्रारूप_लिख("ERROR: failed to read record at %u\n", विस्थापनम्);
		प्रत्यागम 0;
	}

	प्रारूप_लिख(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, विस्थापनम्, rec.अग्र, rec.rec_len, rec.कुञ्चिका_len, rec.दत्त_len,
	       rec.पूर्णम्_hash, rec.magic);

	tailer_ofs = विस्थापनम् + परिमाण(rec) + rec.rec_len - परिमाण(tdb_off_t);

	यदि (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		प्रारूप_लिख("ERROR: failed to read tailer at %u\n", tailer_ofs);
		प्रत्यागम rec.अग्र;
	}

	यदि (tailer != rec.rec_len + परिमाण(rec)) {
		प्रारूप_लिख("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(अचिह्नित पूर्णाङ्क)tailer, (अचिह्नित पूर्णाङ्क)(rec.rec_len + परिमाण(rec)));
	}
	प्रत्यागम rec.अग्र;
}

स्थिर पूर्णाङ्क tdb_dump_chain(संरचना tdb_context *tdb, पूर्णाङ्क i)
{
	tdb_off_t rec_ptr, शिखरम्;

	शिखरम् = TDB_HASH_शिखरम्(i);

	यदि (tdb_तालकम्(tdb, i, F_WRLCK) != 0)
		प्रत्यागम -1;

	यदि (tdb_ofs_read(tdb, शिखरम्, &rec_ptr) == -1)
		प्रत्यागम tdb_unlock(tdb, i, F_WRLCK);

	यदि (rec_ptr)
		प्रारूप_लिख("hash=%d\n", i);

	यावत् (rec_ptr) {
		rec_ptr = tdb_dump_लेखः(tdb, i, rec_ptr);
	}

	प्रत्यागम tdb_unlock(tdb, i, F_WRLCK);
}

शून्य tdb_dump_all(संरचना tdb_context *tdb)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < (पूर्णाङ्क)tdb->शीर्षकम्.hash_आकार; i++) {
		tdb_dump_chain(tdb, i);
	}
	प्रारूप_लिख("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

पूर्णाङ्क tdb_printfreelist(संरचना tdb_context *tdb)
{
	पूर्णाङ्क ret;
	दीर्घ योग_free = 0;
	tdb_off_t विस्थापनम्, rec_ptr;
	संरचना list_struct rec;

	यदि ((ret = tdb_तालकम्(tdb, -1, F_WRLCK)) != 0)
		प्रत्यागम ret;

	विस्थापनम् = FREELIST_शिखरम्;

	/* read in the freelist top */
	यदि (tdb_ofs_read(tdb, विस्थापनम्, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		प्रत्यागम 0;
	}

	प्रारूप_लिख("freelist top=[0x%08x]\n", rec_ptr );
	यावत् (rec_ptr) {
		यदि (tdb->methods->tdb_read(tdb, rec_ptr, (वर्ण *)&rec,
					   परिमाण(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			प्रत्यागम -1;
		}

		यदि (rec.magic != TDB_FREE_MAGIC) {
			प्रारूप_लिख("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			प्रत्यागम -1;
		}

		प्रारूप_लिख("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		योग_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.अग्र;
	}
	प्रारूप_लिख("total rec_len = [0x%08x (%d)]\n", (पूर्णाङ्क)योग_free,
               (पूर्णाङ्क)योग_free);

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

	यदि (!(tdb->flags & TDB_SEQNUM)) {
		प्रत्यागम;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	यदि (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
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
	यदि (!(tdb->flags & TDB_SEQNUM)) {
		प्रत्यागम;
	}

	यदि (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		प्रत्यागम;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

स्थिर पूर्णाङ्क tdb_कुञ्चिका_compare(TDB_दत्त कुञ्चिका, TDB_दत्त दत्त,
			   शून्य *वैयक्तिकम्_दत्त EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम memcmp(दत्त.dptr, कुञ्चिका.dptr, दत्त.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
स्थिर tdb_off_t tdb_find(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, u32 hash,
			संरचना list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		प्रत्यागम 0;

	/* keep looking until we find the right record */
	यावत् (rec_ptr) {
		यदि (tdb_rec_read(tdb, rec_ptr, r) == -1)
			प्रत्यागम 0;

		यदि (!TDB_DEAD(r) && hash==r->पूर्णम्_hash
		    && कुञ्चिका.dsize==r->कुञ्चिका_len
		    && tdb_parse_दत्त(tdb, कुञ्चिका, rec_ptr + परिमाण(*r),
				      r->कुञ्चिका_len, tdb_कुञ्चिका_compare,
				      रिक्त) == 0) {
			प्रत्यागम rec_ptr;
		}
		rec_ptr = r->अग्र;
	}
	प्रत्यागम TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_find_तालकम्_hash(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, u32 hash, पूर्णाङ्क locktype,
			   संरचना list_struct *rec)
{
	u32 rec_ptr;

	यदि (tdb_तालकम्(tdb, BUCKET(hash), locktype) == -1)
		प्रत्यागम 0;
	यदि (!(rec_ptr = tdb_find(tdb, कुञ्चिका, hash, rec)))
		tdb_unlock(tdb, BUCKET(hash), locktype);
	प्रत्यागम rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
स्थिर पूर्णाङ्क tdb_update_hash(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, u32 hash, TDB_दत्त dbuf)
{
	संरचना list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	यदि (!(rec_ptr = tdb_find(tdb, कुञ्चिका, hash, &rec)))
		प्रत्यागम -1;

	/* must be long enough key, data and tailer */
	यदि (rec.rec_len < कुञ्चिका.dsize + dbuf.dsize + परिमाण(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		प्रत्यागम -1;
	}

	यदि (tdb->methods->tdb_write(tdb, rec_ptr + परिमाण(rec) + rec.कुञ्चिका_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		प्रत्यागम -1;

	यदि (dbuf.dsize != rec.दत्त_len) {
		/* update size */
		rec.दत्त_len = dbuf.dsize;
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
TDB_दत्त tdb_fetch(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	TDB_दत्त ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);
	यदि (!(rec_ptr = tdb_find_तालकम्_hash(tdb,कुञ्चिका,hash,F_RDLCK,&rec)))
		प्रत्यागम tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + परिमाण(rec) + rec.कुञ्चिका_len,
				  rec.दत्त_len);
	ret.dsize = rec.दत्त_len;
	tdb_unlock(tdb, BUCKET(rec.पूर्णम्_hash), F_RDLCK);
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

पूर्णाङ्क tdb_parse_लेखः(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका,
		     पूर्णाङ्क (*parser)(TDB_दत्त कुञ्चिका, TDB_दत्त दत्त,
				   शून्य *वैयक्तिकम्_दत्त),
		     शून्य *वैयक्तिकम्_दत्त)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	पूर्णाङ्क ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);

	यदि (!(rec_ptr = tdb_find_तालकम्_hash(tdb,कुञ्चिका,hash,F_RDLCK,&rec))) {
		प्रत्यागम TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_दत्त(tdb, कुञ्चिका, rec_ptr + परिमाण(rec) + rec.कुञ्चिका_len,
			     rec.दत्त_len, parser, वैयक्तिकम्_दत्त);

	tdb_unlock(tdb, BUCKET(rec.पूर्णम्_hash), F_RDLCK);

	प्रत्यागम ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
स्थिर पूर्णाङ्क tdb_exists_hash(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, u32 hash)
{
	संरचना list_struct rec;

	यदि (tdb_find_तालकम्_hash(tdb, कुञ्चिका, hash, F_RDLCK, &rec) == 0)
		प्रत्यागम 0;
	tdb_unlock(tdb, BUCKET(rec.पूर्णम्_hash), F_RDLCK);
	प्रत्यागम 1;
}

पूर्णाङ्क tdb_exists(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	u32 hash = tdb->hash_fn(&कुञ्चिका);
	प्रत्यागम tdb_exists_hash(tdb, कुञ्चिका, hash);
}

/* actually delete an entry in the database given the offset */
पूर्णाङ्क tdb_do_delete(संरचना tdb_context *tdb, tdb_off_t rec_ptr, संरचना list_struct*rec)
{
	tdb_off_t चरमः_ptr, i;
	संरचना list_struct lastrec;

	यदि (tdb->read_only || tdb->traverse_read) प्रत्यागम -1;

	यदि (tdb_write_तालकम्_लेखः(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		प्रत्यागम tdb_rec_write(tdb, rec_ptr, rec);
	}
	यदि (tdb_write_unlock_लेखः(tdb, rec_ptr) != 0)
		प्रत्यागम -1;

	/* find previous record in hash chain */
	यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(rec->पूर्णम्_hash), &i) == -1)
		प्रत्यागम -1;
	प्रति (चरमः_ptr = 0; i != rec_ptr; चरमः_ptr = i, i = lastrec.अग्र)
		यदि (tdb_rec_read(tdb, i, &lastrec) == -1)
			प्रत्यागम -1;

	/* unlink it: next ptr is at start of record. */
	यदि (चरमः_ptr == 0)
		चरमः_ptr = TDB_HASH_शिखरम्(rec->पूर्णम्_hash);
	यदि (tdb_ofs_write(tdb, चरमः_ptr, &rec->अग्र) == -1)
		प्रत्यागम -1;

	/* recover the space */
	यदि (tdb_free(tdb, rec_ptr, rec) == -1)
		प्रत्यागम -1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क tdb_गणनम्_dead(संरचना tdb_context *tdb, u32 hash)
{
	पूर्णाङ्क res = 0;
	tdb_off_t rec_ptr;
	संरचना list_struct rec;

	/* read in the hash top */
	यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		प्रत्यागम 0;

	यावत् (rec_ptr) {
		यदि (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			प्रत्यागम 0;

		यदि (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.अग्र;
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

	यदि (tdb_तालकम्(tdb, -1, F_WRLCK) == -1) {
		प्रत्यागम -1;
	}

	/* read in the hash top */
	यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		गच्छ fail;

	यावत् (rec_ptr) {
		tdb_off_t अग्र;

		यदि (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			गच्छ fail;
		}

		अग्र = rec.अग्र;

		यदि (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			गच्छ fail;
		}
		rec_ptr = अग्र;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	प्रत्यागम res;
}

/* delete an entry in the database given a key */
स्थिर पूर्णाङ्क tdb_delete_hash(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, u32 hash)
{
	tdb_off_t rec_ptr;
	संरचना list_struct rec;
	पूर्णाङ्क ret;

	यदि (tdb->max_dead_लेखाः != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		यदि (tdb_तालकम्(tdb, BUCKET(hash), F_WRLCK) == -1)
			प्रत्यागम -1;

		यदि (tdb_गणनम्_dead(tdb, hash) >= tdb->max_dead_लेखाः) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		यदि (!(rec_ptr = tdb_find(tdb, कुञ्चिका, hash, &rec))) {
			tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
			प्रत्यागम -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	अन्यथा {
		यदि (!(rec_ptr = tdb_find_तालकम्_hash(tdb, कुञ्चिका, hash, F_WRLCK,
						   &rec)))
			प्रत्यागम -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	यदि (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	यदि (tdb_unlock(tdb, BUCKET(rec.पूर्णम्_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	प्रत्यागम ret;
}

पूर्णाङ्क tdb_delete(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका)
{
	u32 hash = tdb->hash_fn(&कुञ्चिका);
	प्रत्यागम tdb_delete_hash(tdb, कुञ्चिका, hash);
}

/*
 * See if we have a dead record around with enough space
 */
स्थिर tdb_off_t tdb_find_dead(संरचना tdb_context *tdb, u32 hash,
			       संरचना list_struct *r, tdb_len_t दीर्घता)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1)
		प्रत्यागम 0;

	/* keep looking until we find the right record */
	यावत् (rec_ptr) {
		यदि (tdb_rec_read(tdb, rec_ptr, r) == -1)
			प्रत्यागम 0;

		यदि (TDB_DEAD(r) && r->rec_len >= दीर्घता) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			प्रत्यागम rec_ptr;
		}
		rec_ptr = r->अग्र;
	}
	प्रत्यागम 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
पूर्णाङ्क tdb_store(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, पूर्णाङ्क flag)
{
	संरचना list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	वर्ण *p = रिक्त;
	पूर्णाङ्क ret = -1;

	यदि (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		प्रत्यागम -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);
	यदि (tdb_तालकम्(tdb, BUCKET(hash), F_WRLCK) == -1)
		प्रत्यागम -1;

	/* check for it existing, on insert. */
	यदि (flag == TDB_निवेशय) {
		यदि (tdb_exists_hash(tdb, कुञ्चिका, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			गच्छ fail;
		}
	} अन्यथा {
		/* first try in-place update, on modify or replace. */
		यदि (tdb_update_hash(tdb, कुञ्चिका, hash, dbuf) == 0) {
			गच्छ समाप्तम्;
		}
		यदि (tdb->ecode == TDB_ERR_NOEXIST &&
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
	यदि (flag != TDB_निवेशय)
		tdb_delete_hash(tdb, कुञ्चिका, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	यदि (!(p = (वर्ण *)स्मृतिं_देहि(कुञ्चिका.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		गच्छ fail;
	}

	स्मृतिं_प्रतिलिख(p, कुञ्चिका.dptr, कुञ्चिका.dsize);
	यदि (dbuf.dsize)
		स्मृतिं_प्रतिलिख(p+कुञ्चिका.dsize, dbuf.dptr, dbuf.dsize);

	यदि (tdb->max_dead_लेखाः != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_find_dead(
			tdb, hash, &rec,
			कुञ्चिका.dsize + dbuf.dsize + परिमाण(tdb_off_t));

		यदि (rec_ptr != 0) {
			rec.कुञ्चिका_len = कुञ्चिका.dsize;
			rec.दत्त_len = dbuf.dsize;
			rec.पूर्णम्_hash = hash;
			rec.magic = TDB_MAGIC;
			यदि (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + परिमाण(rec),
				    p, कुञ्चिका.dsize + dbuf.dsize) == -1) {
				गच्छ fail;
			}
			गच्छ समाप्तम्;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	यदि (tdb_तालकम्(tdb, -1, F_WRLCK) == -1) {
		गच्छ fail;
	}

	यदि ((tdb->max_dead_लेखाः != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		गच्छ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, कुञ्चिका.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	यदि (rec_ptr == 0) {
		गच्छ fail;
	}

	/* Read hash top into next ptr */
	यदि (tdb_ofs_read(tdb, TDB_HASH_शिखरम्(hash), &rec.अग्र) == -1)
		गच्छ fail;

	rec.कुञ्चिका_len = कुञ्चिका.dsize;
	rec.दत्त_len = dbuf.dsize;
	rec.पूर्णम्_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	यदि (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+परिमाण(rec), p, कुञ्चिका.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_शिखरम्(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		गच्छ fail;
	}

 समाप्तम्:
	ret = 0;
 fail:
	यदि (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	SAFE_FREE(p);
	tdb_unlock(tdb, BUCKET(hash), F_WRLCK);
	प्रत्यागम ret;
}


/* Append to an entry. Create if not exist. */
पूर्णाङ्क tdb_append(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, TDB_दत्त new_dbuf)
{
	u32 hash;
	TDB_दत्त dbuf;
	पूर्णाङ्क ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&कुञ्चिका);
	यदि (tdb_तालकम्(tdb, BUCKET(hash), F_WRLCK) == -1)
		प्रत्यागम -1;

	dbuf = tdb_fetch(tdb, कुञ्चिका);

	यदि (dbuf.dptr == रिक्त) {
		dbuf.dptr = (अचिह्नित वर्ण *)स्मृतिं_देहि(new_dbuf.dsize);
	} अन्यथा {
		अचिह्नित वर्ण *new_dptr = (अचिह्नित वर्ण *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		यदि (new_dptr == रिक्त) {
			मुक्त(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	यदि (dbuf.dptr == रिक्त) {
		tdb->ecode = TDB_ERR_OOM;
		गच्छ failed;
	}

	स्मृतिं_प्रतिलिख(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
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
	प्रत्यागम tdb->लघुगणकः.log_fn;
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

	यदि (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		प्रत्यागम 0;
	प्रत्यागम seqnum;
}

पूर्णाङ्क tdb_hash_आकार(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->शीर्षकम्.hash_आकार;
}

size_t tdb_map_आकार(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->map_आकार;
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
स्थिर अचिह्नित पूर्णाङ्क default_tdb_hash(TDB_दत्त *कुञ्चिका)
{
	u32 मानम्;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	प्रति (मानम् = 0, i=0; i < कुञ्चिका->dsize; i++)
		मानम् = मानम् * 256 + कुञ्चिका->dptr[i] + (मानम् >> 24) * 241;

	प्रत्यागम मानम्;
}


/* initialise a new database with a specified hash size */
स्थिर पूर्णाङ्क tdb_new_database(संरचना tdb_context *tdb, पूर्णाङ्क hash_आकार)
{
	संरचना tdb_शीर्षकम् *newdb;
	पूर्णाङ्क आकार, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	आकार = परिमाण(संरचना tdb_शीर्षकम्) + (hash_आकार+1)*परिमाण(tdb_off_t);
	यदि (!(newdb = (संरचना tdb_शीर्षकम् *)शून्यस्मृतिं_देहि(आकार, 1)))
		प्रत्यागम TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->संस्करणम् = TDB_संस्करणम्;
	newdb->hash_आकार = hash_आकार;
	यदि (tdb->flags & TDB_आन्तरिकम्) {
		tdb->map_आकार = आकार;
		tdb->map_ptr = (वर्ण *)newdb;
		स्मृतिं_प्रतिलिख(&tdb->शीर्षकम्, newdb, परिमाण(tdb->शीर्षकम्));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		प्रत्यागम 0;
	}
	यदि (lseek(tdb->fd, 0, SEEK_SET) == -1)
		गच्छ fail;

	यदि (ftruncate(tdb->fd, 0) == -1)
		गच्छ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	स्मृतिं_प्रतिलिख(&tdb->शीर्षकम्, newdb, परिमाण(tdb->शीर्षकम्));
	/* Don't endian-convert the magic food! */
	स्मृतिं_प्रतिलिख(newdb->magic_food, TDB_MAGIC_FOOD, वाक्यदैर्घ्यम्(TDB_MAGIC_FOOD)+1);
	यदि (लिख(tdb->fd, newdb, आकार) != आकार) {
		ret = -1;
	} अन्यथा {
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

	प्रति (i = tdbs; i; i = i->अग्र) {
		यदि (i->device == device && i->inode == ino) {
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
संरचना tdb_context *tdb_open(ध्रुव वर्ण *name, पूर्णाङ्क hash_आकार, पूर्णाङ्क tdb_flags,
		      पूर्णाङ्क open_flags, mode_t बहुलकम्)
{
	प्रत्यागम tdb_open_ex(name, hash_आकार, tdb_flags, open_flags, बहुलकम्, रिक्त, रिक्त);
}

/* a default logging function */
स्थिर शून्य null_log_fn(संरचना tdb_context *tdb, गणना tdb_debug_level level, ध्रुव वर्ण *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
स्थिर शून्य null_log_fn(संरचना tdb_context *tdb EXT2FS_ATTR((अप्रयुक्तम्)),
			गणना tdb_debug_level level EXT2FS_ATTR((अप्रयुक्तम्)),
			ध्रुव वर्ण *fmt EXT2FS_ATTR((अप्रयुक्तम्)), ...)
{
}


संरचना tdb_context *tdb_open_ex(ध्रुव वर्ण *name, पूर्णाङ्क hash_आकार, पूर्णाङ्क tdb_flags,
				पूर्णाङ्क open_flags, mode_t बहुलकम्,
				ध्रुव संरचना tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	संरचना tdb_context *tdb;
	संरचना stat st;
	पूर्णाङ्क rev = 0, locked = 0;
	अचिह्नित वर्ण *vp;
	u32 vertest;

	यदि (!(tdb = (संरचना tdb_context *)शून्यस्मृतिं_देहि(1, परिमाण *tdb))) {
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
	यदि (log_ctx) {
		tdb->लघुगणकः = *log_ctx;
	} अन्यथा {
		tdb->लघुगणकः.log_fn = null_log_fn;
		tdb->लघुगणकः.log_वैयक्तिकम् = रिक्त;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->page_आकार = sysconf(_SC_PAGESIZE);
	यदि (tdb->page_आकार <= 0) {
		tdb->page_आकार = 0x2000;
	}

	यदि ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		गच्छ fail;
	}

	यदि (hash_आकार == 0)
		hash_आकार = DEFAULT_HASH_आकार;
	यदि ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	यदि (tdb->flags & TDB_आन्तरिकम्) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		यदि (tdb_new_database(tdb, hash_आकार) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			गच्छ fail;
		}
		गच्छ आन्तरिकम्;
	}

	यदि ((tdb->fd = उद्घाटय(name, open_flags, बहुलकम्)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		गच्छ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	यदि (tdb->methods->tdb_brlock(tdb, GLOBAL_तालकम्, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		गच्छ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	यदि ((tdb_flags & TDB_CLEAR_IF_FIRST) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, सक्रिय_तालकम्, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		यदि (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			गच्छ fail; /* errno set by ftruncate */
		}
	}

	यदि (पठ(tdb->fd, &tdb->शीर्षकम्, परिमाण(tdb->शीर्षकम्)) != परिमाण(tdb->शीर्षकम्)
	    || memcmp(tdb->शीर्षकम्.magic_food, TDB_MAGIC_FOOD,
		      परिमाण(TDB_MAGIC_FOOD)) != 0
	    || (tdb->शीर्षकम्.संस्करणम् != TDB_संस्करणम्
		&& !(rev = (tdb->शीर्षकम्.संस्करणम्==TDB_BYTEREV(TDB_संस्करणम्))))) {
		/* its not a valid database - possibly initialise it */
		यदि (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_आकार) == -1) {
			errno = EIO; /* ie bad format or something */
			गच्छ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (अचिह्नित वर्ण *)&tdb->शीर्षकम्.संस्करणम्;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_संस्करणम्) ? TDB_BIGENDIAN : 0;
	यदि (!rev)
		tdb->flags &= ~TDB_CONVERT;
	अन्यथा {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->शीर्षकम्, परिमाण(tdb->शीर्षकम्));
	}
	यदि (fstat(tdb->fd, &st) == -1)
		गच्छ fail;

	यदि (tdb->शीर्षकम्.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		गच्छ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	यदि (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (पूर्णाङ्क)st.st_dev, (पूर्णाङ्क)st.st_ino));
		errno = EBUSY;
		गच्छ fail;
	}

	यदि (!(tdb->name = (वर्ण *)strdup(name))) {
		errno = ENOMEM;
		गच्छ fail;
	}

	tdb->map_आकार = st.st_आकार;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_लेखाः = 0;
	tdb_mmap(tdb);
	यदि (locked) {
		यदि (tdb->methods->tdb_brlock(tdb, सक्रिय_तालकम्, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			गच्छ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	यदि (tdb_flags & TDB_CLEAR_IF_FIRST) {
		/* leave this lock in place to indicate it's in use */
		यदि (tdb->methods->tdb_brlock(tdb, सक्रिय_तालकम्, F_RDLCK, F_SETLKW, 0, 1) == -1)
			गच्छ fail;
	}

	/* if needed, run recovery */
	यदि (tdb_transaction_recover(tdb) == -1) {
		गच्छ fail;
	}

 आन्तरिकम्:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	यदि (tdb->methods->tdb_brlock(tdb, GLOBAL_तालकम्, F_UNLCK, F_SETLKW, 0, 1) == -1)
		गच्छ fail;
	tdb->अग्र = tdbs;
	tdbs = tdb;
	प्रत्यागम tdb;

 fail:
	{ पूर्णाङ्क save_errno = errno;

	यदि (!tdb)
		प्रत्यागम रिक्त;

	यदि (tdb->map_ptr) {
		यदि (tdb->flags & TDB_आन्तरिकम्)
			SAFE_FREE(tdb->map_ptr);
		अन्यथा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	यदि (tdb->fd != -1)
		यदि (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	SAFE_FREE(tdb);
	errno = save_errno;
	प्रत्यागम रिक्त;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

शून्य tdb_समुच्चयः_max_dead(संरचना tdb_context *tdb, पूर्णाङ्क max_dead)
{
	tdb->max_dead_लेखाः = max_dead;
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

	यदि (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	यदि (tdb->map_ptr) {
		यदि (tdb->flags & TDB_आन्तरिकम्)
			SAFE_FREE(tdb->map_ptr);
		अन्यथा
			tdb_munmap(tdb);
	}
	SAFE_FREE(tdb->name);
	यदि (tdb->fd != -1)
		ret = close(tdb->fd);
	SAFE_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	प्रति (i = &tdbs; *i; i = &(*i)->अग्र) {
		यदि (*i == tdb) {
			*i = tdb->अग्र;
			भङ्ग;
		}
	}

	स्मृतिं_पूरय(tdb, 0, परिमाण(*tdb));
	SAFE_FREE(tdb);

	प्रत्यागम ret;
}

/* register a logging function */
शून्य tdb_समुच्चयः_logging_function(संरचना tdb_context *tdb,
                              ध्रुव संरचना tdb_logging_context *log_ctx)
{
        tdb->लघुगणकः = *log_ctx;
}

शून्य *tdb_get_logging_वैयक्तिकम्(संरचना tdb_context *tdb)
{
	प्रत्यागम tdb->लघुगणकः.log_वैयक्तिकम्;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
पूर्णाङ्क tdb_reopen(संरचना tdb_context *tdb)
{
	संरचना stat st;

	यदि (tdb->flags & TDB_आन्तरिकम्) {
		प्रत्यागम 0; /* Nothing to do. */
	}

	यदि (tdb->अङ्कः_locks != 0 || tdb->global_तालकम्.गणनम्) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		गच्छ fail;
	}

	यदि (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		गच्छ fail;
	}

	यदि (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		गच्छ fail;
	}
	यदि (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = उद्घाटय(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	यदि (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		गच्छ fail;
	}
	यदि ((tdb->flags & TDB_CLEAR_IF_FIRST) &&
	    (tdb->methods->tdb_brlock(tdb, सक्रिय_तालकम्, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		गच्छ fail;
	}
	यदि (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		गच्छ fail;
	}
	यदि (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
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

	प्रति (tdb=tdbs; tdb; tdb = tdb->अग्र) {
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
		यदि (parent_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_FIRST;
		}

		यदि (tdb_reopen(tdb) != 0)
			प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

/**
 * Flush a database file from the page cache.
 **/
पूर्णाङ्क tdb_flush(संरचना tdb_context *tdb)
{
	यदि (tdb->fd != -1)
		प्रत्यागम fsync(tdb->fd);
	प्रत्यागम 0;
}
