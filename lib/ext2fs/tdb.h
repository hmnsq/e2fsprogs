#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __TDB_H__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __TDB_H__

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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ  __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ


/* flags to tdb_store() */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_REPLACE 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ਪਾਓ 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_MODIFY 3

/* flags for tdb_open() */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_DEFAULT 0 /* just a readability place holder */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_CLEAR_IF_ਪਹਿਲਾ 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ਅੰਦਰੂਨੀ 2 /* don't store on disk */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_NOLOCK   4 /* don't do any locking */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_NOMMAP   8 /* don't use mmap */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_CONVERT 16 /* convert endian (internal use) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_NOSYNC   64 /* don't use synchronous transactions */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_SEQNUM   128 /* maintain a sequence number */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ERRCODE(ਕੋਡ, ret) ((tdb->ecode = (ਕੋਡ)), ret)

/* error codes */
ਗਿਣਤੀ TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_ਤਾਲਾ,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_ਤਾਲਾ_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
ਗਿਣਤੀ tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ TDB_ਅੰਕੜੇ {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *dptr;
	size_t dsize;
} TDB_ਅੰਕੜੇ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PRINTF_ATTRIBUTE
#ਜੇ (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINTF_ATTRIBUTE(a1, a2)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* ext2fs tdb renames */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_open ext2fs_tdb_open
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_open_ex ext2fs_tdb_open_ex
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_ਸੈੱਟ_max_dead ext2fs_tdb_ਸੈੱਟ_max_dead
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_reopen ext2fs_tdb_reopen
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_reopen_all ext2fs_tdb_reopen_all
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_ਸੈੱਟ_logging_function ext2fs_tdb_ਸੈੱਟ_logging_function
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_error ext2fs_tdb_error
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_errorstr ext2fs_tdb_errorstr
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_fetch ext2fs_tdb_fetch
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_parse_ਰਿਕਾਰਡ ext2fs_tdb_parse_ਰਿਕਾਰਡ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_delete ext2fs_tdb_delete
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_store ext2fs_tdb_store
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_append ext2fs_tdb_append
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_close ext2fs_tdb_close
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_firstkey ext2fs_tdb_firstkey
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_nextkey ext2fs_tdb_nextkey
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_traverse ext2fs_tdb_traverse
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_traverse_read ext2fs_tdb_traverse_read
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_exists ext2fs_tdb_exists
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_lockall ext2fs_tdb_lockall
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_unlockall ext2fs_tdb_unlockall
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_lockall_read ext2fs_tdb_lockall_read
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_unlockall_read ext2fs_tdb_unlockall_read
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_name ext2fs_tdb_name
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_fd ext2fs_tdb_fd
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_log_fn ext2fs_tdb_log_fn
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_get_logging_ਨਿੱਜੀ ext2fs_tdb_get_logging_ਨਿੱਜੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_transaction_ਸ਼ੁਰੂ ext2fs_tdb_transaction_ਸ਼ੁਰੂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_transaction_commit ext2fs_tdb_transaction_commit
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_transaction_recover ext2fs_tdb_transaction_recover
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_get_seqnum ext2fs_tdb_get_seqnum
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_hash_ਮਾਪ ext2fs_tdb_hash_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_map_ਮਾਪ ext2fs_tdb_map_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_get_flags ext2fs_tdb_get_flags
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainlock ext2fs_tdb_chainlock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainunlock ext2fs_tdb_chainunlock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainlock_read ext2fs_tdb_chainlock_read
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_dump_all ext2fs_tdb_dump_all
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_printfreelist ext2fs_tdb_printfreelist
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_validate_freelist ext2fs_tdb_validate_freelist
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_ਤਾਲਾ_nonblock ext2fs_tdb_ਤਾਲਾ_nonblock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_lockall_mark ext2fs_tdb_lockall_mark
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ tdb_context TDB_CONTEXT;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*tdb_traverse_func)(ਬਣਤਰ tdb_context *, TDB_ਅੰਕੜੇ, TDB_ਅੰਕੜੇ, ਖਾਲੀ *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*tdb_log_func)(ਬਣਤਰ tdb_context *, ਗਿਣਤੀ tdb_debug_level, ਅਚਲ ਅੱਖਰ *, ...) PRINTF_ATTRIBUTE(3, 4);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ (*tdb_hash_func)(TDB_ਅੰਕੜੇ *ਕੁੰਜੀ);

ਬਣਤਰ tdb_logging_context {
        tdb_log_func log_fn;
        ਖਾਲੀ *log_ਨਿੱਜੀ;
};

ਬਣਤਰ tdb_context *tdb_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ hash_ਮਾਪ, ਪੂਰਨ tdb_flags,
		      ਪੂਰਨ open_flags, mode_t ਬਹੁਲਕ);
ਬਣਤਰ tdb_context *tdb_open_ex(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ hash_ਮਾਪ, ਪੂਰਨ tdb_flags,
			 ਪੂਰਨ open_flags, mode_t ਬਹੁਲਕ,
			 ਅਚਲ ਬਣਤਰ tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
ਖਾਲੀ tdb_ਸੈੱਟ_max_dead(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ max_dead);

ਪੂਰਨ tdb_reopen(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_reopen_all(ਪੂਰਨ ਮਾਪਾ_longlived);
ਖਾਲੀ tdb_ਸੈੱਟ_logging_function(ਬਣਤਰ tdb_context *tdb, ਅਚਲ ਬਣਤਰ tdb_logging_context *log_ctx);
ਗਿਣਤੀ TDB_ERROR tdb_error(ਬਣਤਰ tdb_context *tdb);
ਅਚਲ ਅੱਖਰ *tdb_errorstr(ਬਣਤਰ tdb_context *tdb);
TDB_ਅੰਕੜੇ tdb_fetch(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_parse_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ,
		     ਪੂਰਨ (*parser)(TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ ਅੰਕੜੇ,
				   ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ),
		     ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ);
ਪੂਰਨ tdb_delete(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_store(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਪੂਰਨ flag);
ਪੂਰਨ tdb_append(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ new_dbuf);
ਪੂਰਨ tdb_close(ਬਣਤਰ tdb_context *tdb);
TDB_ਅੰਕੜੇ tdb_firstkey(ਬਣਤਰ tdb_context *tdb);
TDB_ਅੰਕੜੇ tdb_nextkey(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_traverse(ਬਣਤਰ tdb_context *tdb, tdb_traverse_func fn, ਖਾਲੀ *);
ਪੂਰਨ tdb_traverse_read(ਬਣਤਰ tdb_context *tdb, tdb_traverse_func fn, ਖਾਲੀ *);
ਪੂਰਨ tdb_exists(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_lockall(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_lockall_nonblock(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_unlockall(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_lockall_read(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_lockall_read_nonblock(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_unlockall_read(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_lockall_mark(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_lockall_unmark(ਬਣਤਰ tdb_context *tdb);
ਅਚਲ ਅੱਖਰ *tdb_name(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_fd(ਬਣਤਰ tdb_context *tdb);
tdb_log_func tdb_log_fn(ਬਣਤਰ tdb_context *tdb);
ਖਾਲੀ *tdb_get_logging_ਨਿੱਜੀ(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_transaction_ਸ਼ੁਰੂ(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_transaction_commit(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_transaction_cancel(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_transaction_recover(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_get_seqnum(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_hash_ਮਾਪ(ਬਣਤਰ tdb_context *tdb);
size_t tdb_map_ਮਾਪ(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_get_flags(ਬਣਤਰ tdb_context *tdb);
ਖਾਲੀ tdb_enable_seqnum(ਬਣਤਰ tdb_context *tdb);
ਖਾਲੀ tdb_increment_seqnum_nonblock(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_flush(ਬਣਤਰ tdb_context *tdb);

/* Low level locking functions: use with care */
ਪੂਰਨ tdb_chainlock(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_chainlock_nonblock(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_chainunlock(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_chainlock_read(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_chainunlock_read(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_chainlock_mark(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);
ਪੂਰਨ tdb_chainlock_unmark(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ);

/* Debug functions. Not used in production. */
ਖਾਲੀ tdb_dump_all(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_printfreelist(ਬਣਤਰ tdb_context *tdb);
ਪੂਰਨ tdb_validate_freelist(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ *pnum_entries);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ  __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* tdb.h */
