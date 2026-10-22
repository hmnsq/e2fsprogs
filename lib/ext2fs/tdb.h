#اگر_غیر_تعریف_شدہ __TDB_H__
#تعریف_کرو __TDB_H__

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

#اگر_تعریف_شدہ  __cplusplus
خارجی "C" {
#اگر_ختم


/* flags to tdb_store() */
#تعریف_کرو TDB_REPLACE 1
#تعریف_کرو TDB_داخل_کرو 2
#تعریف_کرو TDB_MODIFY 3

/* flags for tdb_open() */
#تعریف_کرو TDB_DEFAULT 0 /* just a readability place holder */
#تعریف_کرو TDB_CLEAR_IF_پہلا 1
#تعریف_کرو TDB_اندرونی 2 /* don't store on disk */
#تعریف_کرو TDB_NOLOCK   4 /* don't do any locking */
#تعریف_کرو TDB_NOMMAP   8 /* don't use mmap */
#تعریف_کرو TDB_CONVERT 16 /* convert endian (internal use) */
#تعریف_کرو TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#تعریف_کرو TDB_NOSYNC   64 /* don't use synchronous transactions */
#تعریف_کرو TDB_SEQNUM   128 /* maintain a sequence number */

#تعریف_کرو TDB_ERRCODE(رمز, ret) ((tdb->ecode = (رمز)), ret)

/* error codes */
شمار TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_تالا,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_تالا_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
شمار tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

قسمتعریف ساخت TDB_ڈیٹا {
	غیرعلامتی حرف *dptr;
	size_t dsize;
} TDB_ڈیٹا;

#اگر_غیر_تعریف_شدہ PRINTF_ATTRIBUTE
#اگر (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ورنہ
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2)
#اگر_ختم
#اگر_ختم

/* ext2fs tdb renames */
#تعریف_کرو tdb_open ext2fs_tdb_open
#تعریف_کرو tdb_open_ex ext2fs_tdb_open_ex
#تعریف_کرو tdb_سیٹ_max_dead ext2fs_tdb_سیٹ_max_dead
#تعریف_کرو tdb_reopen ext2fs_tdb_reopen
#تعریف_کرو tdb_reopen_all ext2fs_tdb_reopen_all
#تعریف_کرو tdb_سیٹ_logging_function ext2fs_tdb_سیٹ_logging_function
#تعریف_کرو tdb_error ext2fs_tdb_error
#تعریف_کرو tdb_errorstr ext2fs_tdb_errorstr
#تعریف_کرو tdb_fetch ext2fs_tdb_fetch
#تعریف_کرو tdb_parse_ریکارڈ ext2fs_tdb_parse_ریکارڈ
#تعریف_کرو tdb_delete ext2fs_tdb_delete
#تعریف_کرو tdb_store ext2fs_tdb_store
#تعریف_کرو tdb_append ext2fs_tdb_append
#تعریف_کرو tdb_close ext2fs_tdb_close
#تعریف_کرو tdb_firstkey ext2fs_tdb_firstkey
#تعریف_کرو tdb_nextkey ext2fs_tdb_nextkey
#تعریف_کرو tdb_traverse ext2fs_tdb_traverse
#تعریف_کرو tdb_traverse_read ext2fs_tdb_traverse_read
#تعریف_کرو tdb_exists ext2fs_tdb_exists
#تعریف_کرو tdb_lockall ext2fs_tdb_lockall
#تعریف_کرو tdb_unlockall ext2fs_tdb_unlockall
#تعریف_کرو tdb_lockall_read ext2fs_tdb_lockall_read
#تعریف_کرو tdb_unlockall_read ext2fs_tdb_unlockall_read
#تعریف_کرو tdb_name ext2fs_tdb_name
#تعریف_کرو tdb_fd ext2fs_tdb_fd
#تعریف_کرو tdb_log_fn ext2fs_tdb_log_fn
#تعریف_کرو tdb_get_logging_نجی ext2fs_tdb_get_logging_نجی
#تعریف_کرو tdb_transaction_آغاز ext2fs_tdb_transaction_آغاز
#تعریف_کرو tdb_transaction_commit ext2fs_tdb_transaction_commit
#تعریف_کرو tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#تعریف_کرو tdb_transaction_recover ext2fs_tdb_transaction_recover
#تعریف_کرو tdb_get_seqnum ext2fs_tdb_get_seqnum
#تعریف_کرو tdb_hash_سائز ext2fs_tdb_hash_سائز
#تعریف_کرو tdb_map_سائز ext2fs_tdb_map_سائز
#تعریف_کرو tdb_get_flags ext2fs_tdb_get_flags
#تعریف_کرو tdb_chainlock ext2fs_tdb_chainlock
#تعریف_کرو tdb_chainunlock ext2fs_tdb_chainunlock
#تعریف_کرو tdb_chainlock_read ext2fs_tdb_chainlock_read
#تعریف_کرو tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#تعریف_کرو tdb_dump_all ext2fs_tdb_dump_all
#تعریف_کرو tdb_printfreelist ext2fs_tdb_printfreelist
#تعریف_کرو tdb_validate_freelist ext2fs_tdb_validate_freelist
#تعریف_کرو tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#تعریف_کرو tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#تعریف_کرو tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#تعریف_کرو tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#تعریف_کرو tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#تعریف_کرو tdb_تالا_nonblock ext2fs_tdb_تالا_nonblock
#تعریف_کرو tdb_lockall_mark ext2fs_tdb_lockall_mark
#تعریف_کرو tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#تعریف_کرو tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#تعریف_کرو tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#تعریف_کرو tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
قسمتعریف ساخت tdb_context TDB_CONTEXT;

قسمتعریف صحیح (*tdb_traverse_func)(ساخت tdb_context *, TDB_ڈیٹا, TDB_ڈیٹا, خالی *);
قسمتعریف خالی (*tdb_log_func)(ساخت tdb_context *, شمار tdb_debug_level, ثابت حرف *, ...) PRINTF_ATTRIBUTE(3, 4);
قسمتعریف غیرعلامتی صحیح (*tdb_hash_func)(TDB_ڈیٹا *کلید);

ساخت tdb_logging_context {
        tdb_log_func log_fn;
        خالی *log_نجی;
};

ساخت tdb_context *tdb_open(ثابت حرف *name, صحیح hash_سائز, صحیح tdb_flags,
		      صحیح open_flags, mode_t منوال);
ساخت tdb_context *tdb_open_ex(ثابت حرف *name, صحیح hash_سائز, صحیح tdb_flags,
			 صحیح open_flags, mode_t منوال,
			 ثابت ساخت tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
خالی tdb_سیٹ_max_dead(ساخت tdb_context *tdb, صحیح max_dead);

صحیح tdb_reopen(ساخت tdb_context *tdb);
صحیح tdb_reopen_all(صحیح والد_longlived);
خالی tdb_سیٹ_logging_function(ساخت tdb_context *tdb, ثابت ساخت tdb_logging_context *log_ctx);
شمار TDB_ERROR tdb_error(ساخت tdb_context *tdb);
ثابت حرف *tdb_errorstr(ساخت tdb_context *tdb);
TDB_ڈیٹا tdb_fetch(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_parse_ریکارڈ(ساخت tdb_context *tdb, TDB_ڈیٹا کلید,
		     صحیح (*parser)(TDB_ڈیٹا کلید, TDB_ڈیٹا ڈیٹا,
				   خالی *نجی_ڈیٹا),
		     خالی *نجی_ڈیٹا);
صحیح tdb_delete(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_store(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, صحیح flag);
صحیح tdb_append(ساخت tdb_context *tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا new_dbuf);
صحیح tdb_close(ساخت tdb_context *tdb);
TDB_ڈیٹا tdb_firstkey(ساخت tdb_context *tdb);
TDB_ڈیٹا tdb_nextkey(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_traverse(ساخت tdb_context *tdb, tdb_traverse_func fn, خالی *);
صحیح tdb_traverse_read(ساخت tdb_context *tdb, tdb_traverse_func fn, خالی *);
صحیح tdb_exists(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_lockall(ساخت tdb_context *tdb);
صحیح tdb_lockall_nonblock(ساخت tdb_context *tdb);
صحیح tdb_unlockall(ساخت tdb_context *tdb);
صحیح tdb_lockall_read(ساخت tdb_context *tdb);
صحیح tdb_lockall_read_nonblock(ساخت tdb_context *tdb);
صحیح tdb_unlockall_read(ساخت tdb_context *tdb);
صحیح tdb_lockall_mark(ساخت tdb_context *tdb);
صحیح tdb_lockall_unmark(ساخت tdb_context *tdb);
ثابت حرف *tdb_name(ساخت tdb_context *tdb);
صحیح tdb_fd(ساخت tdb_context *tdb);
tdb_log_func tdb_log_fn(ساخت tdb_context *tdb);
خالی *tdb_get_logging_نجی(ساخت tdb_context *tdb);
صحیح tdb_transaction_آغاز(ساخت tdb_context *tdb);
صحیح tdb_transaction_commit(ساخت tdb_context *tdb);
صحیح tdb_transaction_cancel(ساخت tdb_context *tdb);
صحیح tdb_transaction_recover(ساخت tdb_context *tdb);
صحیح tdb_get_seqnum(ساخت tdb_context *tdb);
صحیح tdb_hash_سائز(ساخت tdb_context *tdb);
size_t tdb_map_سائز(ساخت tdb_context *tdb);
صحیح tdb_get_flags(ساخت tdb_context *tdb);
خالی tdb_enable_seqnum(ساخت tdb_context *tdb);
خالی tdb_increment_seqnum_nonblock(ساخت tdb_context *tdb);
صحیح tdb_flush(ساخت tdb_context *tdb);

/* Low level locking functions: use with care */
صحیح tdb_chainlock(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_chainlock_nonblock(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_chainunlock(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_chainlock_read(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_chainunlock_read(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_chainlock_mark(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);
صحیح tdb_chainlock_unmark(ساخت tdb_context *tdb, TDB_ڈیٹا کلید);

/* Debug functions. Not used in production. */
خالی tdb_dump_all(ساخت tdb_context *tdb);
صحیح tdb_printfreelist(ساخت tdb_context *tdb);
صحیح tdb_validate_freelist(ساخت tdb_context *tdb, صحیح *pnum_entries);

#اگر_تعریف_شدہ  __cplusplus
}
#اگر_ختم

#اگر_ختم /* tdb.h */
