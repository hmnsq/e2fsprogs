#جيڪڏهن_وصف_نه_ٿيل __TDB_H__
#وصف_ڪريو __TDB_H__

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

#جيڪڏهن_وصف_ٿيل  __cplusplus
خارجي "C" {
#جيڪڏهن_پورو


/* flags to tdb_store() */
#وصف_ڪريو TDB_REPLACE 1
#وصف_ڪريو TDB_وجهو 2
#وصف_ڪريو TDB_MODIFY 3

/* flags for tdb_open() */
#وصف_ڪريو TDB_DEFAULT 0 /* just a readability place holder */
#وصف_ڪريو TDB_CLEAR_IF_پهريون 1
#وصف_ڪريو TDB_اندروني 2 /* don't store on disk */
#وصف_ڪريو TDB_NOLOCK   4 /* don't do any locking */
#وصف_ڪريو TDB_NOMMAP   8 /* don't use mmap */
#وصف_ڪريو TDB_CONVERT 16 /* convert endian (internal use) */
#وصف_ڪريو TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#وصف_ڪريو TDB_NOSYNC   64 /* don't use synchronous transactions */
#وصف_ڪريو TDB_SEQNUM   128 /* maintain a sequence number */

#وصف_ڪريو TDB_ERRCODE(ڪوڊ, ret) ((tdb->ecode = (ڪوڊ)), ret)

/* error codes */
تعداد TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_تالو,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_تالو_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
تعداد tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

تعريفنوع بنية TDB_معلومات {
	بنانشان حرف *dptr;
	size_t dsize;
} TDB_معلومات;

#جيڪڏهن_وصف_نه_ٿيل PRINTF_ATTRIBUTE
#إذا (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#وصف_ڪريو PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وإلا
#وصف_ڪريو PRINTF_ATTRIBUTE(a1, a2)
#جيڪڏهن_پورو
#جيڪڏهن_پورو

/* ext2fs tdb renames */
#وصف_ڪريو tdb_open ext2fs_tdb_open
#وصف_ڪريو tdb_open_ex ext2fs_tdb_open_ex
#وصف_ڪريو tdb_سيٽ_max_dead ext2fs_tdb_سيٽ_max_dead
#وصف_ڪريو tdb_reopen ext2fs_tdb_reopen
#وصف_ڪريو tdb_reopen_all ext2fs_tdb_reopen_all
#وصف_ڪريو tdb_سيٽ_logging_function ext2fs_tdb_سيٽ_logging_function
#وصف_ڪريو tdb_error ext2fs_tdb_error
#وصف_ڪريو tdb_errorstr ext2fs_tdb_errorstr
#وصف_ڪريو tdb_fetch ext2fs_tdb_fetch
#وصف_ڪريو tdb_parse_رڪارڊ ext2fs_tdb_parse_رڪارڊ
#وصف_ڪريو tdb_delete ext2fs_tdb_delete
#وصف_ڪريو tdb_store ext2fs_tdb_store
#وصف_ڪريو tdb_append ext2fs_tdb_append
#وصف_ڪريو tdb_close ext2fs_tdb_close
#وصف_ڪريو tdb_firstkey ext2fs_tdb_firstkey
#وصف_ڪريو tdb_nextkey ext2fs_tdb_nextkey
#وصف_ڪريو tdb_traverse ext2fs_tdb_traverse
#وصف_ڪريو tdb_traverse_read ext2fs_tdb_traverse_read
#وصف_ڪريو tdb_exists ext2fs_tdb_exists
#وصف_ڪريو tdb_lockall ext2fs_tdb_lockall
#وصف_ڪريو tdb_unlockall ext2fs_tdb_unlockall
#وصف_ڪريو tdb_lockall_read ext2fs_tdb_lockall_read
#وصف_ڪريو tdb_unlockall_read ext2fs_tdb_unlockall_read
#وصف_ڪريو tdb_name ext2fs_tdb_name
#وصف_ڪريو tdb_fd ext2fs_tdb_fd
#وصف_ڪريو tdb_log_fn ext2fs_tdb_log_fn
#وصف_ڪريو tdb_get_logging_ذاتي ext2fs_tdb_get_logging_ذاتي
#وصف_ڪريو tdb_transaction_شروعات ext2fs_tdb_transaction_شروعات
#وصف_ڪريو tdb_transaction_commit ext2fs_tdb_transaction_commit
#وصف_ڪريو tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#وصف_ڪريو tdb_transaction_recover ext2fs_tdb_transaction_recover
#وصف_ڪريو tdb_get_seqnum ext2fs_tdb_get_seqnum
#وصف_ڪريو tdb_hash_سائيز ext2fs_tdb_hash_سائيز
#وصف_ڪريو tdb_map_سائيز ext2fs_tdb_map_سائيز
#وصف_ڪريو tdb_get_flags ext2fs_tdb_get_flags
#وصف_ڪريو tdb_chainlock ext2fs_tdb_chainlock
#وصف_ڪريو tdb_chainunlock ext2fs_tdb_chainunlock
#وصف_ڪريو tdb_chainlock_read ext2fs_tdb_chainlock_read
#وصف_ڪريو tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#وصف_ڪريو tdb_dump_all ext2fs_tdb_dump_all
#وصف_ڪريو tdb_printfreelist ext2fs_tdb_printfreelist
#وصف_ڪريو tdb_validate_freelist ext2fs_tdb_validate_freelist
#وصف_ڪريو tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#وصف_ڪريو tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#وصف_ڪريو tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#وصف_ڪريو tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#وصف_ڪريو tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#وصف_ڪريو tdb_تالو_nonblock ext2fs_tdb_تالو_nonblock
#وصف_ڪريو tdb_lockall_mark ext2fs_tdb_lockall_mark
#وصف_ڪريو tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#وصف_ڪريو tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#وصف_ڪريو tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#وصف_ڪريو tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
تعريفنوع بنية tdb_context TDB_CONTEXT;

تعريفنوع صحيح (*tdb_traverse_func)(بنية tdb_context *, TDB_معلومات, TDB_معلومات, فراغ *);
تعريفنوع فراغ (*tdb_log_func)(بنية tdb_context *, تعداد tdb_debug_level, ثابت حرف *, ...) PRINTF_ATTRIBUTE(3, 4);
تعريفنوع بنانشان صحيح (*tdb_hash_func)(TDB_معلومات *ڪنجي);

بنية tdb_logging_context {
        tdb_log_func log_fn;
        فراغ *log_ذاتي;
};

بنية tdb_context *tdb_open(ثابت حرف *name, صحيح hash_سائيز, صحيح tdb_flags,
		      صحيح open_flags, mode_t گهڻائي);
بنية tdb_context *tdb_open_ex(ثابت حرف *name, صحيح hash_سائيز, صحيح tdb_flags,
			 صحيح open_flags, mode_t گهڻائي,
			 ثابت بنية tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
فراغ tdb_سيٽ_max_dead(بنية tdb_context *tdb, صحيح max_dead);

صحيح tdb_reopen(بنية tdb_context *tdb);
صحيح tdb_reopen_all(صحيح والد_longlived);
فراغ tdb_سيٽ_logging_function(بنية tdb_context *tdb, ثابت بنية tdb_logging_context *log_ctx);
تعداد TDB_ERROR tdb_error(بنية tdb_context *tdb);
ثابت حرف *tdb_errorstr(بنية tdb_context *tdb);
TDB_معلومات tdb_fetch(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_parse_رڪارڊ(بنية tdb_context *tdb, TDB_معلومات ڪنجي,
		     صحيح (*parser)(TDB_معلومات ڪنجي, TDB_معلومات معلومات,
				   فراغ *ذاتي_معلومات),
		     فراغ *ذاتي_معلومات);
صحيح tdb_delete(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_store(بنية tdb_context *tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, صحيح flag);
صحيح tdb_append(بنية tdb_context *tdb, TDB_معلومات ڪنجي, TDB_معلومات new_dbuf);
صحيح tdb_close(بنية tdb_context *tdb);
TDB_معلومات tdb_firstkey(بنية tdb_context *tdb);
TDB_معلومات tdb_nextkey(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_traverse(بنية tdb_context *tdb, tdb_traverse_func fn, فراغ *);
صحيح tdb_traverse_read(بنية tdb_context *tdb, tdb_traverse_func fn, فراغ *);
صحيح tdb_exists(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
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
فراغ *tdb_get_logging_ذاتي(بنية tdb_context *tdb);
صحيح tdb_transaction_شروعات(بنية tdb_context *tdb);
صحيح tdb_transaction_commit(بنية tdb_context *tdb);
صحيح tdb_transaction_cancel(بنية tdb_context *tdb);
صحيح tdb_transaction_recover(بنية tdb_context *tdb);
صحيح tdb_get_seqnum(بنية tdb_context *tdb);
صحيح tdb_hash_سائيز(بنية tdb_context *tdb);
size_t tdb_map_سائيز(بنية tdb_context *tdb);
صحيح tdb_get_flags(بنية tdb_context *tdb);
فراغ tdb_enable_seqnum(بنية tdb_context *tdb);
فراغ tdb_increment_seqnum_nonblock(بنية tdb_context *tdb);
صحيح tdb_flush(بنية tdb_context *tdb);

/* Low level locking functions: use with care */
صحيح tdb_chainlock(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_chainlock_nonblock(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_chainunlock(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_chainlock_read(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_chainunlock_read(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_chainlock_mark(بنية tdb_context *tdb, TDB_معلومات ڪنجي);
صحيح tdb_chainlock_unmark(بنية tdb_context *tdb, TDB_معلومات ڪنجي);

/* Debug functions. Not used in production. */
فراغ tdb_dump_all(بنية tdb_context *tdb);
صحيح tdb_printfreelist(بنية tdb_context *tdb);
صحيح tdb_validate_freelist(بنية tdb_context *tdb, صحيح *pnum_entries);

#جيڪڏهن_وصف_ٿيل  __cplusplus
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* tdb.h */
