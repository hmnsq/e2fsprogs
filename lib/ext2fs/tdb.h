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
خارجي "C" {
#endif


/* flags to tdb_store() */
#define TDB_REPLACE 1
#define TDB_قىستۇرۇش 2
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
تعداد TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_LOCK,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_LOCK_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
تعداد tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

تعريفنوع بنية TDB_DATA {
	unsigned حرف *dptr;
	size_t dsize;
} TDB_DATA;

#ifndef PRINTF_ATTRIBUTE
#إذا (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وإلا
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
تعريفنوع بنية tdb_context TDB_CONTEXT;

تعريفنوع صحيح (*tdb_traverse_func)(بنية tdb_context *, TDB_DATA, TDB_DATA, فراغ *);
تعريفنوع فراغ (*tdb_log_func)(بنية tdb_context *, تعداد tdb_debug_level, ثابت حرف *, ...) PRINTF_ATTRIBUTE(3, 4);
تعريفنوع unsigned صحيح (*tdb_hash_func)(TDB_DATA *ئاچقۇچ);

بنية tdb_logging_context {
        tdb_log_func log_fn;
        فراغ *log_private;
};

بنية tdb_context *tdb_open(ثابت حرف *name, صحيح hash_size, صحيح tdb_flags,
		      صحيح open_flags, mode_t mode);
بنية tdb_context *tdb_open_ex(ثابت حرف *name, صحيح hash_size, صحيح tdb_flags,
			 صحيح open_flags, mode_t mode,
			 ثابت بنية tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
فراغ tdb_set_max_dead(بنية tdb_context *tdb, صحيح max_dead);

صحيح tdb_reopen(بنية tdb_context *tdb);
صحيح tdb_reopen_all(صحيح parent_longlived);
فراغ tdb_set_logging_function(بنية tdb_context *tdb, ثابت بنية tdb_logging_context *log_ctx);
تعداد TDB_ERROR tdb_error(بنية tdb_context *tdb);
ثابت حرف *tdb_errorstr(بنية tdb_context *tdb);
TDB_DATA tdb_fetch(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_parse_record(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ,
		     صحيح (*parser)(TDB_DATA ئاچقۇچ, TDB_DATA data,
				   فراغ *private_data),
		     فراغ *private_data);
صحيح tdb_delete(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_store(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ, TDB_DATA dbuf, صحيح flag);
صحيح tdb_append(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ, TDB_DATA new_dbuf);
صحيح tdb_close(بنية tdb_context *tdb);
TDB_DATA tdb_firstkey(بنية tdb_context *tdb);
TDB_DATA tdb_nextkey(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_traverse(بنية tdb_context *tdb, tdb_traverse_func fn, فراغ *);
صحيح tdb_traverse_read(بنية tdb_context *tdb, tdb_traverse_func fn, فراغ *);
صحيح tdb_exists(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_lockall(بنية tdb_context *tdb);
صحيح tdb_lockall_nonblock(بنية tdb_context *tdb);
صحيح tdb_unlockall(بنية tdb_context *tdb);
صحيح tdb_lockall_read(بنية tdb_context *tdb);
صحيح tdb_lockall_read_nonblock(بنية tdb_context *tdb);
صحيح tdb_unlockall_read(بنية tdb_context *tdb);
صحيح tdb_lockall_mark(بنية tdb_context *tdb);
صحيح tdb_lockall_unmark(بنية tdb_context *tdb);
ثابت حرف *tdb_name(بنية tdb_context *tdb);
صحيح tdb_fd(بنية tdb_context *tdb);
tdb_log_func tdb_log_fn(بنية tdb_context *tdb);
فراغ *tdb_get_logging_private(بنية tdb_context *tdb);
صحيح tdb_transaction_start(بنية tdb_context *tdb);
صحيح tdb_transaction_commit(بنية tdb_context *tdb);
صحيح tdb_transaction_cancel(بنية tdb_context *tdb);
صحيح tdb_transaction_recover(بنية tdb_context *tdb);
صحيح tdb_get_seqnum(بنية tdb_context *tdb);
صحيح tdb_hash_size(بنية tdb_context *tdb);
size_t tdb_map_size(بنية tdb_context *tdb);
صحيح tdb_get_flags(بنية tdb_context *tdb);
فراغ tdb_enable_seqnum(بنية tdb_context *tdb);
فراغ tdb_increment_seqnum_nonblock(بنية tdb_context *tdb);
صحيح tdb_flush(بنية tdb_context *tdb);

/* Low level locking functions: use with care */
صحيح tdb_chainlock(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_chainlock_nonblock(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_chainunlock(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_chainlock_read(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_chainunlock_read(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_chainlock_mark(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);
صحيح tdb_chainlock_unmark(بنية tdb_context *tdb, TDB_DATA ئاچقۇچ);

/* Debug functions. Not used in production. */
فراغ tdb_dump_all(بنية tdb_context *tdb);
صحيح tdb_printfreelist(بنية tdb_context *tdb);
صحيح tdb_validate_freelist(بنية tdb_context *tdb, صحيح *pnum_entries);

#ifdef  __cplusplus
}
#endif

#endif /* tdb.h */
