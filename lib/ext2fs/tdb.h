#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __TDB_H__
#ವ್ಯಾಖ್ಯಾನಿಸು __TDB_H__

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ  __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ


/* flags to tdb_store() */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_REPLACE 1
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ಸೇರಿಸು 2
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_MODIFY 3

/* flags for tdb_open() */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_DEFAULT 0 /* just a readability place holder */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_CLEAR_IF_ಮೊದಲ 1
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ಆಂತರಿಕ 2 /* don't store on disk */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_NOLOCK   4 /* don't do any locking */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_NOMMAP   8 /* don't use mmap */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_CONVERT 16 /* convert endian (internal use) */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_NOSYNC   64 /* don't use synchronous transactions */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_SEQNUM   128 /* maintain a sequence number */

#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ERRCODE(ಸಂಕೇತ_2, ret) ((tdb->ecode = (ಸಂಕೇತ_2)), ret)

/* error codes */
ಎಣಿಕೆ TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_ಬೀಗ,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_ಬೀಗ_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
ಎಣಿಕೆ tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ TDB_ದತ್ತಾಂಶ {
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *dptr;
	size_t dsize;
} TDB_ದತ್ತಾಂಶ;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PRINTF_ATTRIBUTE
#ಆದರೆ (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#ವ್ಯಾಖ್ಯಾನಿಸು PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು PRINTF_ATTRIBUTE(a1, a2)
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

/* ext2fs tdb renames */
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_open ext2fs_tdb_open
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_open_ex ext2fs_tdb_open_ex
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_ಗಣ_max_dead ext2fs_tdb_ಗಣ_max_dead
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_reopen ext2fs_tdb_reopen
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_reopen_all ext2fs_tdb_reopen_all
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_ಗಣ_logging_function ext2fs_tdb_ಗಣ_logging_function
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_error ext2fs_tdb_error
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_errorstr ext2fs_tdb_errorstr
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_fetch ext2fs_tdb_fetch
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_parse_ದಾಖಲೆ ext2fs_tdb_parse_ದಾಖಲೆ
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_delete ext2fs_tdb_delete
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_store ext2fs_tdb_store
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_append ext2fs_tdb_append
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_close ext2fs_tdb_close
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_firstkey ext2fs_tdb_firstkey
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_nextkey ext2fs_tdb_nextkey
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_traverse ext2fs_tdb_traverse
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_traverse_read ext2fs_tdb_traverse_read
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_exists ext2fs_tdb_exists
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_lockall ext2fs_tdb_lockall
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_unlockall ext2fs_tdb_unlockall
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_lockall_read ext2fs_tdb_lockall_read
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_unlockall_read ext2fs_tdb_unlockall_read
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_name ext2fs_tdb_name
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_fd ext2fs_tdb_fd
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_log_fn ext2fs_tdb_log_fn
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_get_logging_ಖಾಸಗಿ ext2fs_tdb_get_logging_ಖಾಸಗಿ
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_transaction_ಆರಂಭ ext2fs_tdb_transaction_ಆರಂಭ
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_transaction_commit ext2fs_tdb_transaction_commit
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_transaction_recover ext2fs_tdb_transaction_recover
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_get_seqnum ext2fs_tdb_get_seqnum
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_hash_ಗಾತ್ರ ext2fs_tdb_hash_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_map_ಗಾತ್ರ ext2fs_tdb_map_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_get_flags ext2fs_tdb_get_flags
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainlock ext2fs_tdb_chainlock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainunlock ext2fs_tdb_chainunlock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainlock_read ext2fs_tdb_chainlock_read
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_dump_all ext2fs_tdb_dump_all
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_printfreelist ext2fs_tdb_printfreelist
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_validate_freelist ext2fs_tdb_validate_freelist
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_ಬೀಗ_nonblock ext2fs_tdb_ಬೀಗ_nonblock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_lockall_mark ext2fs_tdb_lockall_mark
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#ವ್ಯಾಖ್ಯಾನಿಸು tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ tdb_context TDB_CONTEXT;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*tdb_traverse_func)(ರಚನೆ tdb_context *, TDB_ದತ್ತಾಂಶ, TDB_ದತ್ತಾಂಶ, ಶೂನ್ಯ *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*tdb_log_func)(ರಚನೆ tdb_context *, ಎಣಿಕೆ tdb_debug_level, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *, ...) PRINTF_ATTRIBUTE(3, 4);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ (*tdb_hash_func)(TDB_ದತ್ತಾಂಶ *ಕೀಲಿ);

ರಚನೆ tdb_logging_context {
        tdb_log_func log_fn;
        ಶೂನ್ಯ *log_ಖಾಸಗಿ;
};

ರಚನೆ tdb_context *tdb_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ hash_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ tdb_flags,
		      ಪೂರ್ಣಾಂಕ open_flags, mode_t ಬಹುಲಕ);
ರಚನೆ tdb_context *tdb_open_ex(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ hash_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ tdb_flags,
			 ಪೂರ್ಣಾಂಕ open_flags, mode_t ಬಹುಲಕ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
ಶೂನ್ಯ tdb_ಗಣ_max_dead(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ max_dead);

ಪೂರ್ಣಾಂಕ tdb_reopen(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_reopen_all(ಪೂರ್ಣಾಂಕ ಪೋಷಕ_longlived);
ಶೂನ್ಯ tdb_ಗಣ_logging_function(ರಚನೆ tdb_context *tdb, ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_logging_context *log_ctx);
ಎಣಿಕೆ TDB_ERROR tdb_error(ರಚನೆ tdb_context *tdb);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tdb_errorstr(ರಚನೆ tdb_context *tdb);
TDB_ದತ್ತಾಂಶ tdb_fetch(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_parse_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ,
		     ಪೂರ್ಣಾಂಕ (*parser)(TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ ದತ್ತಾಂಶ,
				   ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ),
		     ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ);
ಪೂರ್ಣಾಂಕ tdb_delete(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_store(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಪೂರ್ಣಾಂಕ flag);
ಪೂರ್ಣಾಂಕ tdb_append(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ new_dbuf);
ಪೂರ್ಣಾಂಕ tdb_close(ರಚನೆ tdb_context *tdb);
TDB_ದತ್ತಾಂಶ tdb_firstkey(ರಚನೆ tdb_context *tdb);
TDB_ದತ್ತಾಂಶ tdb_nextkey(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_traverse(ರಚನೆ tdb_context *tdb, tdb_traverse_func fn, ಶೂನ್ಯ *);
ಪೂರ್ಣಾಂಕ tdb_traverse_read(ರಚನೆ tdb_context *tdb, tdb_traverse_func fn, ಶೂನ್ಯ *);
ಪೂರ್ಣಾಂಕ tdb_exists(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_lockall(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_lockall_nonblock(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_unlockall(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_lockall_read(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_lockall_read_nonblock(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_unlockall_read(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_lockall_mark(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_lockall_unmark(ರಚನೆ tdb_context *tdb);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tdb_name(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_fd(ರಚನೆ tdb_context *tdb);
tdb_log_func tdb_log_fn(ರಚನೆ tdb_context *tdb);
ಶೂನ್ಯ *tdb_get_logging_ಖಾಸಗಿ(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_transaction_ಆರಂಭ(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_transaction_commit(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_transaction_cancel(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_transaction_recover(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_get_seqnum(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_hash_ಗಾತ್ರ(ರಚನೆ tdb_context *tdb);
size_t tdb_map_ಗಾತ್ರ(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_get_flags(ರಚನೆ tdb_context *tdb);
ಶೂನ್ಯ tdb_enable_seqnum(ರಚನೆ tdb_context *tdb);
ಶೂನ್ಯ tdb_increment_seqnum_nonblock(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_flush(ರಚನೆ tdb_context *tdb);

/* Low level locking functions: use with care */
ಪೂರ್ಣಾಂಕ tdb_chainlock(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_chainlock_nonblock(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_chainunlock(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_chainlock_read(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_chainunlock_read(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_chainlock_mark(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);
ಪೂರ್ಣಾಂಕ tdb_chainlock_unmark(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ);

/* Debug functions. Not used in production. */
ಶೂನ್ಯ tdb_dump_all(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_printfreelist(ರಚನೆ tdb_context *tdb);
ಪೂರ್ಣಾಂಕ tdb_validate_freelist(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ *pnum_entries);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ  __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* tdb.h */
