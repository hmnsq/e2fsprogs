#ifndef __TDB_H__
#define __TDB_H__

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

#ifdef  __cplusplus
বাহ্য "C" {
#endif


/* flags to tdb_store() */
#define TDB_REPLACE 1
#define TDB_ঢোকাও 2
#define TDB_MODIFY 3

/* flags for tdb_open() */
#define TDB_DEFAULT 0 /* just a readability place holder */
#define TDB_CLEAR_IF_FIRST 1
#define TDB_INTERNAL 2 /* don't store on disk */
#define TDB_NOLOCK   4 /* don't do any locking */
#define TDB_NOMMAP   8 /* don't use mmap */
#define TDB_CONVERT 16 /* convert endian (internal use) */
#define TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#define TDB_NOSYNC   64 /* don't use synchronous transactions */
#define TDB_SEQNUM   128 /* maintain a sequence number */

#define TDB_ERRCODE(code, ret) ((tdb->ecode = (code)), ret)

/* error codes */
গণনা TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_LOCK,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_LOCK_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
গণনা tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

প্রকারনির্দেশ সংরচনা TDB_DATA {
	unsigned ৱর্ণ *dptr;
	size_t dsize;
} TDB_DATA;

#ifndef PRINTF_ATTRIBUTE
#যদি (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#অন্যথা
#define PRINTF_ATTRIBUTE(a1, a2)
#endif
#endif

/* ext2fs tdb renames */
#define tdb_open ext2fs_tdb_open
#define tdb_open_ex ext2fs_tdb_open_ex
#define tdb_set_max_dead ext2fs_tdb_set_max_dead
#define tdb_reopen ext2fs_tdb_reopen
#define tdb_reopen_all ext2fs_tdb_reopen_all
#define tdb_set_logging_function ext2fs_tdb_set_logging_function
#define tdb_error ext2fs_tdb_error
#define tdb_errorstr ext2fs_tdb_errorstr
#define tdb_fetch ext2fs_tdb_fetch
#define tdb_parse_record ext2fs_tdb_parse_record
#define tdb_delete ext2fs_tdb_delete
#define tdb_store ext2fs_tdb_store
#define tdb_append ext2fs_tdb_append
#define tdb_close ext2fs_tdb_close
#define tdb_firstkey ext2fs_tdb_firstkey
#define tdb_nextkey ext2fs_tdb_nextkey
#define tdb_traverse ext2fs_tdb_traverse
#define tdb_traverse_read ext2fs_tdb_traverse_read
#define tdb_exists ext2fs_tdb_exists
#define tdb_lockall ext2fs_tdb_lockall
#define tdb_unlockall ext2fs_tdb_unlockall
#define tdb_lockall_read ext2fs_tdb_lockall_read
#define tdb_unlockall_read ext2fs_tdb_unlockall_read
#define tdb_name ext2fs_tdb_name
#define tdb_fd ext2fs_tdb_fd
#define tdb_log_fn ext2fs_tdb_log_fn
#define tdb_get_logging_private ext2fs_tdb_get_logging_private
#define tdb_transaction_start ext2fs_tdb_transaction_start
#define tdb_transaction_commit ext2fs_tdb_transaction_commit
#define tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#define tdb_transaction_recover ext2fs_tdb_transaction_recover
#define tdb_get_seqnum ext2fs_tdb_get_seqnum
#define tdb_hash_size ext2fs_tdb_hash_size
#define tdb_map_size ext2fs_tdb_map_size
#define tdb_get_flags ext2fs_tdb_get_flags
#define tdb_chainlock ext2fs_tdb_chainlock
#define tdb_chainunlock ext2fs_tdb_chainunlock
#define tdb_chainlock_read ext2fs_tdb_chainlock_read
#define tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#define tdb_dump_all ext2fs_tdb_dump_all
#define tdb_printfreelist ext2fs_tdb_printfreelist
#define tdb_validate_freelist ext2fs_tdb_validate_freelist
#define tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#define tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#define tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#define tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#define tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#define tdb_lock_nonblock ext2fs_tdb_lock_nonblock
#define tdb_lockall_mark ext2fs_tdb_lockall_mark
#define tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#define tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#define tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#define tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
প্রকারনির্দেশ সংরচনা tdb_context TDB_CONTEXT;

প্রকারনির্দেশ পূর্ণাঙ্ক (*tdb_traverse_func)(সংরচনা tdb_context *, TDB_DATA, TDB_DATA, শূন্য *);
প্রকারনির্দেশ শূন্য (*tdb_log_func)(সংরচনা tdb_context *, গণনা tdb_debug_level, ধ্রুৱ ৱর্ণ *, ...) PRINTF_ATTRIBUTE(3, 4);
প্রকারনির্দেশ unsigned পূর্ণাঙ্ক (*tdb_hash_func)(TDB_DATA *চাবি);

সংরচনা tdb_logging_context {
        tdb_log_func log_fn;
        শূন্য *log_private;
};

সংরচনা tdb_context *tdb_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক hash_size, পূর্ণাঙ্ক tdb_flags,
		      পূর্ণাঙ্ক open_flags, mode_t mode);
সংরচনা tdb_context *tdb_open_ex(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক hash_size, পূর্ণাঙ্ক tdb_flags,
			 পূর্ণাঙ্ক open_flags, mode_t mode,
			 ধ্রুৱ সংরচনা tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
শূন্য tdb_set_max_dead(সংরচনা tdb_context *tdb, পূর্ণাঙ্ক max_dead);

পূর্ণাঙ্ক tdb_reopen(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_reopen_all(পূর্ণাঙ্ক parent_longlived);
শূন্য tdb_set_logging_function(সংরচনা tdb_context *tdb, ধ্রুৱ সংরচনা tdb_logging_context *log_ctx);
গণনা TDB_ERROR tdb_error(সংরচনা tdb_context *tdb);
ধ্রুৱ ৱর্ণ *tdb_errorstr(সংরচনা tdb_context *tdb);
TDB_DATA tdb_fetch(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_parse_record(সংরচনা tdb_context *tdb, TDB_DATA চাবি,
		     পূর্ণাঙ্ক (*parser)(TDB_DATA চাবি, TDB_DATA data,
				   শূন্য *private_data),
		     শূন্য *private_data);
পূর্ণাঙ্ক tdb_delete(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_store(সংরচনা tdb_context *tdb, TDB_DATA চাবি, TDB_DATA dbuf, পূর্ণাঙ্ক flag);
পূর্ণাঙ্ক tdb_append(সংরচনা tdb_context *tdb, TDB_DATA চাবি, TDB_DATA new_dbuf);
পূর্ণাঙ্ক tdb_close(সংরচনা tdb_context *tdb);
TDB_DATA tdb_firstkey(সংরচনা tdb_context *tdb);
TDB_DATA tdb_nextkey(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_traverse(সংরচনা tdb_context *tdb, tdb_traverse_func fn, শূন্য *);
পূর্ণাঙ্ক tdb_traverse_read(সংরচনা tdb_context *tdb, tdb_traverse_func fn, শূন্য *);
পূর্ণাঙ্ক tdb_exists(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_lockall(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_lockall_nonblock(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_unlockall(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_lockall_read(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_lockall_read_nonblock(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_unlockall_read(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_lockall_mark(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_lockall_unmark(সংরচনা tdb_context *tdb);
ধ্রুৱ ৱর্ণ *tdb_name(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_fd(সংরচনা tdb_context *tdb);
tdb_log_func tdb_log_fn(সংরচনা tdb_context *tdb);
শূন্য *tdb_get_logging_private(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_transaction_start(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_transaction_commit(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_transaction_cancel(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_transaction_recover(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_get_seqnum(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_hash_size(সংরচনা tdb_context *tdb);
size_t tdb_map_size(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_get_flags(সংরচনা tdb_context *tdb);
শূন্য tdb_enable_seqnum(সংরচনা tdb_context *tdb);
শূন্য tdb_increment_seqnum_nonblock(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_flush(সংরচনা tdb_context *tdb);

/* Low level locking functions: use with care */
পূর্ণাঙ্ক tdb_chainlock(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_chainlock_nonblock(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_chainunlock(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_chainlock_read(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_chainunlock_read(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_chainlock_mark(সংরচনা tdb_context *tdb, TDB_DATA চাবি);
পূর্ণাঙ্ক tdb_chainlock_unmark(সংরচনা tdb_context *tdb, TDB_DATA চাবি);

/* Debug functions. Not used in production. */
শূন্য tdb_dump_all(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_printfreelist(সংরচনা tdb_context *tdb);
পূর্ণাঙ্ক tdb_validate_freelist(সংরচনা tdb_context *tdb, পূর্ণাঙ্ক *pnum_entries);

#ifdef  __cplusplus
}
#endif

#endif /* tdb.h */
