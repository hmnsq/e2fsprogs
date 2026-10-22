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

#నిర్వచితమైతే CONFIG_STAND_ALONE
#నిర్వచించు HAVE_MMAP
#నిర్వచించు HAVE_STRDUP
#నిర్వచించు HAVE_SYS_MMAN_H
#నిర్వచించు HAVE_UTIME_H
#నిర్వచించు HAVE_UTIME
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే __FreeBSD__
#నిర్వచించు XOPEN_ఆధారం 600
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <string.h>
#నిర్వచితమైతే HAVE_SYS_SELECT_H
#అన్తర్భావ <sys/select.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <time.h>
#నిర్వచితమైతే HAVE_UTIME_H
#అన్తర్భావ <utime.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/file.h>
#అన్తర్భావ <fcntl.h>

#నిర్వచితమైతే HAVE_SYS_MMAN_H
#అన్తర్భావ <sys/mman.h>
#అయితే_ముగింపు

#నిర్వచితమైతే __GNUC__
#నిర్వచించు EXT2FS_ATTR(x) __attribute__(x)
#లేకపోతే
#నిర్వచించు EXT2FS_ATTR(x)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MAP_FILE
#నిర్వచించు MAP_FILE 0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MAP_FAILED
#నిర్వచించు MAP_FAILED ((శూన్య *)-1)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HAVE_STRDUP
#నిర్వచించు strdup rep_strdup
స్థిర అక్షరం *rep_strdup(స్థిరరాశి అక్షరం *s)
{
	అక్షరం *ret;
	పూర్ణాంకం నిడివి;

	ఒకవేళ (!s)
		తిరిగి ఖాళీ;
	నిడివి = అక్షరశ్రేణి_పొడవు(s);
	ret = జ్ఞాపకం_ఇవ్వు(నిడివి + 1);
	ఒకవేళ (ret) {
		strncpy(ret, s, నిడివి);
		ret[నిడివి] = '\0';
	}
	తిరిగి ret;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే PRINTF_ATTRIBUTE
#ఒకవేళ (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#నిర్వచించు PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#లేకపోతే
#నిర్వచించు PRINTF_ATTRIBUTE(a1, a2)
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
రకనిర్వచనం పూర్ణాంకం bool;
#అయితే_ముగింపు

#అన్తర్భావ "tdb.h"

స్థిర TDB_దత్తాంశం tdb_null;

#నిర్వచితం_కాకపోతే u32
#నిర్వచించు u32 గుర్తులేని
#అయితే_ముగింపు

రకనిర్వచనం u32 tdb_len_t;
రకనిర్వచనం u32 tdb_off_t;

#నిర్వచితం_కాకపోతే offsetof
#నిర్వచించు offsetof(t,f) ((గుర్తులేని పూర్ణాంకం)&((t *)0)->f)
#అయితే_ముగింపు

#నిర్వచించు TDB_MAGIC_FOOD "TDB file\n"
#నిర్వచించు TDB_సంస్కరణ (0x26011967 + 6)
#నిర్వచించు TDB_MAGIC (0x26011999U)
#నిర్వచించు TDB_FREE_MAGIC (~TDB_MAGIC)
#నిర్వచించు TDB_DEAD_MAGIC (0xFEE1DEAD)
#నిర్వచించు TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#నిర్వచించు TDB_ALIGNMENT 4
#నిర్వచించు MIN_REC_కొలత (2*పరిమాణం(నిర్మాణం list_struct) + TDB_ALIGNMENT)
#నిర్వచించు DEFAULT_HASH_కొలత 131
#నిర్వచించు FREELIST_శిఖరం (పరిమాణం(నిర్మాణం tdb_శీర్షిక))
#నిర్వచించు TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#నిర్వచించు TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#నిర్వచించు TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#నిర్వచించు TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#నిర్వచించు TDB_HASH_శిఖరం(hash) (FREELIST_శిఖరం + (పాత్ర(hash)+1)*పరిమాణం(tdb_off_t))
#నిర్వచించు TDB_HASHTABLE_కొలత(tdb) ((tdb->శీర్షిక.hash_కొలత+1)*పరిమాణం(tdb_off_t))
#నిర్వచించు TDB_దత్తాంశం_ప్రారంభం(hash_కొలత) TDB_HASH_శిఖరం(hash_కొలత-1)
#నిర్వచించు TDB_RECOVERY_తల offsetof(నిర్మాణం tdb_శీర్షిక, recovery_ప్రారంభం)
#నిర్వచించు TDB_SEQNUM_OFS    offsetof(నిర్మాణం tdb_శీర్షిక, sequence_సంఖ్య)
#నిర్వచించు TDB_PAD_BYTE 0x42
#నిర్వచించు TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#నిర్వచించు TDB_LOG(x) tdb->సంవర్గమానం.log_fn x

/* lock offsets */
#నిర్వచించు GLOBAL_తాళం      0
#నిర్వచించు చురుకైన_తాళం      4
#నిర్వచించు TRANSACTION_తాళం 8

/* free memory if the pointer is valid and zero the pointer */
#నిర్వచితం_కాకపోతే సురక్షిత_FREE
#నిర్వచించు సురక్షిత_FREE(x) చేయి { ఒకవేళ ((x) != ఖాళీ) {విడుదల(x); (x)=ఖాళీ;} } అయితే(0)
#అయితే_ముగింపు

#నిర్వచించు పాత్ర(hash) ((hash) % tdb->శీర్షిక.hash_కొలత)

#నిర్వచించు DOCONV() (tdb->flags & TDB_CONVERT)
#నిర్వచించు CONVERT(x) (DOCONV() ? tdb_convert(&x, పరిమాణం(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
నిర్మాణం list_struct {
	tdb_off_t తదుపరి; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t కీలకం_len; /* byte length of key */
	tdb_len_t దత్తాంశం_len; /* byte length of data */
	u32 నిండిన_hash; /* the full 32 bit hash of the key */
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
నిర్మాణం tdb_శీర్షిక {
	అక్షరం magic_food[32]; /* for /etc/magic */
	u32 సంస్కరణ; /* version of the code */
	u32 hash_కొలత; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_ప్రారంభం; /* offset of transaction recovery region */
	tdb_off_t sequence_సంఖ్య; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

నిర్మాణం tdb_తాళం_type {
	పూర్ణాంకం list;
	u32 లెక్క;
	u32 ltype;
};

నిర్మాణం tdb_traverse_తాళం {
	నిర్మాణం tdb_traverse_తాళం *తదుపరి;
	u32 off;
	u32 hash;
	పూర్ణాంకం తాళం_rw;
};


నిర్మాణం tdb_methods {
	పూర్ణాంకం (*tdb_read)(నిర్మాణం tdb_context *, tdb_off_t , శూన్య *, tdb_len_t , పూర్ణాంకం );
	పూర్ణాంకం (*tdb_write)(నిర్మాణం tdb_context *, tdb_off_t, స్థిరరాశి శూన్య *, tdb_len_t);
	శూన్య (*తదుపరి_hash_chain)(నిర్మాణం tdb_context *, u32 *);
	పూర్ణాంకం (*tdb_oob)(నిర్మాణం tdb_context *, tdb_off_t , పూర్ణాంకం );
	పూర్ణాంకం (*tdb_expand_file)(నిర్మాణం tdb_context *, tdb_off_t , tdb_off_t );
	పూర్ణాంకం (*tdb_brlock)(నిర్మాణం tdb_context *, tdb_off_t , పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం, size_t);
};

నిర్మాణం tdb_context {
	అక్షరం *name; /* the name of the database */
	శూన్య *map_ptr; /* where it is currently mapped */
	పూర్ణాంకం fd; /* open file descriptor for the database */
	tdb_len_t map_కొలత; /* how much space has been mapped */
	పూర్ణాంకం read_only; /* opened read-only */
	పూర్ణాంకం traverse_read; /* read-only traversal */
	నిర్మాణం tdb_తాళం_type global_తాళం;
	పూర్ణాంకం అంకె_lockrecs;
	నిర్మాణం tdb_తాళం_type *lockrecs; /* only real locks, all with count>0 */
	గణన TDB_ERROR ecode; /* error code for last tdb error */
	నిర్మాణం tdb_శీర్షిక శీర్షిక; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	నిర్మాణం tdb_traverse_తాళం travlocks; /* current traversal locks */
	నిర్మాణం tdb_context *తదుపరి; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	నిర్మాణం tdb_logging_context సంవర్గమానం;
	గుర్తులేని పూర్ణాంకం (*hash_fn)(TDB_దత్తాంశం *కీలకం);
	పూర్ణాంకం open_flags; /* flags used in the open - needed by reopen */
	గుర్తులేని పూర్ణాంకం అంకె_locks; /* number of chain locks held */
	స్థిరరాశి నిర్మాణం tdb_methods *methods;
	నిర్మాణం tdb_transaction *transaction;
	పూర్ణాంకం పుట_కొలత;
	పూర్ణాంకం max_dead_రికార్డులు;
	bool have_transaction_తాళం;
	tdb_len_t real_map_కొలత; /* how much space has been mapped */
};


/*
  internal prototypes
*/
స్థిర పూర్ణాంకం tdb_munmap(నిర్మాణం tdb_context *tdb);
స్థిర శూన్య tdb_mmap(నిర్మాణం tdb_context *tdb);
స్థిర పూర్ణాంకం tdb_తాళం(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype);
పూర్ణాంకం tdb_తాళం_nonblock(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype);
స్థిర పూర్ణాంకం tdb_unlock(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype);
స్థిర పూర్ణాంకం tdb_brlock(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, పూర్ణాంకం rw_type, పూర్ణాంకం lck_type, పూర్ణాంకం probe, size_t len);
స్థిర పూర్ణాంకం tdb_transaction_తాళం(నిర్మాణం tdb_context *tdb, పూర్ణాంకం ltype);
స్థిర పూర్ణాంకం tdb_transaction_unlock(నిర్మాణం tdb_context *tdb);
స్థిర పూర్ణాంకం tdb_brlock_upgrade(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, size_t len);
స్థిర పూర్ణాంకం tdb_write_తాళం_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off);
స్థిర పూర్ణాంకం tdb_write_unlock_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off);
స్థిర పూర్ణాంకం tdb_ofs_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_off_t *d);
స్థిర పూర్ణాంకం tdb_ofs_write(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_off_t *d);
స్థిర శూన్య *tdb_convert(శూన్య *buf, u32 కొలత);
స్థిర పూర్ణాంకం tdb_free(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, నిర్మాణం list_struct *rec);
స్థిర tdb_off_t tdb_allocate(నిర్మాణం tdb_context *tdb, tdb_len_t నిడివి, నిర్మాణం list_struct *rec);
స్థిర పూర్ణాంకం tdb_ofs_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_off_t *d);
స్థిర పూర్ణాంకం tdb_ofs_write(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_off_t *d);
స్థిర పూర్ణాంకం tdb_తాళం_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off);
స్థిర పూర్ణాంకం tdb_unlock_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off);
స్థిర పూర్ణాంకం tdb_rec_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, నిర్మాణం list_struct *rec);
స్థిర పూర్ణాంకం tdb_rec_write(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, నిర్మాణం list_struct *rec);
స్థిర పూర్ణాంకం tdb_do_delete(నిర్మాణం tdb_context *tdb, tdb_off_t rec_ptr, నిర్మాణం list_struct *rec);
స్థిర గుర్తులేని అక్షరం *tdb_alloc_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_len_t len);
స్థిర పూర్ణాంకం tdb_parse_దత్తాంశం(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం,
		   tdb_off_t స్థానభ్రంశం, tdb_len_t len,
		   పూర్ణాంకం (*parser)(TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం దత్తాంశం,
				 శూన్య *వ్యక్తిగత_దత్తాంశం),
		   శూన్య *వ్యక్తిగత_దత్తాంశం);
స్థిర tdb_off_t tdb_కనుగొను_తాళం_hash(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, u32 hash, పూర్ణాంకం locktype,
			   నిర్మాణం list_struct *rec);
స్థిర శూన్య tdb_io_init(నిర్మాణం tdb_context *tdb);
స్థిర పూర్ణాంకం tdb_expand(నిర్మాణం tdb_context *tdb, tdb_off_t కొలత);
స్థిర పూర్ణాంకం tdb_rec_free_read(నిర్మాణం tdb_context *tdb, tdb_off_t off,
		      నిర్మాణం list_struct *rec);


/* file: error.c */

గణన TDB_ERROR tdb_error(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->ecode;
}

స్థిర నిర్మాణం tdb_errname {
	గణన TDB_ERROR ecode; స్థిరరాశి అక్షరం *estring;
} emap[] = { {TDB_SUCCESS, "Success"},
	     {TDB_ERR_CORRUPT, "Corrupt database"},
	     {TDB_ERR_IO, "IO Error"},
	     {TDB_ERR_తాళం, "Locking error"},
	     {TDB_ERR_OOM, "Out of memory"},
	     {TDB_ERR_EXISTS, "Record exists"},
	     {TDB_ERR_NOLOCK, "Lock exists on other keys"},
	     {TDB_ERR_EINVAL, "Invalid parameter"},
	     {TDB_ERR_NOEXIST, "Record does not exist"},
	     {TDB_ERR_RDONLY, "write not permitted"} };

/* Error string for the last tdb error */
స్థిరరాశి అక్షరం *tdb_errorstr(నిర్మాణం tdb_context *tdb)
{
	u32 i;
	కొరకు (i = 0; i < పరిమాణం(emap) / పరిమాణం(నిర్మాణం tdb_errname); i++)
		ఒకవేళ (tdb->ecode == emap[i].ecode)
			తిరిగి emap[i].estring;
	తిరిగి "Invalid error code";
}

/* file: lock.c */

#నిర్వచించు TDB_MARK_తాళం 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
పూర్ణాంకం tdb_brlock(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం,
	       పూర్ణాంకం rw_type, పూర్ణాంకం lck_type, పూర్ణాంకం probe, size_t len)
{
	నిర్మాణం flock fl;
	పూర్ణాంకం ret;

	ఒకవేళ (tdb->flags & TDB_NOLOCK) {
		తిరిగి 0;
	}

	ఒకవేళ ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		తిరిగి -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_ప్రారంభం = స్థానభ్రంశం;
	fl.l_len = len;
	fl.l_pid = 0;

	చేయి {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} అయితే (ret == -1 && errno == EINTR);

	ఒకవేళ (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		ఒకవేళ (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_తాళం;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, స్థానభ్రంశం, rw_type, lck_type, (పూర్ణాంకం)len));
		}
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}
	తిరిగి 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
పూర్ణాంకం tdb_brlock_upgrade(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, size_t len)
{
	పూర్ణాంకం లెక్క = 1000;
	అయితే (లెక్క--) {
		నిర్మాణం timeval tv;
		ఒకవేళ (tdb_brlock(tdb, స్థానభ్రంశం, F_WRLCK, F_SETLKW, 1, len) == 0) {
			తిరిగి 0;
		}
		ఒకవేళ (errno != EDEADLK) {
			ఆపు;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ఖాళీ, ఖాళీ, ఖాళీ, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", స్థానభ్రంశం));
	తిరిగి -1;
}


/* lock a list in the database. list -1 is the alloc list */
స్థిర పూర్ణాంకం tdb_తాళం_2(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype, పూర్ణాంకం op)
{
	నిర్మాణం tdb_తాళం_type *new_lck;
	పూర్ణాంకం i;
	bool mark_తాళం = ((ltype & TDB_MARK_తాళం) == TDB_MARK_తాళం);

	ltype &= ~TDB_MARK_తాళం;

	/* a global lock allows us to avoid per chain locks */
	ఒకవేళ (tdb->global_తాళం.లెక్క &&
	    ((u32)ltype == tdb->global_తాళం.ltype || ltype == F_RDLCK)) {
		తిరిగి 0;
	}

	ఒకవేళ (tdb->global_తాళం.లెక్క) {
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}

	ఒకవేళ (list < -1 || list >= (పూర్ణాంకం)tdb->శీర్షిక.hash_కొలత) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		తిరిగి -1;
	}
	ఒకవేళ (tdb->flags & TDB_NOLOCK)
		తిరిగి 0;

	కొరకు (i=0; i<tdb->అంకె_lockrecs; i++) {
		ఒకవేళ (tdb->lockrecs[i].list == list) {
			ఒకవేళ (tdb->lockrecs[i].లెక్క == 0) {
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
			tdb->lockrecs[i].లెక్క++;
			తిరిగి 0;
		}
	}

	new_lck = (నిర్మాణం tdb_తాళం_type *)realloc(
		tdb->lockrecs,
		పరిమాణం(*tdb->lockrecs) * (tdb->అంకె_lockrecs+1));
	ఒకవేళ (new_lck == ఖాళీ) {
		errno = ENOMEM;
		తిరిగి -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	ఒకవేళ (!mark_తాళం &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_శిఖరం+4*list, ltype, op,
				     0, 1)) {
		తిరిగి -1;
	}

	tdb->అంకె_locks++;

	tdb->lockrecs[tdb->అంకె_lockrecs].list = list;
	tdb->lockrecs[tdb->అంకె_lockrecs].లెక్క = 1;
	tdb->lockrecs[tdb->అంకె_lockrecs].ltype = ltype;
	tdb->అంకె_lockrecs += 1;

	తిరిగి 0;
}

/* lock a list in the database. list -1 is the alloc list */
పూర్ణాంకం tdb_తాళం(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype)
{
	పూర్ణాంకం ret;
	ret = tdb_తాళం_2(tdb, list, ltype, F_SETLKW);
	ఒకవేళ (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	తిరిగి ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
పూర్ణాంకం tdb_తాళం_nonblock(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype)
{
	తిరిగి tdb_తాళం_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
పూర్ణాంకం tdb_unlock(నిర్మాణం tdb_context *tdb, పూర్ణాంకం list, పూర్ణాంకం ltype)
{
	పూర్ణాంకం ret = -1;
	పూర్ణాంకం i;
	నిర్మాణం tdb_తాళం_type *lck = ఖాళీ;
	bool mark_తాళం = ((ltype & TDB_MARK_తాళం) == TDB_MARK_తాళం);

	ltype &= ~TDB_MARK_తాళం;

	/* a global lock allows us to avoid per chain locks */
	ఒకవేళ (tdb->global_తాళం.లెక్క &&
	    ((u32)ltype == tdb->global_తాళం.ltype || ltype == F_RDLCK)) {
		తిరిగి 0;
	}

	ఒకవేళ (tdb->global_తాళం.లెక్క) {
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}

	ఒకవేళ (tdb->flags & TDB_NOLOCK)
		తిరిగి 0;

	/* Sanity checks */
	ఒకవేళ (list < -1 || list >= (పూర్ణాంకం)tdb->శీర్షిక.hash_కొలత) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->శీర్షిక.hash_కొలత));
		తిరిగి ret;
	}

	కొరకు (i=0; i<tdb->అంకె_lockrecs; i++) {
		ఒకవేళ (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			ఆపు;
		}
	}

	ఒకవేళ ((lck == ఖాళీ) || (lck->లెక్క == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		తిరిగి -1;
	}

	ఒకవేళ (lck->లెక్క > 1) {
		lck->లెక్క--;
		తిరిగి 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	ఒకవేళ (mark_తాళం) {
		ret = 0;
	} లేకపోతే {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_శిఖరం+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->అంకె_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	ఒకవేళ (tdb->అంకె_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->అంకె_lockrecs-1];
	}
	tdb->అంకె_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	ఒకవేళ (tdb->అంకె_lockrecs == 0) {
		సురక్షిత_FREE(tdb->lockrecs);
	}

	ఒకవేళ (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	తిరిగి ret;
}

/*
  get the transaction lock
 */
పూర్ణాంకం tdb_transaction_తాళం(నిర్మాణం tdb_context *tdb, పూర్ణాంకం ltype)
{
	ఒకవేళ (tdb->have_transaction_తాళం || tdb->global_తాళం.లెక్క) {
		తిరిగి 0;
	}
	ఒకవేళ (tdb->methods->tdb_brlock(tdb, TRANSACTION_తాళం, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_తాళం;
		తిరిగి -1;
	}
	tdb->have_transaction_తాళం = 1;
	తిరిగి 0;
}

/*
  release the transaction lock
 */
పూర్ణాంకం tdb_transaction_unlock(నిర్మాణం tdb_context *tdb)
{
	పూర్ణాంకం ret;
	ఒకవేళ (!tdb->have_transaction_తాళం) {
		తిరిగి 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_తాళం, F_UNLCK, F_SETLKW, 0, 1);
	ఒకవేళ (ret == 0) {
		tdb->have_transaction_తాళం = 0;
	}
	తిరిగి ret;
}




/* lock/unlock entire database */
స్థిర పూర్ణాంకం _tdb_lockall(నిర్మాణం tdb_context *tdb, పూర్ణాంకం ltype, పూర్ణాంకం op)
{
	bool mark_తాళం = ((ltype & TDB_MARK_తాళం) == TDB_MARK_తాళం);

	ltype &= ~TDB_MARK_తాళం;

	/* There are no locks on read-only dbs */
	ఒకవేళ (tdb->read_only || tdb->traverse_read)
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);

	ఒకవేళ (tdb->global_తాళం.లెక్క && tdb->global_తాళం.ltype == (u32)ltype) {
		tdb->global_తాళం.లెక్క++;
		తిరిగి 0;
	}

	ఒకవేళ (tdb->global_తాళం.లెక్క) {
		/* a global lock of a different type exists */
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}

	ఒకవేళ (tdb->అంకె_locks != 0) {
		/* can't combine global and chain locks */
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}

	ఒకవేళ (!mark_తాళం &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_శిఖరం, ltype, op,
				     0, 4*tdb->శీర్షిక.hash_కొలత)) {
		ఒకవేళ (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		తిరిగి -1;
	}

	tdb->global_తాళం.లెక్క = 1;
	tdb->global_తాళం.ltype = ltype;

	తిరిగి 0;
}



/* unlock entire db */
స్థిర పూర్ణాంకం _tdb_unlockall(నిర్మాణం tdb_context *tdb, పూర్ణాంకం ltype)
{
	bool mark_తాళం = ((ltype & TDB_MARK_తాళం) == TDB_MARK_తాళం);

	ltype &= ~TDB_MARK_తాళం;

	/* There are no locks on read-only dbs */
	ఒకవేళ (tdb->read_only || tdb->traverse_read) {
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}

	ఒకవేళ (tdb->global_తాళం.ltype != (u32)ltype ||
	    tdb->global_తాళం.లెక్క == 0) {
		తిరిగి TDB_ERRCODE(TDB_ERR_తాళం, -1);
	}

	ఒకవేళ (tdb->global_తాళం.లెక్క > 1) {
		tdb->global_తాళం.లెక్క--;
		తిరిగి 0;
	}

	ఒకవేళ (!mark_తాళం &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_శిఖరం, F_UNLCK, F_SETLKW,
				     0, 4*tdb->శీర్షిక.hash_కొలత)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		తిరిగి -1;
	}

	tdb->global_తాళం.లెక్క = 0;
	tdb->global_తాళం.ltype = 0;

	తిరిగి 0;
}

/* lock entire database with write lock */
పూర్ణాంకం tdb_lockall(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
పూర్ణాంకం tdb_lockall_mark(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_lockall(tdb, F_WRLCK | TDB_MARK_తాళం, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
పూర్ణాంకం tdb_lockall_unmark(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_తాళం);
}

/* lock entire database with write lock - nonblocking variant */
పూర్ణాంకం tdb_lockall_nonblock(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
పూర్ణాంకం tdb_unlockall(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
పూర్ణాంకం tdb_lockall_read(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
పూర్ణాంకం tdb_lockall_read_nonblock(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
పూర్ణాంకం tdb_unlockall_read(నిర్మాణం tdb_context *tdb)
{
	తిరిగి _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
పూర్ణాంకం tdb_chainlock(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_తాళం(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
పూర్ణాంకం tdb_chainlock_nonblock(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_తాళం_nonblock(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
పూర్ణాంకం tdb_chainlock_mark(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_తాళం(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_WRLCK | TDB_MARK_తాళం);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
పూర్ణాంకం tdb_chainlock_unmark(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_unlock(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_WRLCK | TDB_MARK_తాళం);
}

పూర్ణాంకం tdb_chainunlock(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_unlock(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_WRLCK);
}

పూర్ణాంకం tdb_chainlock_read(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_తాళం(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_RDLCK);
}

పూర్ణాంకం tdb_chainunlock_read(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	తిరిగి tdb_unlock(tdb, పాత్ర(tdb->hash_fn(&కీలకం)), F_RDLCK);
}



/* record lock stops delete underneath */
పూర్ణాంకం tdb_తాళం_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off)
{
	తిరిగి off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
పూర్ణాంకం tdb_write_తాళం_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off)
{
	నిర్మాణం tdb_traverse_తాళం *i;
	కొరకు (i = &tdb->travlocks; i; i = i->తదుపరి)
		ఒకవేళ (i->off == off)
			తిరిగి -1;
	తిరిగి tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
పూర్ణాంకం tdb_write_unlock_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off)
{
	తిరిగి tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
పూర్ణాంకం tdb_unlock_రికార్డు(నిర్మాణం tdb_context *tdb, tdb_off_t off)
{
	నిర్మాణం tdb_traverse_తాళం *i;
	u32 లెక్క = 0;

	ఒకవేళ (off == 0)
		తిరిగి 0;
	కొరకు (i = &tdb->travlocks; i; i = i->తదుపరి)
		ఒకవేళ (i->off == off)
			లెక్క++;
	తిరిగి (లెక్క == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
స్థిర పూర్ణాంకం tdb_oob(నిర్మాణం tdb_context *tdb, tdb_off_t len, పూర్ణాంకం probe)
{
	నిర్మాణం stat st;
	ఒకవేళ (len <= tdb->map_కొలత)
		తిరిగి 0;
	ఒకవేళ (tdb->flags & TDB_అంతర్గత) {
		ఒకవేళ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (పూర్ణాంకం)len, (పూర్ణాంకం)tdb->map_కొలత));
		}
		తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ఒకవేళ (fstat(tdb->fd, &st) == -1) {
		తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ఒకవేళ (st.st_కొలత < (off_t)len) {
		ఒకవేళ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (పూర్ణాంకం)len, (పూర్ణాంకం)st.st_కొలత));
		}
		తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	ఒకవేళ (tdb_munmap(tdb) == -1)
		తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_కొలత = st.st_కొలత;
	tdb_mmap(tdb);
	తిరిగి 0;
}

/* write a lump of data at a specified offset */
స్థిర పూర్ణాంకం tdb_write(నిర్మాణం tdb_context *tdb, tdb_off_t off,
		     స్థిరరాశి శూన్య *buf, tdb_len_t len)
{
	ఒకవేళ (len == 0) {
		తిరిగి 0;
	}

	ఒకవేళ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		తిరిగి -1;
	}

	ఒకవేళ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		తిరిగి -1;

	ఒకవేళ (tdb->map_ptr) {
		జ్ఞాపకం_నకలు(off + (అక్షరం *)tdb->map_ptr, buf, len);
	} లేకపోతే ఒకవేళ (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	తిరిగి 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
శూన్య *tdb_convert(శూన్య *buf, u32 కొలత)
{
	u32 i, *p = (u32 *)buf;
	కొరకు (i = 0; i < కొలత / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	తిరిగి buf;
}


/* read a lump of data at a specified offset, maybe convert */
స్థిర పూర్ణాంకం tdb_read(నిర్మాణం tdb_context *tdb, tdb_off_t off, శూన్య *buf,
		    tdb_len_t len, పూర్ణాంకం cv)
{
	ఒకవేళ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		తిరిగి -1;
	}

	ఒకవేళ (tdb->map_ptr) {
		జ్ఞాపకం_నకలు(buf, off + (అక్షరం *)tdb->map_ptr, len);
	} లేకపోతే {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		ఒకవేళ (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (పూర్ణాంకం)off, (పూర్ణాంకం)len, (పూర్ణాంకం)ret, strerror(errno),
				 (పూర్ణాంకం)tdb->map_కొలత));
			తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	ఒకవేళ (cv) {
		tdb_convert(buf, len);
	}
	తిరిగి 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
స్థిర శూన్య tdb_తదుపరి_hash_chain(నిర్మాణం tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ఒకవేళ (tdb->map_ptr) {
		కొరకు (;h < tdb->శీర్షిక.hash_కొలత;h++) {
			ఒకవేళ (0 != *(u32 *)(TDB_HASH_శిఖరం(h) + (గుర్తులేని అక్షరం *)tdb->map_ptr)) {
				ఆపు;
			}
		}
	} లేకపోతే {
		u32 off=0;
		కొరకు (;h < tdb->శీర్షిక.hash_కొలత;h++) {
			ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(h), &off) != 0 || off != 0) {
				ఆపు;
			}
		}
	}
	(*chain) = h;
}


పూర్ణాంకం tdb_munmap(నిర్మాణం tdb_context *tdb)
{
	ఒకవేళ (tdb->flags & TDB_అంతర్గత)
		తిరిగి 0;

#నిర్వచితమైతే HAVE_MMAP
	ఒకవేళ (tdb->map_ptr) {
		పూర్ణాంకం ret = munmap(tdb->map_ptr, tdb->real_map_కొలత);
		ఒకవేళ (ret != 0)
			తిరిగి ret;
		tdb->real_map_కొలత = 0;
	}
#అయితే_ముగింపు
	tdb->map_ptr = ఖాళీ;
	తిరిగి 0;
}

శూన్య tdb_mmap(నిర్మాణం tdb_context *tdb)
{
	ఒకవేళ (tdb->flags & TDB_అంతర్గత)
		తిరిగి;

#నిర్వచితమైతే HAVE_MMAP
	ఒకవేళ (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ఖాళీ, tdb->map_కొలత,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		ఒకవేళ (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_కొలత = 0;
			tdb->map_ptr = ఖాళీ;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_కొలత, strerror(errno)));
		}
		tdb->real_map_కొలత = tdb->map_కొలత;
	} లేకపోతే {
		tdb->map_ptr = ఖాళీ;
	}
#లేకపోతే
	tdb->map_ptr = ఖాళీ;
#అయితే_ముగింపు
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
స్థిర పూర్ణాంకం tdb_expand_file(నిర్మాణం tdb_context *tdb, tdb_off_t కొలత, tdb_off_t addition)
{
	అక్షరం buf[1024];

	ఒకవేళ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		తిరిగి -1;
	}

	ఒకవేళ (ftruncate(tdb->fd, కొలత+addition) == -1) {
		అక్షరం b = 0;
		ఒకవేళ (pwrite(tdb->fd,  &b, 1, (కొలత+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 కొలత+addition, strerror(errno)));
			తిరిగి -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, పరిమాణం(buf));
	అయితే (addition) {
		పూర్ణాంకం n = addition>పరిమాణం(buf)?పరిమాణం(buf):addition;
		పూర్ణాంకం ret = pwrite(tdb->fd, buf, n, కొలత);
		ఒకవేళ (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			తిరిగి -1;
		}
		addition -= n;
		కొలత += n;
	}
	తిరిగి 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
పూర్ణాంకం tdb_expand(నిర్మాణం tdb_context *tdb, tdb_off_t కొలత)
{
	నిర్మాణం list_struct rec;
	tdb_off_t స్థానభ్రంశం;

	ఒకవేళ (tdb_తాళం(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		తిరిగి -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_కొలత + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	కొలత = TDB_ALIGN(tdb->map_కొలత + కొలత*10, tdb->పుట_కొలత) - tdb->map_కొలత;

	ఒకవేళ (!(tdb->flags & TDB_అంతర్గత))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	ఒకవేళ (!(tdb->flags & TDB_అంతర్గత)) {
		ఒకవేళ (tdb->methods->tdb_expand_file(tdb, tdb->map_కొలత, కొలత) != 0)
			వెళ్ళు fail;
	}

	tdb->map_కొలత += కొలత;

	ఒకవేళ (tdb->flags & TDB_అంతర్గత) {
		అక్షరం *new_map_ptr = (అక్షరం *)realloc(tdb->map_ptr,
						    tdb->map_కొలత);
		ఒకవేళ (!new_map_ptr) {
			tdb->map_కొలత -= కొలత;
			వెళ్ళు fail;
		}
		tdb->map_ptr = new_map_ptr;
	} లేకపోతే {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',పరిమాణం(rec));
	rec.rec_len = కొలత - పరిమాణం(rec);

	/* link it into the free list */
	స్థానభ్రంశం = tdb->map_కొలత - కొలత;
	ఒకవేళ (tdb_free(tdb, స్థానభ్రంశం, &rec) == -1)
		వెళ్ళు fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి -1;
}

/* read/write a tdb_off_t */
పూర్ణాంకం tdb_ofs_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_off_t *d)
{
	తిరిగి tdb->methods->tdb_read(tdb, స్థానభ్రంశం, (అక్షరం*)d, పరిమాణం(*d), DOCONV());
}

పూర్ణాంకం tdb_ofs_write(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_off_t *d)
{
	tdb_off_t off = *d;
	తిరిగి tdb->methods->tdb_write(tdb, స్థానభ్రంశం, CONVERT(off), పరిమాణం(*d));
}


/* read a lump of data, allocating the space for it */
గుర్తులేని అక్షరం *tdb_alloc_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_len_t len)
{
	గుర్తులేని అక్షరం *buf;

	/* some systems don't like zero length malloc */
	ఒకవేళ (len == 0) {
		len = 1;
	}

	ఒకవేళ (!(buf = (గుర్తులేని అక్షరం *)జ్ఞాపకం_ఇవ్వు(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		తిరిగి TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	ఒకవేళ (tdb->methods->tdb_read(tdb, స్థానభ్రంశం, buf, len, 0) == -1) {
		సురక్షిత_FREE(buf);
		తిరిగి ఖాళీ;
	}
	తిరిగి buf;
}

/* Give a piece of tdb data to a parser */

పూర్ణాంకం tdb_parse_దత్తాంశం(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం,
		   tdb_off_t స్థానభ్రంశం, tdb_len_t len,
		   పూర్ణాంకం (*parser)(TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం దత్తాంశం,
				 శూన్య *వ్యక్తిగత_దత్తాంశం),
		   శూన్య *వ్యక్తిగత_దత్తాంశం)
{
	TDB_దత్తాంశం దత్తాంశం;
	పూర్ణాంకం ఫలితం;

	దత్తాంశం.dsize = len;

	ఒకవేళ ((tdb->transaction == ఖాళీ) && (tdb->map_ptr != ఖాళీ)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		ఒకవేళ (tdb->methods->tdb_oob(tdb, స్థానభ్రంశం+len, 0) != 0) {
			తిరిగి -1;
		}
		దత్తాంశం.dptr = స్థానభ్రంశం + (గుర్తులేని అక్షరం *)tdb->map_ptr;
		తిరిగి parser(కీలకం, దత్తాంశం, వ్యక్తిగత_దత్తాంశం);
	}

	ఒకవేళ (!(దత్తాంశం.dptr = tdb_alloc_read(tdb, స్థానభ్రంశం, len))) {
		తిరిగి -1;
	}

	ఫలితం = parser(కీలకం, దత్తాంశం, వ్యక్తిగత_దత్తాంశం);
	విడుదల(దత్తాంశం.dptr);
	తిరిగి ఫలితం;
}

/* read/write a record */
పూర్ణాంకం tdb_rec_read(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, నిర్మాణం list_struct *rec)
{
	ఒకవేళ (tdb->methods->tdb_read(tdb, స్థానభ్రంశం, rec, పరిమాణం(*rec),DOCONV()) == -1)
		తిరిగి -1;
	ఒకవేళ (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, స్థానభ్రంశం));
		తిరిగి TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	తిరిగి tdb->methods->tdb_oob(tdb, rec->తదుపరి+పరిమాణం(*rec), 0);
}

పూర్ణాంకం tdb_rec_write(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, నిర్మాణం list_struct *rec)
{
	నిర్మాణం list_struct r = *rec;
	తిరిగి tdb->methods->tdb_write(tdb, స్థానభ్రంశం, CONVERT(r), పరిమాణం(r));
}

స్థిర స్థిరరాశి నిర్మాణం tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_తదుపరి_hash_chain,
	tdb_oob,
	tdb_expand_file,
	tdb_brlock
};

/*
  initialise the default methods table
*/
శూన్య tdb_io_init(నిర్మాణం tdb_context *tdb)
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

నిర్మాణం tdb_transaction_el {
	నిర్మాణం tdb_transaction_el *తదుపరి, *prev;
	tdb_off_t స్థానభ్రంశం;
	tdb_len_t నిడివి;
	గుర్తులేని అక్షరం *దత్తాంశం;
};

/*
  hold the context of any current transaction
*/
నిర్మాణం tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	స్థిరరాశి నిర్మాణం tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	నిర్మాణం tdb_transaction_el *elements, *elements_చివరి;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	పూర్ణాంకం transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	పూర్ణాంకం nesting;

	/* old file size before transaction */
	tdb_len_t old_map_కొలత;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
స్థిర పూర్ణాంకం transaction_read(నిర్మాణం tdb_context *tdb, tdb_off_t off, శూన్య *buf,
			    tdb_len_t len, పూర్ణాంకం cv)
{
	నిర్మాణం tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	కొరకు (el=tdb->transaction->elements_చివరి;el;el=el->prev) {
		tdb_len_t partial;

		ఒకవేళ (off+len <= el->స్థానభ్రంశం) {
			కొనసాగు;
		}
		ఒకవేళ (off >= el->స్థానభ్రంశం + el->నిడివి) {
			కొనసాగు;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		ఒకవేళ (off < el->స్థానభ్రంశం) {
			partial = el->స్థానభ్రంశం - off;
			ఒకవేళ (transaction_read(tdb, off, buf, partial, cv) != 0) {
				వెళ్ళు fail;
			}
			len -= partial;
			off += partial;
			buf = (శూన్య *)(partial + (అక్షరం *)buf);
		}
		ఒకవేళ (off + len <= el->స్థానభ్రంశం + el->నిడివి) {
			partial = len;
		} లేకపోతే {
			partial = el->స్థానభ్రంశం + el->నిడివి - off;
		}
		జ్ఞాపకం_నకలు(buf, el->దత్తాంశం + (off - el->స్థానభ్రంశం), partial);
		ఒకవేళ (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (శూన్య *)(partial + (అక్షరం *)buf);

		ఒకవేళ (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			వెళ్ళు fail;
		}

		తిరిగి 0;
	}

	/* its not in the transaction elements - do a real read */
	తిరిగి tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	తిరిగి -1;
}


/*
  write while in a transaction
*/
స్థిర పూర్ణాంకం transaction_write(నిర్మాణం tdb_context *tdb, tdb_off_t off,
			     స్థిరరాశి శూన్య *buf, tdb_len_t len)
{
	నిర్మాణం tdb_transaction_el *el, *ఉత్తమ_el=ఖాళీ;

	ఒకవేళ (len == 0) {
		తిరిగి 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	ఒకవేళ (len == పరిమాణం(tdb_off_t) && off >= FREELIST_శిఖరం &&
	    off < FREELIST_శిఖరం+TDB_HASHTABLE_కొలత(tdb)) {
		u32 chain = (off-FREELIST_శిఖరం) / పరిమాణం(tdb_off_t);
		జ్ఞాపకం_నకలు(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	కొరకు (el=tdb->transaction->elements_చివరి;el;el=el->prev) {
		tdb_len_t partial;

		ఒకవేళ (ఉత్తమ_el == ఖాళీ && off == el->స్థానభ్రంశం+el->నిడివి) {
			ఉత్తమ_el = el;
		}

		ఒకవేళ (off+len <= el->స్థానభ్రంశం) {
			కొనసాగు;
		}
		ఒకవేళ (off >= el->స్థానభ్రంశం + el->నిడివి) {
			కొనసాగు;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		ఒకవేళ (off < el->స్థానభ్రంశం) {
			partial = el->స్థానభ్రంశం - off;
			ఒకవేళ (transaction_write(tdb, off, buf, partial) != 0) {
				వెళ్ళు fail;
			}
			len -= partial;
			off += partial;
			buf = (స్థిరరాశి శూన్య *)(partial + (స్థిరరాశి అక్షరం *)buf);
		}
		ఒకవేళ (off + len <= el->స్థానభ్రంశం + el->నిడివి) {
			partial = len;
		} లేకపోతే {
			partial = el->స్థానభ్రంశం + el->నిడివి - off;
		}
		జ్ఞాపకం_నకలు(el->దత్తాంశం + (off - el->స్థానభ్రంశం), buf, partial);
		len -= partial;
		off += partial;
		buf = (స్థిరరాశి శూన్య *)(partial + (స్థిరరాశి అక్షరం *)buf);

		ఒకవేళ (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			వెళ్ళు fail;
		}

		తిరిగి 0;
	}

	/* see if we can append the new entry to an existing entry */
	ఒకవేళ (ఉత్తమ_el && ఉత్తమ_el->స్థానభ్రంశం + ఉత్తమ_el->నిడివి == off &&
	    (off+len < tdb->transaction->old_map_కొలత ||
	     off > tdb->transaction->old_map_కొలత)) {
		గుర్తులేని అక్షరం *దత్తాంశం = ఉత్తమ_el->దత్తాంశం;
		el = ఉత్తమ_el;
		el->దత్తాంశం = (గుర్తులేని అక్షరం *)realloc(el->దత్తాంశం,
						    el->నిడివి + len);
		ఒకవేళ (el->దత్తాంశం == ఖాళీ) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->దత్తాంశం = దత్తాంశం;
			తిరిగి -1;
		}
		ఒకవేళ (buf) {
			జ్ఞాపకం_నకలు(el->దత్తాంశం + el->నిడివి, buf, len);
		} లేకపోతే {
			memset(el->దత్తాంశం + el->నిడివి, TDB_PAD_BYTE, len);
		}
		el->నిడివి += len;
		తిరిగి 0;
	}

	/* add a new entry at the end of the list */
	el = (నిర్మాణం tdb_transaction_el *)జ్ఞాపకం_ఇవ్వు(పరిమాణం(*el));
	ఒకవేళ (el == ఖాళీ) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		తిరిగి -1;
	}
	el->తదుపరి = ఖాళీ;
	el->prev = tdb->transaction->elements_చివరి;
	el->స్థానభ్రంశం = off;
	el->నిడివి = len;
	el->దత్తాంశం = (గుర్తులేని అక్షరం *)జ్ఞాపకం_ఇవ్వు(len);
	ఒకవేళ (el->దత్తాంశం == ఖాళీ) {
		విడుదల(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		తిరిగి -1;
	}
	ఒకవేళ (buf) {
		జ్ఞాపకం_నకలు(el->దత్తాంశం, buf, len);
	} లేకపోతే {
		memset(el->దత్తాంశం, TDB_PAD_BYTE, len);
	}
	ఒకవేళ (el->prev) {
		el->prev->తదుపరి = el;
	} లేకపోతే {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_చివరి = el;
	తిరిగి 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	తిరిగి -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
స్థిర శూన్య transaction_తదుపరి_hash_chain(నిర్మాణం tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	కొరకు (;h < tdb->శీర్షిక.hash_కొలత;h++) {
		/* the +1 takes account of the freelist */
		ఒకవేళ (0 != tdb->transaction->hash_heads[h+1]) {
			ఆపు;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
స్థిర పూర్ణాంకం transaction_oob(నిర్మాణం tdb_context *tdb, tdb_off_t len,
			   పూర్ణాంకం probe EXT2FS_ATTR((వాడని)))
{
	ఒకవేళ (len <= tdb->map_కొలత) {
		తిరిగి 0;
	}
	తిరిగి TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
స్థిర పూర్ణాంకం transaction_expand_file(నిర్మాణం tdb_context *tdb, tdb_off_t కొలత,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	ఒకవేళ (transaction_write(tdb, కొలత, ఖాళీ, addition) != 0) {
		తిరిగి -1;
	}

	తిరిగి 0;
}

/*
  brlock during a transaction - ignore them
*/
స్థిర పూర్ణాంకం transaction_brlock(నిర్మాణం tdb_context *tdb EXT2FS_ATTR((వాడని)),
			      tdb_off_t స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			      పూర్ణాంకం rw_type EXT2FS_ATTR((వాడని)),
			      పూర్ణాంకం lck_type EXT2FS_ATTR((వాడని)),
			      పూర్ణాంకం probe EXT2FS_ATTR((వాడని)),
			      size_t len EXT2FS_ATTR((వాడని)))
{
	తిరిగి 0;
}

స్థిర స్థిరరాశి నిర్మాణం tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_తదుపరి_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
పూర్ణాంకం tdb_transaction_ప్రారంభం(నిర్మాణం tdb_context *tdb)
{
	/* some sanity checks */
	ఒకవేళ (tdb->read_only || (tdb->flags & TDB_అంతర్గత) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		తిరిగి -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	ఒకవేళ (tdb->transaction != ఖాళీ) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		తిరిగి 0;
	}

	ఒకవేళ (tdb->అంకె_locks != 0 || tdb->global_తాళం.లెక్క) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_తాళం;
		తిరిగి -1;
	}

	ఒకవేళ (tdb->travlocks.తదుపరి != ఖాళీ) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_తాళం;
		తిరిగి -1;
	}

	tdb->transaction = (నిర్మాణం tdb_transaction *)
		సున్నా_జ్ఞాపకం_ఇవ్వు(పరిమాణం(నిర్మాణం tdb_transaction), 1);
	ఒకవేళ (tdb->transaction == ఖాళీ) {
		tdb->ecode = TDB_ERR_OOM;
		తిరిగి -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	ఒకవేళ (tdb_transaction_తాళం(tdb, F_WRLCK) == -1) {
		సురక్షిత_FREE(tdb->transaction);
		తిరిగి -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	ఒకవేళ (tdb_brlock(tdb, FREELIST_శిఖరం, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_తాళం;
		వెళ్ళు fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		సున్నా_జ్ఞాపకం_ఇవ్వు(tdb->శీర్షిక.hash_కొలత+1, పరిమాణం(u32));
	ఒకవేళ (tdb->transaction->hash_heads == ఖాళీ) {
		tdb->ecode = TDB_ERR_OOM;
		వెళ్ళు fail;
	}
	ఒకవేళ (tdb->methods->tdb_read(tdb, FREELIST_శిఖరం, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_కొలత(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		వెళ్ళు fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_కొలత + 1, 1);
	tdb->transaction->old_map_కొలత = tdb->map_కొలత;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	ఒకవేళ (transaction_write(tdb, FREELIST_శిఖరం, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_కొలత(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		వెళ్ళు fail;
	}

	తిరిగి 0;

fail:
	tdb_brlock(tdb, FREELIST_శిఖరం, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	సురక్షిత_FREE(tdb->transaction->hash_heads);
	సురక్షిత_FREE(tdb->transaction);
	తిరిగి -1;
}


/*
  cancel the current transaction
*/
పూర్ణాంకం tdb_transaction_cancel(నిర్మాణం tdb_context *tdb)
{
	ఒకవేళ (tdb->transaction == ఖాళీ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		తిరిగి -1;
	}

	ఒకవేళ (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		తిరిగి 0;
	}

	tdb->map_కొలత = tdb->transaction->old_map_కొలత;

	/* free all the transaction elements */
	అయితే (tdb->transaction->elements) {
		నిర్మాణం tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->తదుపరి;
		విడుదల(el->దత్తాంశం);
		విడుదల(el);
	}

	/* remove any global lock created during the transaction */
	ఒకవేళ (tdb->global_తాళం.లెక్క != 0) {
		tdb_brlock(tdb, FREELIST_శిఖరం, F_UNLCK, F_SETLKW, 0, 4*tdb->శీర్షిక.hash_కొలత);
		tdb->global_తాళం.లెక్క = 0;
	}

	/* remove any locks created during the transaction */
	ఒకవేళ (tdb->అంకె_locks != 0) {
		పూర్ణాంకం i;
		కొరకు (i=0;i<tdb->అంకె_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_శిఖరం+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->అంకె_locks = 0;
		tdb->అంకె_lockrecs = 0;
		సురక్షిత_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_శిఖరం, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	సురక్షిత_FREE(tdb->transaction->hash_heads);
	సురక్షిత_FREE(tdb->transaction);

	తిరిగి 0;
}

/*
  sync to disk
*/
స్థిర పూర్ణాంకం transaction_sync(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, tdb_len_t నిడివి)
{
	ఒకవేళ (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		తిరిగి -1;
	}
#ఒకవేళ defined(HAVE_MSYNC) && defined(MS_SYNC)
	ఒకవేళ (tdb->map_ptr) {
		tdb_off_t moffset = స్థానభ్రంశం & ~(tdb->పుట_కొలత-1);
		ఒకవేళ (msync(moffset + (అక్షరం *)tdb->map_ptr,
			  నిడివి + (స్థానభ్రంశం - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			తిరిగి -1;
		}
	}
#అయితే_ముగింపు
	తిరిగి 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
స్థిర tdb_len_t tdb_recovery_కొలత(నిర్మాణం tdb_context *tdb)
{
	నిర్మాణం tdb_transaction_el *el;
	tdb_len_t recovery_కొలత = 0;

	recovery_కొలత = పరిమాణం(u32);
	కొరకు (el=tdb->transaction->elements;el;el=el->తదుపరి) {
		ఒకవేళ (el->స్థానభ్రంశం >= tdb->transaction->old_map_కొలత) {
			కొనసాగు;
		}
		recovery_కొలత += 2*పరిమాణం(tdb_off_t) + el->నిడివి;
	}

	తిరిగి recovery_కొలత;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
స్థిర పూర్ణాంకం tdb_recovery_allocate(నిర్మాణం tdb_context *tdb,
				 tdb_len_t *recovery_కొలత,
				 tdb_off_t *recovery_స్థానభ్రంశం,
				 tdb_len_t *recovery_max_కొలత)
{
	నిర్మాణం list_struct rec;
	స్థిరరాశి నిర్మాణం tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_తల;

	ఒకవేళ (tdb_ofs_read(tdb, TDB_RECOVERY_తల, &recovery_తల) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		తిరిగి -1;
	}

	rec.rec_len = 0;

	ఒకవేళ (recovery_తల != 0 &&
	    methods->tdb_read(tdb, recovery_తల, &rec, పరిమాణం(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		తిరిగి -1;
	}

	*recovery_కొలత = tdb_recovery_కొలత(tdb);

	ఒకవేళ (recovery_తల != 0 && *recovery_కొలత <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_కొలత = rec.rec_len;
		*recovery_స్థానభ్రంశం = recovery_తల;
		తిరిగి 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	ఒకవేళ (recovery_తల != 0) {
		ఒకవేళ (tdb_free(tdb, recovery_తల, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			తిరిగి -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_కొలత = tdb_recovery_కొలత(tdb);

	/* round up to a multiple of page size */
	*recovery_max_కొలత = TDB_ALIGN(పరిమాణం(rec) + *recovery_కొలత, tdb->పుట_కొలత) - పరిమాణం(rec);
	*recovery_స్థానభ్రంశం = tdb->map_కొలత;
	recovery_తల = *recovery_స్థానభ్రంశం;

	ఒకవేళ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_కొలత,
				     (tdb->map_కొలత - tdb->transaction->old_map_కొలత) +
				     పరిమాణం(rec) + *recovery_max_కొలత) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		తిరిగి -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_కొలత + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_కొలత = tdb->map_కొలత;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_తల);
	ఒకవేళ (methods->tdb_write(tdb, TDB_RECOVERY_తల,
			       &recovery_తల, పరిమాణం(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		తిరిగి -1;
	}

	తిరిగి 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
స్థిర పూర్ణాంకం transaction_అమరిక_recovery(నిర్మాణం tdb_context *tdb,
				      tdb_off_t *magic_స్థానభ్రంశం)
{
	నిర్మాణం tdb_transaction_el *el;
	tdb_len_t recovery_కొలత;
	గుర్తులేని అక్షరం *దత్తాంశం, *p;
	స్థిరరాశి నిర్మాణం tdb_methods *methods = tdb->transaction->io_methods;
	నిర్మాణం list_struct *rec;
	tdb_off_t recovery_స్థానభ్రంశం, recovery_max_కొలత;
	tdb_off_t old_map_కొలత = tdb->transaction->old_map_కొలత;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	ఒకవేళ (tdb_recovery_allocate(tdb, &recovery_కొలత,
				  &recovery_స్థానభ్రంశం, &recovery_max_కొలత) == -1) {
		తిరిగి -1;
	}

	దత్తాంశం = (గుర్తులేని అక్షరం *)జ్ఞాపకం_ఇవ్వు(recovery_కొలత + పరిమాణం(*rec));
	ఒకవేళ (దత్తాంశం == ఖాళీ) {
		tdb->ecode = TDB_ERR_OOM;
		తిరిగి -1;
	}

	rec = (నిర్మాణం list_struct *)దత్తాంశం;
	memset(rec, 0, పరిమాణం(*rec));

	rec->magic    = 0;
	rec->దత్తాంశం_len = recovery_కొలత;
	rec->rec_len  = recovery_max_కొలత;
	rec->కీలకం_len  = old_map_కొలత;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = దత్తాంశం + పరిమాణం(*rec);
	కొరకు (el=tdb->transaction->elements;el;el=el->తదుపరి) {
		ఒకవేళ (el->స్థానభ్రంశం >= old_map_కొలత) {
			కొనసాగు;
		}
		ఒకవేళ (el->స్థానభ్రంశం + el->నిడివి > tdb->transaction->old_map_కొలత) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			విడుదల(దత్తాంశం);
			tdb->ecode = TDB_ERR_CORRUPT;
			తిరిగి -1;
		}
		జ్ఞాపకం_నకలు(p, &el->స్థానభ్రంశం, 4);
		జ్ఞాపకం_నకలు(p+4, &el->నిడివి, 4);
		ఒకవేళ (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		ఒకవేళ (methods->tdb_read(tdb, el->స్థానభ్రంశం, p + 8, el->నిడివి, 0) != 0) {
			విడుదల(దత్తాంశం);
			tdb->ecode = TDB_ERR_IO;
			తిరిగి -1;
		}
		p += 8 + el->నిడివి;
	}

	/* and the tailer */
	tailer = పరిమాణం(*rec) + recovery_max_కొలత;
	జ్ఞాపకం_నకలు(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	ఒకవేళ (methods->tdb_write(tdb, recovery_స్థానభ్రంశం, దత్తాంశం, పరిమాణం(*rec) + recovery_కొలత) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		విడుదల(దత్తాంశం);
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	ఒకవేళ (transaction_sync(tdb, recovery_స్థానభ్రంశం, పరిమాణం(*rec) + recovery_కొలత) == -1) {
		విడుదల(దత్తాంశం);
		తిరిగి -1;
	}

	విడుదల(దత్తాంశం);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_స్థానభ్రంశం = recovery_స్థానభ్రంశం + offsetof(నిర్మాణం list_struct, magic);

	ఒకవేళ (methods->tdb_write(tdb, *magic_స్థానభ్రంశం, &magic, పరిమాణం(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	/* ensure the recovery magic marker is on disk */
	ఒకవేళ (transaction_sync(tdb, *magic_స్థానభ్రంశం, పరిమాణం(magic)) == -1) {
		తిరిగి -1;
	}

	తిరిగి 0;
}

/*
  commit the current transaction
*/
పూర్ణాంకం tdb_transaction_commit(నిర్మాణం tdb_context *tdb)
{
	స్థిరరాశి నిర్మాణం tdb_methods *methods;
	tdb_off_t magic_స్థానభ్రంశం = 0;
	u32 zero = 0;

	ఒకవేళ (tdb->transaction == ఖాళీ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		తిరిగి -1;
	}

	ఒకవేళ (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		తిరిగి -1;
	}

	ఒకవేళ (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		తిరిగి 0;
	}

	/* check for a null transaction */
	ఒకవేళ (tdb->transaction->elements == ఖాళీ) {
		tdb_transaction_cancel(tdb);
		తిరిగి 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	ఒకవేళ (tdb->అంకె_locks || tdb->global_తాళం.లెక్క) {
		tdb->ecode = TDB_ERR_తాళం;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		తిరిగి -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	ఒకవేళ (tdb_brlock_upgrade(tdb, FREELIST_శిఖరం, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_తాళం;
		tdb_transaction_cancel(tdb);
		తిరిగి -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	ఒకవేళ (tdb_brlock(tdb, GLOBAL_తాళం, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_తాళం;
		tdb_transaction_cancel(tdb);
		తిరిగి -1;
	}

	ఒకవేళ (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		ఒకవేళ (transaction_అమరిక_recovery(tdb, &magic_స్థానభ్రంశం) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_తాళం, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			తిరిగి -1;
		}
	}

	/* expand the file to the new size if needed */
	ఒకవేళ (tdb->map_కొలత != tdb->transaction->old_map_కొలత) {
		ఒకవేళ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_కొలత,
					     tdb->map_కొలత -
					     tdb->transaction->old_map_కొలత) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_తాళం, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			తిరిగి -1;
		}
		tdb->map_కొలత = tdb->transaction->old_map_కొలత;
		methods->tdb_oob(tdb, tdb->map_కొలత + 1, 1);
	}

	/* perform all the writes */
	అయితే (tdb->transaction->elements) {
		నిర్మాణం tdb_transaction_el *el = tdb->transaction->elements;

		ఒకవేళ (methods->tdb_write(tdb, el->స్థానభ్రంశం, el->దత్తాంశం, el->నిడివి) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_తాళం, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			తిరిగి -1;
		}
		tdb->transaction->elements = el->తదుపరి;
		విడుదల(el->దత్తాంశం);
		విడుదల(el);
	}

	ఒకవేళ (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		ఒకవేళ (transaction_sync(tdb, 0, tdb->map_కొలత) == -1) {
			తిరిగి -1;
		}

		/* remove the recovery marker */
		ఒకవేళ (methods->tdb_write(tdb, magic_స్థానభ్రంశం, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			తిరిగి -1;
		}

		/* ensure the recovery marker has been removed on disk */
		ఒకవేళ (transaction_sync(tdb, magic_స్థానభ్రంశం, 4) == -1) {
			తిరిగి -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_తాళం, F_UNLCK, F_SETLKW, 0, 1);

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
#నిర్వచితమైతే HAVE_UTIME
	utime(tdb->name, ఖాళీ);
#అయితే_ముగింపు

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	తిరిగి 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
పూర్ణాంకం tdb_transaction_recover(నిర్మాణం tdb_context *tdb)
{
	tdb_off_t recovery_తల, recovery_eof;
	గుర్తులేని అక్షరం *దత్తాంశం, *p;
	u32 zero = 0;
	నిర్మాణం list_struct rec;

	/* find the recovery area */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_RECOVERY_తల, &recovery_తల) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	ఒకవేళ (recovery_తల == 0) {
		/* we have never allocated a recovery record */
		తిరిగి 0;
	}

	/* read the recovery record */
	ఒకవేళ (tdb->methods->tdb_read(tdb, recovery_తల, &rec,
				   పరిమాణం(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	ఒకవేళ (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		తిరిగి 0;
	}

	ఒకవేళ (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		తిరిగి -1;
	}

	recovery_eof = rec.కీలకం_len;

	దత్తాంశం = (గుర్తులేని అక్షరం *)జ్ఞాపకం_ఇవ్వు(rec.దత్తాంశం_len);
	ఒకవేళ (దత్తాంశం == ఖాళీ) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		తిరిగి -1;
	}

	/* read the full recovery data */
	ఒకవేళ (tdb->methods->tdb_read(tdb, recovery_తల + పరిమాణం(rec), దత్తాంశం,
				   rec.దత్తాంశం_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		విడుదల(దత్తాంశం);
		తిరిగి -1;
	}

	/* recover the file data */
	p = దత్తాంశం;
	అయితే (p+8 < దత్తాంశం + rec.దత్తాంశం_len) {
		u32 ofs, len;
		ఒకవేళ (DOCONV()) {
			tdb_convert(p, 8);
		}
		జ్ఞాపకం_నకలు(&ofs, p, 4);
		జ్ఞాపకం_నకలు(&len, p+4, 4);

		ఒకవేళ (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			విడుదల(దత్తాంశం);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			తిరిగి -1;
		}
		p += 8 + len;
	}

	విడుదల(దత్తాంశం);

	ఒకవేళ (transaction_sync(tdb, 0, tdb->map_కొలత) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	ఒకవేళ (recovery_eof <= recovery_తల) {
		ఒకవేళ (tdb_ofs_write(tdb, TDB_RECOVERY_తల, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			తిరిగి -1;
		}
	}

	/* remove the recovery magic */
	ఒకవేళ (tdb_ofs_write(tdb, recovery_తల + offsetof(నిర్మాణం list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	ఒకవేళ (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}
	tdb->map_కొలత = recovery_eof;
	tdb_mmap(tdb);

	ఒకవేళ (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		తిరిగి -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	తిరిగి 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
స్థిర పూర్ణాంకం tdb_rec_free_read(నిర్మాణం tdb_context *tdb, tdb_off_t off, నిర్మాణం list_struct *rec)
{
	ఒకవేళ (tdb->methods->tdb_read(tdb, off, rec, పరిమాణం(*rec),DOCONV()) == -1)
		తిరిగి -1;

	ఒకవేళ (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		ఒకవేళ (tdb->methods->tdb_write(tdb, off, rec, పరిమాణం(*rec)) == -1)
			తిరిగి -1;
	}

	ఒకవేళ (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		తిరిగి TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ఒకవేళ (tdb->methods->tdb_oob(tdb, rec->తదుపరి+పరిమాణం(*rec), 0) != 0)
		తిరిగి -1;
	తిరిగి 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
స్థిర పూర్ణాంకం remove_from_freelist(నిర్మాణం tdb_context *tdb, tdb_off_t off, tdb_off_t తదుపరి)
{
	tdb_off_t చివరి_ptr, i;

	/* read in the freelist top */
	చివరి_ptr = FREELIST_శిఖరం;
	అయితే (tdb_ofs_read(tdb, చివరి_ptr, &i) != -1 && i != 0) {
		ఒకవేళ (i == off) {
			/* We've found it! */
			తిరిగి tdb_ofs_write(tdb, చివరి_ptr, &తదుపరి);
		}
		/* Follow chain (next offset is at start of record) */
		చివరి_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	తిరిగి TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
స్థిర పూర్ణాంకం update_tailer(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం,
			 స్థిరరాశి నిర్మాణం list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = పరిమాణం(*rec) + rec->rec_len;
	తిరిగి tdb_ofs_write(tdb, స్థానభ్రంశం + totalsize - పరిమాణం(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
పూర్ణాంకం tdb_free(నిర్మాణం tdb_context *tdb, tdb_off_t స్థానభ్రంశం, నిర్మాణం list_struct *rec)
{
	tdb_off_t కుడి, ఎడమ;

	/* Allocation and tailer lock */
	ఒకవేళ (tdb_తాళం(tdb, -1, F_WRLCK) != 0)
		తిరిగి -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	ఒకవేళ (update_tailer(tdb, స్థానభ్రంశం, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		వెళ్ళు fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	కుడి = స్థానభ్రంశం + పరిమాణం(*rec) + rec->rec_len;
	ఒకవేళ (కుడి + పరిమాణం(*rec) <= tdb->map_కొలత) {
		నిర్మాణం list_struct r;

		ఒకవేళ (tdb->methods->tdb_read(tdb, కుడి, &r, పరిమాణం(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", కుడి));
			వెళ్ళు ఎడమ;
		}

		/* If it's free, expand to include it. */
		ఒకవేళ (r.magic == TDB_FREE_MAGIC) {
			ఒకవేళ (remove_from_freelist(tdb, కుడి, r.తదుపరి) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", కుడి));
				వెళ్ళు ఎడమ;
			}
			rec->rec_len += పరిమాణం(r) + r.rec_len;
		}
	}

ఎడమ:
	/* Look left */
	ఎడమ = స్థానభ్రంశం - పరిమాణం(tdb_off_t);
	ఒకవేళ (ఎడమ > TDB_దత్తాంశం_ప్రారంభం(tdb->శీర్షిక.hash_కొలత)) {
		నిర్మాణం list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		ఒకవేళ (tdb_ofs_read(tdb, ఎడమ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ఎడమ));
			వెళ్ళు update;
		}

		/* it could be uninitialised data */
		ఒకవేళ (leftsize == 0 || leftsize == TDB_PAD_U32) {
			వెళ్ళు update;
		}

		ఎడమ = స్థానభ్రంశం - leftsize;

		/* Now read in record */
		ఒకవేళ (tdb->methods->tdb_read(tdb, ఎడమ, &l, పరిమాణం(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ఎడమ, leftsize));
			వెళ్ళు update;
		}

		/* If it's free, expand to include it. */
		ఒకవేళ (l.magic == TDB_FREE_MAGIC) {
			ఒకవేళ (remove_from_freelist(tdb, ఎడమ, l.తదుపరి) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ఎడమ));
				వెళ్ళు update;
			} లేకపోతే {
				స్థానభ్రంశం = ఎడమ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	ఒకవేళ (update_tailer(tdb, స్థానభ్రంశం, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", స్థానభ్రంశం));
		వెళ్ళు fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	ఒకవేళ (tdb_ofs_read(tdb, FREELIST_శిఖరం, &rec->తదుపరి) == -1 ||
	    tdb_rec_write(tdb, స్థానభ్రంశం, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_శిఖరం, &స్థానభ్రంశం) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", స్థానభ్రంశం));
		వెళ్ళు fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
స్థిర tdb_off_t tdb_allocate_ofs(నిర్మాణం tdb_context *tdb, tdb_len_t నిడివి, tdb_off_t rec_ptr,
				నిర్మాణం list_struct *rec, tdb_off_t చివరి_ptr)
{
	నిర్మాణం list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', పరిమాణం(newrec));

	/* found it - now possibly split it up  */
	ఒకవేళ (rec->rec_len > నిడివి + MIN_REC_కొలత) {
		/* Length of left piece */
		నిడివి = TDB_ALIGN(నిడివి, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (పరిమాణం(*rec) + నిడివి);
		newrec_ptr = rec_ptr + పరిమాణం(*rec) + నిడివి;

		/* And left record is shortened */
		rec->rec_len = నిడివి;
	} లేకపోతే {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	ఒకవేళ (tdb_ofs_write(tdb, చివరి_ptr, &rec->తదుపరి) == -1) {
		తిరిగి 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	ఒకవేళ (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		తిరిగి 0;
	}

	/* Did we create new block? */
	ఒకవేళ (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		ఒకవేళ (update_tailer(tdb, rec_ptr, rec) == -1) {
			తిరిగి 0;
		}

		/* Free new record */
		ఒకవేళ (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			తిరిగి 0;
		}
	}

	/* all done - return the new record offset */
	తిరిగి rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(నిర్మాణం tdb_context *tdb, tdb_len_t నిడివి, నిర్మాణం list_struct *rec)
{
	tdb_off_t rec_ptr, చివరి_ptr, newrec_ptr;
	నిర్మాణం {
		tdb_off_t rec_ptr, చివరి_ptr;
		tdb_len_t rec_len;
	} bestfit;

	ఒకవేళ (tdb_తాళం(tdb, -1, F_WRLCK) == -1)
		తిరిగి 0;

	/* Extra bytes required for tailer */
	నిడివి += పరిమాణం(tdb_off_t);

 again:
	చివరి_ptr = FREELIST_శిఖరం;

	/* read in the freelist top */
	ఒకవేళ (tdb_ofs_read(tdb, FREELIST_శిఖరం, &rec_ptr) == -1)
		వెళ్ళు fail;

	bestfit.rec_ptr = 0;
	bestfit.చివరి_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	అయితే (rec_ptr) {
		ఒకవేళ (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			వెళ్ళు fail;
		}

		ఒకవేళ (rec->rec_len >= నిడివి) {
			ఒకవేళ (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.చివరి_ptr = చివరి_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				ఒకవేళ (bestfit.rec_len < 2*నిడివి) {
					ఆపు;
				}
			}
		}

		/* move to the next record */
		చివరి_ptr = rec_ptr;
		rec_ptr = rec->తదుపరి;
	}

	ఒకవేళ (bestfit.rec_ptr != 0) {
		ఒకవేళ (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			వెళ్ళు fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, నిడివి, bestfit.rec_ptr, rec, bestfit.చివరి_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		తిరిగి newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	ఒకవేళ (tdb_expand(tdb, నిడివి + పరిమాణం(*rec)) == 0)
		వెళ్ళు again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

స్థిర పూర్ణాంకం చూసిన_చొప్పించు(నిర్మాణం tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_దత్తాంశం కీలకం, దత్తాంశం;

	memset(&దత్తాంశం, '\0', పరిమాణం(దత్తాంశం));
	కీలకం.dptr = (గుర్తులేని అక్షరం *)&rec_ptr;
	కీలకం.dsize = పరిమాణం(rec_ptr);
	తిరిగి tdb_store(mem_tdb, కీలకం, దత్తాంశం, TDB_చొప్పించు);
}

పూర్ణాంకం tdb_validate_freelist(నిర్మాణం tdb_context *tdb, పూర్ణాంకం *pnum_entries)
{
	నిర్మాణం tdb_context *mem_tdb = ఖాళీ;
	నిర్మాణం list_struct rec;
	tdb_off_t rec_ptr, చివరి_ptr;
	పూర్ణాంకం ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->శీర్షిక.hash_కొలత,
				TDB_అంతర్గత, O_RDWR, 0600);
	ఒకవేళ (!mem_tdb) {
		తిరిగి -1;
	}

	ఒకవేళ (tdb_తాళం(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		తిరిగి 0;
	}

	చివరి_ptr = FREELIST_శిఖరం;

	/* Store the FREELIST_TOP record. */
	ఒకవేళ (చూసిన_చొప్పించు(mem_tdb, చివరి_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		వెళ్ళు fail;
	}

	/* read in the freelist top */
	ఒకవేళ (tdb_ofs_read(tdb, FREELIST_శిఖరం, &rec_ptr) == -1) {
		వెళ్ళు fail;
	}

	అయితే (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		ఒకవేళ (చూసిన_చొప్పించు(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			వెళ్ళు fail;
		}

		ఒకవేళ (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			వెళ్ళు fail;
		}

		/* move to the next record */
		చివరి_ptr = rec_ptr;
		rec_ptr = rec.తదుపరి;
		*pnum_entries += 1;
	}

	ret = 0;

  fail:

	tdb_close(mem_tdb);
	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
స్థిర పూర్ణాంకం tdb_తదుపరి_తాళం(నిర్మాణం tdb_context *tdb, నిర్మాణం tdb_traverse_తాళం *tlock,
			 నిర్మాణం list_struct *rec)
{
	పూర్ణాంకం want_తదుపరి = (tlock->off != 0);

	/* Lock each chain from the start one. */
	కొరకు (; tlock->hash < tdb->శీర్షిక.hash_కొలత; tlock->hash++) {
		ఒకవేళ (!tlock->off && tlock->hash != 0) {
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
			tdb->methods->తదుపరి_hash_chain(tdb, &tlock->hash);
			ఒకవేళ (tlock->hash == tdb->శీర్షిక.hash_కొలత) {
				కొనసాగు;
			}
		}

		ఒకవేళ (tdb_తాళం(tdb, tlock->hash, tlock->తాళం_rw) == -1)
			తిరిగి -1;

		/* No previous record?  Start at top of chain. */
		ఒకవేళ (!tlock->off) {
			ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(tlock->hash),
				     &tlock->off) == -1)
				వెళ్ళు fail;
		} లేకపోతే {
			/* Otherwise unlock the previous record. */
			ఒకవేళ (tdb_unlock_రికార్డు(tdb, tlock->off) != 0)
				వెళ్ళు fail;
		}

		ఒకవేళ (want_తదుపరి) {
			/* We have offset of old record: grab next */
			ఒకవేళ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				వెళ్ళు fail;
			tlock->off = rec->తదుపరి;
		}

		/* Iterate through chain */
		అయితే( tlock->off) {
			tdb_off_t current;
			ఒకవేళ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				వెళ్ళు fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			ఒకవేళ (tlock->off == rec->తదుపరి) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				వెళ్ళు fail;
			}

			ఒకవేళ (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				ఒకవేళ (tdb_తాళం_రికార్డు(tdb, tlock->off) != 0)
					వెళ్ళు fail;
				తిరిగి tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->తదుపరి;
			ఒకవేళ (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				వెళ్ళు fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->తాళం_rw);
		want_తదుపరి = 0;
	}
	/* We finished iteration without finding anything */
	తిరిగి TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	ఒకవేళ (tdb_unlock(tdb, tlock->hash, tlock->తాళం_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	తిరిగి -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
స్థిర పూర్ణాంకం tdb_traverse_అంతర్గత(నిర్మాణం tdb_context *tdb,
				 tdb_traverse_func fn, శూన్య *వ్యక్తిగత_దత్తాంశం,
				 నిర్మాణం tdb_traverse_తాళం *tl)
{
	TDB_దత్తాంశం కీలకం, dbuf;
	నిర్మాణం list_struct rec;
	పూర్ణాంకం ret, లెక్క = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->తదుపరి = tdb->travlocks.తదుపరి;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.తదుపరి = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	అయితే ((ret = tdb_తదుపరి_తాళం(tdb, tl, &rec)) > 0) {
		లెక్క++;
		/* now read the full record */
		కీలకం.dptr = tdb_alloc_read(tdb, tl->off + పరిమాణం(rec),
					  rec.కీలకం_len + rec.దత్తాంశం_len);
		ఒకవేళ (!కీలకం.dptr) {
			ret = -1;
			ఒకవేళ (tdb_unlock(tdb, tl->hash, tl->తాళం_rw) != 0)
				వెళ్ళు బహిర్గతం;
			ఒకవేళ (tdb_unlock_రికార్డు(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			వెళ్ళు బహిర్గతం;
		}
		కీలకం.dsize = rec.కీలకం_len;
		dbuf.dptr = కీలకం.dptr + rec.కీలకం_len;
		dbuf.dsize = rec.దత్తాంశం_len;

		/* Drop chain lock, call out */
		ఒకవేళ (tdb_unlock(tdb, tl->hash, tl->తాళం_rw) != 0) {
			ret = -1;
			సురక్షిత_FREE(కీలకం.dptr);
			వెళ్ళు బహిర్గతం;
		}
		ఒకవేళ (fn && fn(tdb, కీలకం, dbuf, వ్యక్తిగత_దత్తాంశం)) {
			/* They want us to terminate traversal */
			ret = లెక్క;
			ఒకవేళ (tdb_unlock_రికార్డు(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			సురక్షిత_FREE(కీలకం.dptr);
			వెళ్ళు బహిర్గతం;
		}
		సురక్షిత_FREE(కీలకం.dptr);
	}
బహిర్గతం:
	tdb->travlocks.తదుపరి = tl->తదుపరి;
	ఒకవేళ (ret < 0)
		తిరిగి -1;
	లేకపోతే
		తిరిగి లెక్క;
}


/*
  a write style traverse - temporarily marks the db read only
*/
పూర్ణాంకం tdb_traverse_read(నిర్మాణం tdb_context *tdb,
		      tdb_traverse_func fn, శూన్య *వ్యక్తిగత_దత్తాంశం)
{
	నిర్మాణం tdb_traverse_తాళం tl = { ఖాళీ, 0, 0, F_RDLCK };
	పూర్ణాంకం ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	ఒకవేళ (tdb_transaction_తాళం(tdb, F_RDLCK)) {
		తిరిగి -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_అంతర్గత(tdb, fn, వ్యక్తిగత_దత్తాంశం, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	తిరిగి ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
పూర్ణాంకం tdb_traverse(నిర్మాణం tdb_context *tdb,
		 tdb_traverse_func fn, శూన్య *వ్యక్తిగత_దత్తాంశం)
{
	నిర్మాణం tdb_traverse_తాళం tl = { ఖాళీ, 0, 0, F_WRLCK };
	పూర్ణాంకం ret;

	ఒకవేళ (tdb->read_only || tdb->traverse_read) {
		తిరిగి tdb_traverse_read(tdb, fn, వ్యక్తిగత_దత్తాంశం);
	}

	ఒకవేళ (tdb_transaction_తాళం(tdb, F_WRLCK)) {
		తిరిగి -1;
	}

	ret = tdb_traverse_అంతర్గత(tdb, fn, వ్యక్తిగత_దత్తాంశం, &tl);

	tdb_transaction_unlock(tdb);

	తిరిగి ret;
}


/* find the first entry in the database and return its key */
TDB_దత్తాంశం tdb_firstkey(నిర్మాణం tdb_context *tdb)
{
	TDB_దత్తాంశం కీలకం;
	నిర్మాణం list_struct rec;

	/* release any old lock */
	ఒకవేళ (tdb_unlock_రికార్డు(tdb, tdb->travlocks.off) != 0)
		తిరిగి tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.తాళం_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	ఒకవేళ (tdb_తదుపరి_తాళం(tdb, &tdb->travlocks, &rec) <= 0)
		తిరిగి tdb_null;
	/* now read the key */
	కీలకం.dsize = rec.కీలకం_len;
	కీలకం.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+పరిమాణం(rec),కీలకం.dsize);

	/* Unlock the hash chain of the record we just read. */
	ఒకవేళ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.తాళం_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	తిరిగి కీలకం;
}

/* find the next entry in the database, returning its key */
TDB_దత్తాంశం tdb_nextkey(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం oldkey)
{
	u32 oldhash;
	TDB_దత్తాంశం కీలకం = tdb_null;
	నిర్మాణం list_struct rec;
	గుర్తులేని అక్షరం *k = ఖాళీ;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	ఒకవేళ (tdb->travlocks.off) {
		ఒకవేళ (tdb_తాళం(tdb,tdb->travlocks.hash,tdb->travlocks.తాళం_rw))
			తిరిగి tdb_null;
		ఒకవేళ (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+పరిమాణం(rec),
					    rec.కీలకం_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			ఒకవేళ (tdb_unlock_రికార్డు(tdb, tdb->travlocks.off) != 0) {
				సురక్షిత_FREE(k);
				తిరిగి tdb_null;
			}
			ఒకవేళ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.తాళం_rw) != 0) {
				సురక్షిత_FREE(k);
				తిరిగి tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		సురక్షిత_FREE(k);
	}

	ఒకవేళ (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_కనుగొను_తాళం_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.తాళం_rw, &rec);
		ఒకవేళ (!tdb->travlocks.off)
			తిరిగి tdb_null;
		tdb->travlocks.hash = పాత్ర(rec.నిండిన_hash);
		ఒకవేళ (tdb_తాళం_రికార్డు(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			తిరిగి tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	ఒకవేళ (tdb_తదుపరి_తాళం(tdb, &tdb->travlocks, &rec) > 0) {
		కీలకం.dsize = rec.కీలకం_len;
		కీలకం.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+పరిమాణం(rec),
					  కీలకం.dsize);
		/* Unlock the chain of this new record */
		ఒకవేళ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.తాళం_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	ఒకవేళ (tdb_unlock(tdb, పాత్ర(oldhash), tdb->travlocks.తాళం_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	తిరిగి కీలకం;
}

/* file: dump.c */

స్థిర tdb_off_t tdb_dump_రికార్డు(నిర్మాణం tdb_context *tdb, పూర్ణాంకం hash,
				 tdb_off_t స్థానభ్రంశం)
{
	నిర్మాణం list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	ఒకవేళ (tdb->methods->tdb_read(tdb, స్థానభ్రంశం, (అక్షరం *)&rec,
				   పరిమాణం(rec), DOCONV()) == -1) {
		రాయి("ERROR: failed to read record at %u\n", స్థానభ్రంశం);
		తిరిగి 0;
	}

	రాయి(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, స్థానభ్రంశం, rec.తదుపరి, rec.rec_len, rec.కీలకం_len, rec.దత్తాంశం_len,
	       rec.నిండిన_hash, rec.magic);

	tailer_ofs = స్థానభ్రంశం + పరిమాణం(rec) + rec.rec_len - పరిమాణం(tdb_off_t);

	ఒకవేళ (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		రాయి("ERROR: failed to read tailer at %u\n", tailer_ofs);
		తిరిగి rec.తదుపరి;
	}

	ఒకవేళ (tailer != rec.rec_len + పరిమాణం(rec)) {
		రాయి("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(గుర్తులేని పూర్ణాంకం)tailer, (గుర్తులేని పూర్ణాంకం)(rec.rec_len + పరిమాణం(rec)));
	}
	తిరిగి rec.తదుపరి;
}

స్థిర పూర్ణాంకం tdb_dump_chain(నిర్మాణం tdb_context *tdb, పూర్ణాంకం i)
{
	tdb_off_t rec_ptr, శిఖరం;

	శిఖరం = TDB_HASH_శిఖరం(i);

	ఒకవేళ (tdb_తాళం(tdb, i, F_WRLCK) != 0)
		తిరిగి -1;

	ఒకవేళ (tdb_ofs_read(tdb, శిఖరం, &rec_ptr) == -1)
		తిరిగి tdb_unlock(tdb, i, F_WRLCK);

	ఒకవేళ (rec_ptr)
		రాయి("hash=%d\n", i);

	అయితే (rec_ptr) {
		rec_ptr = tdb_dump_రికార్డు(tdb, i, rec_ptr);
	}

	తిరిగి tdb_unlock(tdb, i, F_WRLCK);
}

శూన్య tdb_dump_all(నిర్మాణం tdb_context *tdb)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < (పూర్ణాంకం)tdb->శీర్షిక.hash_కొలత; i++) {
		tdb_dump_chain(tdb, i);
	}
	రాయి("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

పూర్ణాంకం tdb_printfreelist(నిర్మాణం tdb_context *tdb)
{
	పూర్ణాంకం ret;
	దీర్ఘ మొత్తం_free = 0;
	tdb_off_t స్థానభ్రంశం, rec_ptr;
	నిర్మాణం list_struct rec;

	ఒకవేళ ((ret = tdb_తాళం(tdb, -1, F_WRLCK)) != 0)
		తిరిగి ret;

	స్థానభ్రంశం = FREELIST_శిఖరం;

	/* read in the freelist top */
	ఒకవేళ (tdb_ofs_read(tdb, స్థానభ్రంశం, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		తిరిగి 0;
	}

	రాయి("freelist top=[0x%08x]\n", rec_ptr );
	అయితే (rec_ptr) {
		ఒకవేళ (tdb->methods->tdb_read(tdb, rec_ptr, (అక్షరం *)&rec,
					   పరిమాణం(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			తిరిగి -1;
		}

		ఒకవేళ (rec.magic != TDB_FREE_MAGIC) {
			రాయి("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			తిరిగి -1;
		}

		రాయి("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		మొత్తం_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.తదుపరి;
	}
	రాయి("total rec_len = [0x%08x (%d)]\n", (పూర్ణాంకం)మొత్తం_free,
               (పూర్ణాంకం)మొత్తం_free);

	తిరిగి tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
శూన్య tdb_increment_seqnum_nonblock(నిర్మాణం tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ఒకవేళ (!(tdb->flags & TDB_SEQNUM)) {
		తిరిగి;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	ఒకవేళ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		తిరిగి;
	seqnum++;
	(శూన్య) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
స్థిర శూన్య tdb_increment_seqnum(నిర్మాణం tdb_context *tdb)
{
	ఒకవేళ (!(tdb->flags & TDB_SEQNUM)) {
		తిరిగి;
	}

	ఒకవేళ (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		తిరిగి;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

స్థిర పూర్ణాంకం tdb_కీలకం_compare(TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం దత్తాంశం,
			   శూన్య *వ్యక్తిగత_దత్తాంశం EXT2FS_ATTR((వాడని)))
{
	తిరిగి memcmp(దత్తాంశం.dptr, కీలకం.dptr, దత్తాంశం.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
స్థిర tdb_off_t tdb_కనుగొను(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, u32 hash,
			నిర్మాణం list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(hash), &rec_ptr) == -1)
		తిరిగి 0;

	/* keep looking until we find the right record */
	అయితే (rec_ptr) {
		ఒకవేళ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			తిరిగి 0;

		ఒకవేళ (!TDB_DEAD(r) && hash==r->నిండిన_hash
		    && కీలకం.dsize==r->కీలకం_len
		    && tdb_parse_దత్తాంశం(tdb, కీలకం, rec_ptr + పరిమాణం(*r),
				      r->కీలకం_len, tdb_కీలకం_compare,
				      ఖాళీ) == 0) {
			తిరిగి rec_ptr;
		}
		rec_ptr = r->తదుపరి;
	}
	తిరిగి TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_కనుగొను_తాళం_hash(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, u32 hash, పూర్ణాంకం locktype,
			   నిర్మాణం list_struct *rec)
{
	u32 rec_ptr;

	ఒకవేళ (tdb_తాళం(tdb, పాత్ర(hash), locktype) == -1)
		తిరిగి 0;
	ఒకవేళ (!(rec_ptr = tdb_కనుగొను(tdb, కీలకం, hash, rec)))
		tdb_unlock(tdb, పాత్ర(hash), locktype);
	తిరిగి rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
స్థిర పూర్ణాంకం tdb_update_hash(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, u32 hash, TDB_దత్తాంశం dbuf)
{
	నిర్మాణం list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	ఒకవేళ (!(rec_ptr = tdb_కనుగొను(tdb, కీలకం, hash, &rec)))
		తిరిగి -1;

	/* must be long enough key, data and tailer */
	ఒకవేళ (rec.rec_len < కీలకం.dsize + dbuf.dsize + పరిమాణం(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		తిరిగి -1;
	}

	ఒకవేళ (tdb->methods->tdb_write(tdb, rec_ptr + పరిమాణం(rec) + rec.కీలకం_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		తిరిగి -1;

	ఒకవేళ (dbuf.dsize != rec.దత్తాంశం_len) {
		/* update size */
		rec.దత్తాంశం_len = dbuf.dsize;
		తిరిగి tdb_rec_write(tdb, rec_ptr, &rec);
	}

	తిరిగి 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_దత్తాంశం tdb_fetch(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	tdb_off_t rec_ptr;
	నిర్మాణం list_struct rec;
	TDB_దత్తాంశం ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&కీలకం);
	ఒకవేళ (!(rec_ptr = tdb_కనుగొను_తాళం_hash(tdb,కీలకం,hash,F_RDLCK,&rec)))
		తిరిగి tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + పరిమాణం(rec) + rec.కీలకం_len,
				  rec.దత్తాంశం_len);
	ret.dsize = rec.దత్తాంశం_len;
	tdb_unlock(tdb, పాత్ర(rec.నిండిన_hash), F_RDLCK);
	తిరిగి ret;
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

పూర్ణాంకం tdb_parse_రికార్డు(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం,
		     పూర్ణాంకం (*parser)(TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం దత్తాంశం,
				   శూన్య *వ్యక్తిగత_దత్తాంశం),
		     శూన్య *వ్యక్తిగత_దత్తాంశం)
{
	tdb_off_t rec_ptr;
	నిర్మాణం list_struct rec;
	పూర్ణాంకం ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&కీలకం);

	ఒకవేళ (!(rec_ptr = tdb_కనుగొను_తాళం_hash(tdb,కీలకం,hash,F_RDLCK,&rec))) {
		తిరిగి TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_దత్తాంశం(tdb, కీలకం, rec_ptr + పరిమాణం(rec) + rec.కీలకం_len,
			     rec.దత్తాంశం_len, parser, వ్యక్తిగత_దత్తాంశం);

	tdb_unlock(tdb, పాత్ర(rec.నిండిన_hash), F_RDLCK);

	తిరిగి ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
స్థిర పూర్ణాంకం tdb_exists_hash(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, u32 hash)
{
	నిర్మాణం list_struct rec;

	ఒకవేళ (tdb_కనుగొను_తాళం_hash(tdb, కీలకం, hash, F_RDLCK, &rec) == 0)
		తిరిగి 0;
	tdb_unlock(tdb, పాత్ర(rec.నిండిన_hash), F_RDLCK);
	తిరిగి 1;
}

పూర్ణాంకం tdb_exists(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	u32 hash = tdb->hash_fn(&కీలకం);
	తిరిగి tdb_exists_hash(tdb, కీలకం, hash);
}

/* actually delete an entry in the database given the offset */
పూర్ణాంకం tdb_do_delete(నిర్మాణం tdb_context *tdb, tdb_off_t rec_ptr, నిర్మాణం list_struct*rec)
{
	tdb_off_t చివరి_ptr, i;
	నిర్మాణం list_struct lastrec;

	ఒకవేళ (tdb->read_only || tdb->traverse_read) తిరిగి -1;

	ఒకవేళ (tdb_write_తాళం_రికార్డు(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		తిరిగి tdb_rec_write(tdb, rec_ptr, rec);
	}
	ఒకవేళ (tdb_write_unlock_రికార్డు(tdb, rec_ptr) != 0)
		తిరిగి -1;

	/* find previous record in hash chain */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(rec->నిండిన_hash), &i) == -1)
		తిరిగి -1;
	కొరకు (చివరి_ptr = 0; i != rec_ptr; చివరి_ptr = i, i = lastrec.తదుపరి)
		ఒకవేళ (tdb_rec_read(tdb, i, &lastrec) == -1)
			తిరిగి -1;

	/* unlink it: next ptr is at start of record. */
	ఒకవేళ (చివరి_ptr == 0)
		చివరి_ptr = TDB_HASH_శిఖరం(rec->నిండిన_hash);
	ఒకవేళ (tdb_ofs_write(tdb, చివరి_ptr, &rec->తదుపరి) == -1)
		తిరిగి -1;

	/* recover the space */
	ఒకవేళ (tdb_free(tdb, rec_ptr, rec) == -1)
		తిరిగి -1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం tdb_లెక్క_dead(నిర్మాణం tdb_context *tdb, u32 hash)
{
	పూర్ణాంకం res = 0;
	tdb_off_t rec_ptr;
	నిర్మాణం list_struct rec;

	/* read in the hash top */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(hash), &rec_ptr) == -1)
		తిరిగి 0;

	అయితే (rec_ptr) {
		ఒకవేళ (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			తిరిగి 0;

		ఒకవేళ (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.తదుపరి;
	}
	తిరిగి res;
}

/*
 * Purge all DEAD records from a hash chain
 */
స్థిర పూర్ణాంకం tdb_purge_dead(నిర్మాణం tdb_context *tdb, u32 hash)
{
	పూర్ణాంకం res = -1;
	నిర్మాణం list_struct rec;
	tdb_off_t rec_ptr;

	ఒకవేళ (tdb_తాళం(tdb, -1, F_WRLCK) == -1) {
		తిరిగి -1;
	}

	/* read in the hash top */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(hash), &rec_ptr) == -1)
		వెళ్ళు fail;

	అయితే (rec_ptr) {
		tdb_off_t తదుపరి;

		ఒకవేళ (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			వెళ్ళు fail;
		}

		తదుపరి = rec.తదుపరి;

		ఒకవేళ (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			వెళ్ళు fail;
		}
		rec_ptr = తదుపరి;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	తిరిగి res;
}

/* delete an entry in the database given a key */
స్థిర పూర్ణాంకం tdb_delete_hash(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, u32 hash)
{
	tdb_off_t rec_ptr;
	నిర్మాణం list_struct rec;
	పూర్ణాంకం ret;

	ఒకవేళ (tdb->max_dead_రికార్డులు != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		ఒకవేళ (tdb_తాళం(tdb, పాత్ర(hash), F_WRLCK) == -1)
			తిరిగి -1;

		ఒకవేళ (tdb_లెక్క_dead(tdb, hash) >= tdb->max_dead_రికార్డులు) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		ఒకవేళ (!(rec_ptr = tdb_కనుగొను(tdb, కీలకం, hash, &rec))) {
			tdb_unlock(tdb, పాత్ర(hash), F_WRLCK);
			తిరిగి -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	లేకపోతే {
		ఒకవేళ (!(rec_ptr = tdb_కనుగొను_తాళం_hash(tdb, కీలకం, hash, F_WRLCK,
						   &rec)))
			తిరిగి -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	ఒకవేళ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ఒకవేళ (tdb_unlock(tdb, పాత్ర(rec.నిండిన_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	తిరిగి ret;
}

పూర్ణాంకం tdb_delete(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం)
{
	u32 hash = tdb->hash_fn(&కీలకం);
	తిరిగి tdb_delete_hash(tdb, కీలకం, hash);
}

/*
 * See if we have a dead record around with enough space
 */
స్థిర tdb_off_t tdb_కనుగొను_dead(నిర్మాణం tdb_context *tdb, u32 hash,
			       నిర్మాణం list_struct *r, tdb_len_t నిడివి)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(hash), &rec_ptr) == -1)
		తిరిగి 0;

	/* keep looking until we find the right record */
	అయితే (rec_ptr) {
		ఒకవేళ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			తిరిగి 0;

		ఒకవేళ (TDB_DEAD(r) && r->rec_len >= నిడివి) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			తిరిగి rec_ptr;
		}
		rec_ptr = r->తదుపరి;
	}
	తిరిగి 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
పూర్ణాంకం tdb_store(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, పూర్ణాంకం flag)
{
	నిర్మాణం list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	అక్షరం *p = ఖాళీ;
	పూర్ణాంకం ret = -1;

	ఒకవేళ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		తిరిగి -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&కీలకం);
	ఒకవేళ (tdb_తాళం(tdb, పాత్ర(hash), F_WRLCK) == -1)
		తిరిగి -1;

	/* check for it existing, on insert. */
	ఒకవేళ (flag == TDB_చొప్పించు) {
		ఒకవేళ (tdb_exists_hash(tdb, కీలకం, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			వెళ్ళు fail;
		}
	} లేకపోతే {
		/* first try in-place update, on modify or replace. */
		ఒకవేళ (tdb_update_hash(tdb, కీలకం, hash, dbuf) == 0) {
			వెళ్ళు పూర్తి;
		}
		ఒకవేళ (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			వెళ్ళు fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	ఒకవేళ (flag != TDB_చొప్పించు)
		tdb_delete_hash(tdb, కీలకం, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	ఒకవేళ (!(p = (అక్షరం *)జ్ఞాపకం_ఇవ్వు(కీలకం.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		వెళ్ళు fail;
	}

	జ్ఞాపకం_నకలు(p, కీలకం.dptr, కీలకం.dsize);
	ఒకవేళ (dbuf.dsize)
		జ్ఞాపకం_నకలు(p+కీలకం.dsize, dbuf.dptr, dbuf.dsize);

	ఒకవేళ (tdb->max_dead_రికార్డులు != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_కనుగొను_dead(
			tdb, hash, &rec,
			కీలకం.dsize + dbuf.dsize + పరిమాణం(tdb_off_t));

		ఒకవేళ (rec_ptr != 0) {
			rec.కీలకం_len = కీలకం.dsize;
			rec.దత్తాంశం_len = dbuf.dsize;
			rec.నిండిన_hash = hash;
			rec.magic = TDB_MAGIC;
			ఒకవేళ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + పరిమాణం(rec),
				    p, కీలకం.dsize + dbuf.dsize) == -1) {
				వెళ్ళు fail;
			}
			వెళ్ళు పూర్తి;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	ఒకవేళ (tdb_తాళం(tdb, -1, F_WRLCK) == -1) {
		వెళ్ళు fail;
	}

	ఒకవేళ ((tdb->max_dead_రికార్డులు != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		వెళ్ళు fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, కీలకం.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	ఒకవేళ (rec_ptr == 0) {
		వెళ్ళు fail;
	}

	/* Read hash top into next ptr */
	ఒకవేళ (tdb_ofs_read(tdb, TDB_HASH_శిఖరం(hash), &rec.తదుపరి) == -1)
		వెళ్ళు fail;

	rec.కీలకం_len = కీలకం.dsize;
	rec.దత్తాంశం_len = dbuf.dsize;
	rec.నిండిన_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	ఒకవేళ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+పరిమాణం(rec), p, కీలకం.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_శిఖరం(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		వెళ్ళు fail;
	}

 పూర్తి:
	ret = 0;
 fail:
	ఒకవేళ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	సురక్షిత_FREE(p);
	tdb_unlock(tdb, పాత్ర(hash), F_WRLCK);
	తిరిగి ret;
}


/* Append to an entry. Create if not exist. */
పూర్ణాంకం tdb_append(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం new_dbuf)
{
	u32 hash;
	TDB_దత్తాంశం dbuf;
	పూర్ణాంకం ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&కీలకం);
	ఒకవేళ (tdb_తాళం(tdb, పాత్ర(hash), F_WRLCK) == -1)
		తిరిగి -1;

	dbuf = tdb_fetch(tdb, కీలకం);

	ఒకవేళ (dbuf.dptr == ఖాళీ) {
		dbuf.dptr = (గుర్తులేని అక్షరం *)జ్ఞాపకం_ఇవ్వు(new_dbuf.dsize);
	} లేకపోతే {
		గుర్తులేని అక్షరం *new_dptr = (గుర్తులేని అక్షరం *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		ఒకవేళ (new_dptr == ఖాళీ) {
			విడుదల(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	ఒకవేళ (dbuf.dptr == ఖాళీ) {
		tdb->ecode = TDB_ERR_OOM;
		వెళ్ళు failed;
	}

	జ్ఞాపకం_నకలు(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, కీలకం, dbuf, 0);

failed:
	tdb_unlock(tdb, పాత్ర(hash), F_WRLCK);
	సురక్షిత_FREE(dbuf.dptr);
	తిరిగి ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
స్థిరరాశి అక్షరం *tdb_name(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
పూర్ణాంకం tdb_fd(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->సంవర్గమానం.log_fn;
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
పూర్ణాంకం tdb_get_seqnum(నిర్మాణం tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ఒకవేళ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		తిరిగి 0;
	తిరిగి seqnum;
}

పూర్ణాంకం tdb_hash_కొలత(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->శీర్షిక.hash_కొలత;
}

size_t tdb_map_కొలత(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->map_కొలత;
}

పూర్ణాంకం tdb_get_flags(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
శూన్య tdb_enable_seqnum(నిర్మాణం tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
స్థిర నిర్మాణం tdb_context *tdbs = ఖాళీ;


/* This is from a hash algorithm suggested by Rogier Wolff */
స్థిర గుర్తులేని పూర్ణాంకం default_tdb_hash(TDB_దత్తాంశం *కీలకం)
{
	u32 విలువ;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	కొరకు (విలువ = 0, i=0; i < కీలకం->dsize; i++)
		విలువ = విలువ * 256 + కీలకం->dptr[i] + (విలువ >> 24) * 241;

	తిరిగి విలువ;
}


/* initialise a new database with a specified hash size */
స్థిర పూర్ణాంకం tdb_new_database(నిర్మాణం tdb_context *tdb, పూర్ణాంకం hash_కొలత)
{
	నిర్మాణం tdb_శీర్షిక *newdb;
	పూర్ణాంకం కొలత, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	కొలత = పరిమాణం(నిర్మాణం tdb_శీర్షిక) + (hash_కొలత+1)*పరిమాణం(tdb_off_t);
	ఒకవేళ (!(newdb = (నిర్మాణం tdb_శీర్షిక *)సున్నా_జ్ఞాపకం_ఇవ్వు(కొలత, 1)))
		తిరిగి TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->సంస్కరణ = TDB_సంస్కరణ;
	newdb->hash_కొలత = hash_కొలత;
	ఒకవేళ (tdb->flags & TDB_అంతర్గత) {
		tdb->map_కొలత = కొలత;
		tdb->map_ptr = (అక్షరం *)newdb;
		జ్ఞాపకం_నకలు(&tdb->శీర్షిక, newdb, పరిమాణం(tdb->శీర్షిక));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		తిరిగి 0;
	}
	ఒకవేళ (lseek(tdb->fd, 0, SEEK_SET) == -1)
		వెళ్ళు fail;

	ఒకవేళ (ftruncate(tdb->fd, 0) == -1)
		వెళ్ళు fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	జ్ఞాపకం_నకలు(&tdb->శీర్షిక, newdb, పరిమాణం(tdb->శీర్షిక));
	/* Don't endian-convert the magic food! */
	జ్ఞాపకం_నకలు(newdb->magic_food, TDB_MAGIC_FOOD, అక్షరశ్రేణి_పొడవు(TDB_MAGIC_FOOD)+1);
	ఒకవేళ (లిఖ(tdb->fd, newdb, కొలత) != కొలత) {
		ret = -1;
	} లేకపోతే {
		ret = 0;
	}

  fail:
	సురక్షిత_FREE(newdb);
	తిరిగి ret;
}



స్థిర పూర్ణాంకం tdb_already_open(dev_t device,
			    ino_t ino)
{
	నిర్మాణం tdb_context *i;

	కొరకు (i = tdbs; i; i = i->తదుపరి) {
		ఒకవేళ (i->device == device && i->inode == ino) {
			తిరిగి 1;
		}
	}

	తిరిగి 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
నిర్మాణం tdb_context *tdb_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం hash_కొలత, పూర్ణాంకం tdb_flags,
		      పూర్ణాంకం open_flags, mode_t బహుళకం)
{
	తిరిగి tdb_open_ex(name, hash_కొలత, tdb_flags, open_flags, బహుళకం, ఖాళీ, ఖాళీ);
}

/* a default logging function */
స్థిర శూన్య null_log_fn(నిర్మాణం tdb_context *tdb, గణన tdb_debug_level level, స్థిరరాశి అక్షరం *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
స్థిర శూన్య null_log_fn(నిర్మాణం tdb_context *tdb EXT2FS_ATTR((వాడని)),
			గణన tdb_debug_level level EXT2FS_ATTR((వాడని)),
			స్థిరరాశి అక్షరం *fmt EXT2FS_ATTR((వాడని)), ...)
{
}


నిర్మాణం tdb_context *tdb_open_ex(స్థిరరాశి అక్షరం *name, పూర్ణాంకం hash_కొలత, పూర్ణాంకం tdb_flags,
				పూర్ణాంకం open_flags, mode_t బహుళకం,
				స్థిరరాశి నిర్మాణం tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	నిర్మాణం tdb_context *tdb;
	నిర్మాణం stat st;
	పూర్ణాంకం rev = 0, locked = 0;
	గుర్తులేని అక్షరం *vp;
	u32 vertest;

	ఒకవేళ (!(tdb = (నిర్మాణం tdb_context *)సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		వెళ్ళు fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ఖాళీ;
	tdb->map_ptr = ఖాళీ;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	ఒకవేళ (log_ctx) {
		tdb->సంవర్గమానం = *log_ctx;
	} లేకపోతే {
		tdb->సంవర్గమానం.log_fn = null_log_fn;
		tdb->సంవర్గమానం.log_వ్యక్తిగత = ఖాళీ;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->పుట_కొలత = sysconf(_SC_PAGESIZE);
	ఒకవేళ (tdb->పుట_కొలత <= 0) {
		tdb->పుట_కొలత = 0x2000;
	}

	ఒకవేళ ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		వెళ్ళు fail;
	}

	ఒకవేళ (hash_కొలత == 0)
		hash_కొలత = DEFAULT_HASH_కొలత;
	ఒకవేళ ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_మొదటి;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	ఒకవేళ (tdb->flags & TDB_అంతర్గత) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_మొదటి;
		ఒకవేళ (tdb_new_database(tdb, hash_కొలత) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			వెళ్ళు fail;
		}
		వెళ్ళు అంతర్గత;
	}

	ఒకవేళ ((tdb->fd = open(name, open_flags, బహుళకం)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		వెళ్ళు fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	ఒకవేళ (tdb->methods->tdb_brlock(tdb, GLOBAL_తాళం, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		వెళ్ళు fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	ఒకవేళ ((tdb_flags & TDB_CLEAR_IF_మొదటి) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, చురుకైన_తాళం, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		ఒకవేళ (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			వెళ్ళు fail; /* errno set by ftruncate */
		}
	}

	ఒకవేళ (పఠ(tdb->fd, &tdb->శీర్షిక, పరిమాణం(tdb->శీర్షిక)) != పరిమాణం(tdb->శీర్షిక)
	    || memcmp(tdb->శీర్షిక.magic_food, TDB_MAGIC_FOOD,
		      పరిమాణం(TDB_MAGIC_FOOD)) != 0
	    || (tdb->శీర్షిక.సంస్కరణ != TDB_సంస్కరణ
		&& !(rev = (tdb->శీర్షిక.సంస్కరణ==TDB_BYTEREV(TDB_సంస్కరణ))))) {
		/* its not a valid database - possibly initialise it */
		ఒకవేళ (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_కొలత) == -1) {
			errno = EIO; /* ie bad format or something */
			వెళ్ళు fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (గుర్తులేని అక్షరం *)&tdb->శీర్షిక.సంస్కరణ;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_సంస్కరణ) ? TDB_BIGENDIAN : 0;
	ఒకవేళ (!rev)
		tdb->flags &= ~TDB_CONVERT;
	లేకపోతే {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->శీర్షిక, పరిమాణం(tdb->శీర్షిక));
	}
	ఒకవేళ (fstat(tdb->fd, &st) == -1)
		వెళ్ళు fail;

	ఒకవేళ (tdb->శీర్షిక.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		వెళ్ళు fail;
	}

	/* Is it already in the open list?  If so, fail. */
	ఒకవేళ (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (పూర్ణాంకం)st.st_dev, (పూర్ణాంకం)st.st_ino));
		errno = EBUSY;
		వెళ్ళు fail;
	}

	ఒకవేళ (!(tdb->name = (అక్షరం *)strdup(name))) {
		errno = ENOMEM;
		వెళ్ళు fail;
	}

	tdb->map_కొలత = st.st_కొలత;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_రికార్డులు = 0;
	tdb_mmap(tdb);
	ఒకవేళ (locked) {
		ఒకవేళ (tdb->methods->tdb_brlock(tdb, చురుకైన_తాళం, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			వెళ్ళు fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	ఒకవేళ (tdb_flags & TDB_CLEAR_IF_మొదటి) {
		/* leave this lock in place to indicate it's in use */
		ఒకవేళ (tdb->methods->tdb_brlock(tdb, చురుకైన_తాళం, F_RDLCK, F_SETLKW, 0, 1) == -1)
			వెళ్ళు fail;
	}

	/* if needed, run recovery */
	ఒకవేళ (tdb_transaction_recover(tdb) == -1) {
		వెళ్ళు fail;
	}

 అంతర్గత:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	ఒకవేళ (tdb->methods->tdb_brlock(tdb, GLOBAL_తాళం, F_UNLCK, F_SETLKW, 0, 1) == -1)
		వెళ్ళు fail;
	tdb->తదుపరి = tdbs;
	tdbs = tdb;
	తిరిగి tdb;

 fail:
	{ పూర్ణాంకం save_errno = errno;

	ఒకవేళ (!tdb)
		తిరిగి ఖాళీ;

	ఒకవేళ (tdb->map_ptr) {
		ఒకవేళ (tdb->flags & TDB_అంతర్గత)
			సురక్షిత_FREE(tdb->map_ptr);
		లేకపోతే
			tdb_munmap(tdb);
	}
	సురక్షిత_FREE(tdb->name);
	ఒకవేళ (tdb->fd != -1)
		ఒకవేళ (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	సురక్షిత_FREE(tdb);
	errno = save_errno;
	తిరిగి ఖాళీ;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

శూన్య tdb_సమితి_max_dead(నిర్మాణం tdb_context *tdb, పూర్ణాంకం max_dead)
{
	tdb->max_dead_రికార్డులు = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
పూర్ణాంకం tdb_close(నిర్మాణం tdb_context *tdb)
{
	నిర్మాణం tdb_context **i;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	ఒకవేళ (tdb->map_ptr) {
		ఒకవేళ (tdb->flags & TDB_అంతర్గత)
			సురక్షిత_FREE(tdb->map_ptr);
		లేకపోతే
			tdb_munmap(tdb);
	}
	సురక్షిత_FREE(tdb->name);
	ఒకవేళ (tdb->fd != -1)
		ret = close(tdb->fd);
	సురక్షిత_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	కొరకు (i = &tdbs; *i; i = &(*i)->తదుపరి) {
		ఒకవేళ (*i == tdb) {
			*i = tdb->తదుపరి;
			ఆపు;
		}
	}

	memset(tdb, 0, పరిమాణం(*tdb));
	సురక్షిత_FREE(tdb);

	తిరిగి ret;
}

/* register a logging function */
శూన్య tdb_సమితి_logging_function(నిర్మాణం tdb_context *tdb,
                              స్థిరరాశి నిర్మాణం tdb_logging_context *log_ctx)
{
        tdb->సంవర్గమానం = *log_ctx;
}

శూన్య *tdb_get_logging_వ్యక్తిగత(నిర్మాణం tdb_context *tdb)
{
	తిరిగి tdb->సంవర్గమానం.log_వ్యక్తిగత;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
పూర్ణాంకం tdb_reopen(నిర్మాణం tdb_context *tdb)
{
	నిర్మాణం stat st;

	ఒకవేళ (tdb->flags & TDB_అంతర్గత) {
		తిరిగి 0; /* Nothing to do. */
	}

	ఒకవేళ (tdb->అంకె_locks != 0 || tdb->global_తాళం.లెక్క) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		వెళ్ళు fail;
	}

	ఒకవేళ (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		వెళ్ళు fail;
	}

	ఒకవేళ (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		వెళ్ళు fail;
	}
	ఒకవేళ (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	ఒకవేళ (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		వెళ్ళు fail;
	}
	ఒకవేళ ((tdb->flags & TDB_CLEAR_IF_మొదటి) &&
	    (tdb->methods->tdb_brlock(tdb, చురుకైన_తాళం, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		వెళ్ళు fail;
	}
	ఒకవేళ (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		వెళ్ళు fail;
	}
	ఒకవేళ (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		వెళ్ళు fail;
	}
	tdb_mmap(tdb);

	తిరిగి 0;

fail:
	tdb_close(tdb);
	తిరిగి -1;
}

/* reopen all tdb's */
పూర్ణాంకం tdb_reopen_all(పూర్ణాంకం మాతృ_longlived)
{
	నిర్మాణం tdb_context *tdb;

	కొరకు (tdb=tdbs; tdb; tdb = tdb->తదుపరి) {
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
		ఒకవేళ (మాతృ_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_మొదటి;
		}

		ఒకవేళ (tdb_reopen(tdb) != 0)
			తిరిగి -1;
	}

	తిరిగి 0;
}

/**
 * Flush a database file from the page cache.
 **/
పూర్ణాంకం tdb_flush(నిర్మాణం tdb_context *tdb)
{
	ఒకవేళ (tdb->fd != -1)
		తిరిగి fsync(tdb->fd);
	తిరిగి 0;
}
