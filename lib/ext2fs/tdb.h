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
બાહ્ય "C" {
#endif


/* flags to tdb_store() */
#define TDB_REPLACE 1
#define TDB_ઉમેરો 2
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
ગણના TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_LOCK,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_LOCK_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
ગણના tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

પ્રકારવ્યાખ્યા સંરચના TDB_DATA {
	unsigned અક્ષર *dptr;
	size_t dsize;
} TDB_DATA;

#ifndef PRINTF_ATTRIBUTE
#જો (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#define PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#નહીંતર
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
પ્રકારવ્યાખ્યા સંરચના tdb_context TDB_CONTEXT;

પ્રકારવ્યાખ્યા પૂર્ણાંક (*tdb_traverse_func)(સંરચના tdb_context *, TDB_DATA, TDB_DATA, શૂન્ય *);
પ્રકારવ્યાખ્યા શૂન્ય (*tdb_log_func)(સંરચના tdb_context *, ગણના tdb_debug_level, અચલ અક્ષર *, ...) PRINTF_ATTRIBUTE(3, 4);
પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક (*tdb_hash_func)(TDB_DATA *ચાવી);

સંરચના tdb_logging_context {
        tdb_log_func log_fn;
        શૂન્ય *log_private;
};

સંરચના tdb_context *tdb_open(અચલ અક્ષર *name, પૂર્ણાંક hash_size, પૂર્ણાંક tdb_flags,
		      પૂર્ણાંક open_flags, mode_t mode);
સંરચના tdb_context *tdb_open_ex(અચલ અક્ષર *name, પૂર્ણાંક hash_size, પૂર્ણાંક tdb_flags,
			 પૂર્ણાંક open_flags, mode_t mode,
			 અચલ સંરચના tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
શૂન્ય tdb_set_max_dead(સંરચના tdb_context *tdb, પૂર્ણાંક max_dead);

પૂર્ણાંક tdb_reopen(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_reopen_all(પૂર્ણાંક parent_longlived);
શૂન્ય tdb_set_logging_function(સંરચના tdb_context *tdb, અચલ સંરચના tdb_logging_context *log_ctx);
ગણના TDB_ERROR tdb_error(સંરચના tdb_context *tdb);
અચલ અક્ષર *tdb_errorstr(સંરચના tdb_context *tdb);
TDB_DATA tdb_fetch(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_parse_record(સંરચના tdb_context *tdb, TDB_DATA ચાવી,
		     પૂર્ણાંક (*parser)(TDB_DATA ચાવી, TDB_DATA data,
				   શૂન્ય *private_data),
		     શૂન્ય *private_data);
પૂર્ણાંક tdb_delete(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_store(સંરચના tdb_context *tdb, TDB_DATA ચાવી, TDB_DATA dbuf, પૂર્ણાંક flag);
પૂર્ણાંક tdb_append(સંરચના tdb_context *tdb, TDB_DATA ચાવી, TDB_DATA new_dbuf);
પૂર્ણાંક tdb_close(સંરચના tdb_context *tdb);
TDB_DATA tdb_firstkey(સંરચના tdb_context *tdb);
TDB_DATA tdb_nextkey(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_traverse(સંરચના tdb_context *tdb, tdb_traverse_func fn, શૂન્ય *);
પૂર્ણાંક tdb_traverse_read(સંરચના tdb_context *tdb, tdb_traverse_func fn, શૂન્ય *);
પૂર્ણાંક tdb_exists(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_lockall(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_lockall_nonblock(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_unlockall(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_lockall_read(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_lockall_read_nonblock(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_unlockall_read(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_lockall_mark(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_lockall_unmark(સંરચના tdb_context *tdb);
અચલ અક્ષર *tdb_name(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_fd(સંરચના tdb_context *tdb);
tdb_log_func tdb_log_fn(સંરચના tdb_context *tdb);
શૂન્ય *tdb_get_logging_private(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_transaction_start(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_transaction_commit(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_transaction_cancel(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_transaction_recover(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_get_seqnum(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_hash_size(સંરચના tdb_context *tdb);
size_t tdb_map_size(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_get_flags(સંરચના tdb_context *tdb);
શૂન્ય tdb_enable_seqnum(સંરચના tdb_context *tdb);
શૂન્ય tdb_increment_seqnum_nonblock(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_flush(સંરચના tdb_context *tdb);

/* Low level locking functions: use with care */
પૂર્ણાંક tdb_chainlock(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_chainlock_nonblock(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_chainunlock(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_chainlock_read(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_chainunlock_read(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_chainlock_mark(સંરચના tdb_context *tdb, TDB_DATA ચાવી);
પૂર્ણાંક tdb_chainlock_unmark(સંરચના tdb_context *tdb, TDB_DATA ચાવી);

/* Debug functions. Not used in production. */
શૂન્ય tdb_dump_all(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_printfreelist(સંરચના tdb_context *tdb);
પૂર્ણાંક tdb_validate_freelist(સંરચના tdb_context *tdb, પૂર્ણાંક *pnum_entries);

#ifdef  __cplusplus
}
#endif

#endif /* tdb.h */
