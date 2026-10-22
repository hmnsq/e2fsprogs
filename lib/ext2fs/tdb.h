#నిర్వచితం_కాకపోతే __TDB_H__
#నిర్వచించు __TDB_H__

/*
   Unix SMB/CIFS implementation.

   trivial database library

   Copyright (C) Andrew Tridgell 1999-2004

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

#నిర్వచితమైతే  __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు


/* flags to tdb_store() */
#నిర్వచించు TDB_REPLACE 1
#నిర్వచించు TDB_చొప్పించు 2
#నిర్వచించు TDB_MODIFY 3

/* flags for tdb_open() */
#నిర్వచించు TDB_DEFAULT 0 /* just a readability place holder */
#నిర్వచించు TDB_CLEAR_IF_మొదటి 1
#నిర్వచించు TDB_అంతర్గత 2 /* don't store on disk */
#నిర్వచించు TDB_NOLOCK   4 /* don't do any locking */
#నిర్వచించు TDB_NOMMAP   8 /* don't use mmap */
#నిర్వచించు TDB_CONVERT 16 /* convert endian (internal use) */
#నిర్వచించు TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#నిర్వచించు TDB_NOSYNC   64 /* don't use synchronous transactions */
#నిర్వచించు TDB_SEQNUM   128 /* maintain a sequence number */

#నిర్వచించు TDB_ERRCODE(సంకేతం_2, ret) ((tdb->ecode = (సంకేతం_2)), ret)

/* error codes */
గణన TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_తాళం,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_తాళం_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
గణన tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

రకనిర్వచనం నిర్మాణం TDB_దత్తాంశం {
	గుర్తులేని అక్షరం *dptr;
	size_t dsize;
} TDB_దత్తాంశం;

#నిర్వచితం_కాకపోతే PRINTF_ATTRIBUTE
#ఒకవేళ (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#నిర్వచించు PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#లేకపోతే
#నిర్వచించు PRINTF_ATTRIBUTE(a1, a2)
#అయితే_ముగింపు
#అయితే_ముగింపు

/* ext2fs tdb renames */
#నిర్వచించు tdb_open ext2fs_tdb_open
#నిర్వచించు tdb_open_ex ext2fs_tdb_open_ex
#నిర్వచించు tdb_సమితి_max_dead ext2fs_tdb_సమితి_max_dead
#నిర్వచించు tdb_reopen ext2fs_tdb_reopen
#నిర్వచించు tdb_reopen_all ext2fs_tdb_reopen_all
#నిర్వచించు tdb_సమితి_logging_function ext2fs_tdb_సమితి_logging_function
#నిర్వచించు tdb_error ext2fs_tdb_error
#నిర్వచించు tdb_errorstr ext2fs_tdb_errorstr
#నిర్వచించు tdb_fetch ext2fs_tdb_fetch
#నిర్వచించు tdb_parse_రికార్డు ext2fs_tdb_parse_రికార్డు
#నిర్వచించు tdb_delete ext2fs_tdb_delete
#నిర్వచించు tdb_store ext2fs_tdb_store
#నిర్వచించు tdb_append ext2fs_tdb_append
#నిర్వచించు tdb_close ext2fs_tdb_close
#నిర్వచించు tdb_firstkey ext2fs_tdb_firstkey
#నిర్వచించు tdb_nextkey ext2fs_tdb_nextkey
#నిర్వచించు tdb_traverse ext2fs_tdb_traverse
#నిర్వచించు tdb_traverse_read ext2fs_tdb_traverse_read
#నిర్వచించు tdb_exists ext2fs_tdb_exists
#నిర్వచించు tdb_lockall ext2fs_tdb_lockall
#నిర్వచించు tdb_unlockall ext2fs_tdb_unlockall
#నిర్వచించు tdb_lockall_read ext2fs_tdb_lockall_read
#నిర్వచించు tdb_unlockall_read ext2fs_tdb_unlockall_read
#నిర్వచించు tdb_name ext2fs_tdb_name
#నిర్వచించు tdb_fd ext2fs_tdb_fd
#నిర్వచించు tdb_log_fn ext2fs_tdb_log_fn
#నిర్వచించు tdb_get_logging_వ్యక్తిగత ext2fs_tdb_get_logging_వ్యక్తిగత
#నిర్వచించు tdb_transaction_ప్రారంభం ext2fs_tdb_transaction_ప్రారంభం
#నిర్వచించు tdb_transaction_commit ext2fs_tdb_transaction_commit
#నిర్వచించు tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#నిర్వచించు tdb_transaction_recover ext2fs_tdb_transaction_recover
#నిర్వచించు tdb_get_seqnum ext2fs_tdb_get_seqnum
#నిర్వచించు tdb_hash_కొలత ext2fs_tdb_hash_కొలత
#నిర్వచించు tdb_map_కొలత ext2fs_tdb_map_కొలత
#నిర్వచించు tdb_get_flags ext2fs_tdb_get_flags
#నిర్వచించు tdb_chainlock ext2fs_tdb_chainlock
#నిర్వచించు tdb_chainunlock ext2fs_tdb_chainunlock
#నిర్వచించు tdb_chainlock_read ext2fs_tdb_chainlock_read
#నిర్వచించు tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#నిర్వచించు tdb_dump_all ext2fs_tdb_dump_all
#నిర్వచించు tdb_printfreelist ext2fs_tdb_printfreelist
#నిర్వచించు tdb_validate_freelist ext2fs_tdb_validate_freelist
#నిర్వచించు tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#నిర్వచించు tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#నిర్వచించు tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#నిర్వచించు tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#నిర్వచించు tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#నిర్వచించు tdb_తాళం_nonblock ext2fs_tdb_తాళం_nonblock
#నిర్వచించు tdb_lockall_mark ext2fs_tdb_lockall_mark
#నిర్వచించు tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#నిర్వచించు tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#నిర్వచించు tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#నిర్వచించు tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
రకనిర్వచనం నిర్మాణం tdb_context TDB_CONTEXT;

రకనిర్వచనం పూర్ణాంకం (*tdb_traverse_func)(నిర్మాణం tdb_context *, TDB_దత్తాంశం, TDB_దత్తాంశం, శూన్య *);
రకనిర్వచనం శూన్య (*tdb_log_func)(నిర్మాణం tdb_context *, గణన tdb_debug_level, స్థిరరాశి అక్షరం *, ...) PRINTF_ATTRIBUTE(3, 4);
రకనిర్వచనం గుర్తులేని పూర్ణాంకం (*tdb_hash_func)(TDB_దత్తాంశం *కీలకం);

నిర్మాణం tdb_logging_context {
        tdb_log_func log_fn;
        శూన్య *log_వ్యక్తిగత;
};

నిర్మాణం tdb_context *tdb_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం hash_కొలత, పూర్ణాంకం tdb_flags,
		      పూర్ణాంకం open_flags, mode_t బహుళకం);
నిర్మాణం tdb_context *tdb_open_ex(స్థిరరాశి అక్షరం *name, పూర్ణాంకం hash_కొలత, పూర్ణాంకం tdb_flags,
			 పూర్ణాంకం open_flags, mode_t బహుళకం,
			 స్థిరరాశి నిర్మాణం tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
శూన్య tdb_సమితి_max_dead(నిర్మాణం tdb_context *tdb, పూర్ణాంకం max_dead);

పూర్ణాంకం tdb_reopen(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_reopen_all(పూర్ణాంకం మాతృ_longlived);
శూన్య tdb_సమితి_logging_function(నిర్మాణం tdb_context *tdb, స్థిరరాశి నిర్మాణం tdb_logging_context *log_ctx);
గణన TDB_ERROR tdb_error(నిర్మాణం tdb_context *tdb);
స్థిరరాశి అక్షరం *tdb_errorstr(నిర్మాణం tdb_context *tdb);
TDB_దత్తాంశం tdb_fetch(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_parse_రికార్డు(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం,
		     పూర్ణాంకం (*parser)(TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం దత్తాంశం,
				   శూన్య *వ్యక్తిగత_దత్తాంశం),
		     శూన్య *వ్యక్తిగత_దత్తాంశం);
పూర్ణాంకం tdb_delete(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_store(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, పూర్ణాంకం flag);
పూర్ణాంకం tdb_append(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం new_dbuf);
పూర్ణాంకం tdb_close(నిర్మాణం tdb_context *tdb);
TDB_దత్తాంశం tdb_firstkey(నిర్మాణం tdb_context *tdb);
TDB_దత్తాంశం tdb_nextkey(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_traverse(నిర్మాణం tdb_context *tdb, tdb_traverse_func fn, శూన్య *);
పూర్ణాంకం tdb_traverse_read(నిర్మాణం tdb_context *tdb, tdb_traverse_func fn, శూన్య *);
పూర్ణాంకం tdb_exists(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_lockall(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_lockall_nonblock(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_unlockall(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_lockall_read(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_lockall_read_nonblock(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_unlockall_read(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_lockall_mark(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_lockall_unmark(నిర్మాణం tdb_context *tdb);
స్థిరరాశి అక్షరం *tdb_name(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_fd(నిర్మాణం tdb_context *tdb);
tdb_log_func tdb_log_fn(నిర్మాణం tdb_context *tdb);
శూన్య *tdb_get_logging_వ్యక్తిగత(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_transaction_ప్రారంభం(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_transaction_commit(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_transaction_cancel(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_transaction_recover(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_get_seqnum(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_hash_కొలత(నిర్మాణం tdb_context *tdb);
size_t tdb_map_కొలత(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_get_flags(నిర్మాణం tdb_context *tdb);
శూన్య tdb_enable_seqnum(నిర్మాణం tdb_context *tdb);
శూన్య tdb_increment_seqnum_nonblock(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_flush(నిర్మాణం tdb_context *tdb);

/* Low level locking functions: use with care */
పూర్ణాంకం tdb_chainlock(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_chainlock_nonblock(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_chainunlock(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_chainlock_read(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_chainunlock_read(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_chainlock_mark(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);
పూర్ణాంకం tdb_chainlock_unmark(నిర్మాణం tdb_context *tdb, TDB_దత్తాంశం కీలకం);

/* Debug functions. Not used in production. */
శూన్య tdb_dump_all(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_printfreelist(నిర్మాణం tdb_context *tdb);
పూర్ణాంకం tdb_validate_freelist(నిర్మాణం tdb_context *tdb, పూర్ణాంకం *pnum_entries);

#నిర్వచితమైతే  __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు /* tdb.h */
