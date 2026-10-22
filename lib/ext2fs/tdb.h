#جے_بنا_تعریف __TDB_H__
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

#جے_تعریف_شدہ  __cplusplus
خارجي "C" {
#جے_انت


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

#تعریف_کرو TDB_ERRCODE(کوڈ, ret) ((tdb->ecode = (کوڈ)), ret)

/* error codes */
تعداد TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_جندرا,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_جندرا_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
تعداد tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

تعريفنوع بنية TDB_انکڑے {
	بنانشانی حرف *dptr;
	size_t dsize;
} TDB_انکڑے;

#جے_بنا_تعریف PRINTF_ATTRIBUTE
#إذا (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#وإلا
#تعریف_کرو PRINTF_ATTRIBUTE(a1, a2)
#جے_انت
#جے_انت

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
#تعریف_کرو tdb_transaction_شروع ext2fs_tdb_transaction_شروع
#تعریف_کرو tdb_transaction_commit ext2fs_tdb_transaction_commit
#تعریف_کرو tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#تعریف_کرو tdb_transaction_recover ext2fs_tdb_transaction_recover
#تعریف_کرو tdb_get_seqnum ext2fs_tdb_get_seqnum
#تعریف_کرو tdb_hash_ناپ ext2fs_tdb_hash_ناپ
#تعریف_کرو tdb_map_ناپ ext2fs_tdb_map_ناپ
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
#تعریف_کرو tdb_جندرا_nonblock ext2fs_tdb_جندرا_nonblock
#تعریف_کرو tdb_lockall_mark ext2fs_tdb_lockall_mark
#تعریف_کرو tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#تعریف_کرو tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#تعریف_کرو tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#تعریف_کرو tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
تعريفنوع بنية tdb_context TDB_CONTEXT;

تعريفنوع صحيح (*tdb_traverse_func)(بنية tdb_context *, TDB_انکڑے, TDB_انکڑے, فراغ *);
تعريفنوع فراغ (*tdb_log_func)(بنية tdb_context *, تعداد tdb_debug_level, ثابت حرف *, ...) PRINTF_ATTRIBUTE(3, 4);
تعريفنوع بنانشانی صحيح (*tdb_hash_func)(TDB_انکڑے *کلید);

بنية tdb_logging_context {
        tdb_log_func log_fn;
        فراغ *log_نجی;
};

بنية tdb_context *tdb_open(ثابت حرف *name, صحيح hash_ناپ, صحيح tdb_flags,
		      صحيح open_flags, mode_t منوال);
بنية tdb_context *tdb_open_ex(ثابت حرف *name, صحيح hash_ناپ, صحيح tdb_flags,
			 صحيح open_flags, mode_t منوال,
			 ثابت بنية tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
فراغ tdb_سیٹ_max_dead(بنية tdb_context *tdb, صحيح max_dead);

صحيح tdb_reopen(بنية tdb_context *tdb);
صحيح tdb_reopen_all(صحيح ماپا_longlived);
فراغ tdb_سیٹ_logging_function(بنية tdb_context *tdb, ثابت بنية tdb_logging_context *log_ctx);
تعداد TDB_ERROR tdb_error(بنية tdb_context *tdb);
ثابت حرف *tdb_errorstr(بنية tdb_context *tdb);
TDB_انکڑے tdb_fetch(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_parse_ریکارڈ(بنية tdb_context *tdb, TDB_انکڑے کلید,
		     صحيح (*parser)(TDB_انکڑے کلید, TDB_انکڑے انکڑے,
				   فراغ *نجی_انکڑے),
		     فراغ *نجی_انکڑے);
صحيح tdb_delete(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_store(بنية tdb_context *tdb, TDB_انکڑے کلید, TDB_انکڑے dbuf, صحيح flag);
صحيح tdb_append(بنية tdb_context *tdb, TDB_انکڑے کلید, TDB_انکڑے new_dbuf);
صحيح tdb_close(بنية tdb_context *tdb);
TDB_انکڑے tdb_firstkey(بنية tdb_context *tdb);
TDB_انکڑے tdb_nextkey(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_traverse(بنية tdb_context *tdb, tdb_traverse_func fn, فراغ *);
صحيح tdb_traverse_read(بنية tdb_context *tdb, tdb_traverse_func fn, فراغ *);
صحيح tdb_exists(بنية tdb_context *tdb, TDB_انکڑے کلید);
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
فراغ *tdb_get_logging_نجی(بنية tdb_context *tdb);
صحيح tdb_transaction_شروع(بنية tdb_context *tdb);
صحيح tdb_transaction_commit(بنية tdb_context *tdb);
صحيح tdb_transaction_cancel(بنية tdb_context *tdb);
صحيح tdb_transaction_recover(بنية tdb_context *tdb);
صحيح tdb_get_seqnum(بنية tdb_context *tdb);
صحيح tdb_hash_ناپ(بنية tdb_context *tdb);
size_t tdb_map_ناپ(بنية tdb_context *tdb);
صحيح tdb_get_flags(بنية tdb_context *tdb);
فراغ tdb_enable_seqnum(بنية tdb_context *tdb);
فراغ tdb_increment_seqnum_nonblock(بنية tdb_context *tdb);
صحيح tdb_flush(بنية tdb_context *tdb);

/* Low level locking functions: use with care */
صحيح tdb_chainlock(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_chainlock_nonblock(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_chainunlock(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_chainlock_read(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_chainunlock_read(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_chainlock_mark(بنية tdb_context *tdb, TDB_انکڑے کلید);
صحيح tdb_chainlock_unmark(بنية tdb_context *tdb, TDB_انکڑے کلید);

/* Debug functions. Not used in production. */
فراغ tdb_dump_all(بنية tdb_context *tdb);
صحيح tdb_printfreelist(بنية tdb_context *tdb);
صحيح tdb_validate_freelist(بنية tdb_context *tdb, صحيح *pnum_entries);

#جے_تعریف_شدہ  __cplusplus
}
#جے_انت

#جے_انت /* tdb.h */
