#अगर_अघोषित __TDB_H__
#घोषणा __TDB_H__

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

#अगर_घोषित  __cplusplus
बाह्य "C" {
#पूर्ण_अगर


/* flags to tdb_store() */
#घोषणा TDB_REPLACE 1
#घोषणा TDB_जोड़ो 2
#घोषणा TDB_MODIFY 3

/* flags for tdb_open() */
#घोषणा TDB_DEFAULT 0 /* just a readability place holder */
#घोषणा TDB_CLEAR_IF_FIRST 1
#घोषणा TDB_INTERNAL 2 /* don't store on disk */
#घोषणा TDB_NOLOCK   4 /* don't do any locking */
#घोषणा TDB_NOMMAP   8 /* don't use mmap */
#घोषणा TDB_CONVERT 16 /* convert endian (internal use) */
#घोषणा TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#घोषणा TDB_NOSYNC   64 /* don't use synchronous transactions */
#घोषणा TDB_SEQNUM   128 /* maintain a sequence number */

#घोषणा TDB_ERRCODE(code, ret) ((tdb->ecode = (code)), ret)

/* error codes */
क्रमागत TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_LOCK,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_LOCK_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
क्रमागत tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

प्रकार काष्ठा TDB_तथ्य {
	अचिन्हित अक्षर *dptr;
	माप_प्रकार dsize;
} TDB_तथ्य;

#अगर_अघोषित PRINTF_ATTRIBUTE
#अगर (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#घोषणा PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#अन्यथा
#घोषणा PRINTF_ATTRIBUTE(a1, a2)
#पूर्ण_अगर
#पूर्ण_अगर

/* ext2fs tdb renames */
#घोषणा tdb_open ext2fs_tdb_open
#घोषणा tdb_open_ex ext2fs_tdb_open_ex
#घोषणा tdb_set_max_dead ext2fs_tdb_set_max_dead
#घोषणा tdb_reopen ext2fs_tdb_reopen
#घोषणा tdb_reopen_all ext2fs_tdb_reopen_all
#घोषणा tdb_set_logging_function ext2fs_tdb_set_logging_function
#घोषणा tdb_error ext2fs_tdb_error
#घोषणा tdb_errorstr ext2fs_tdb_errorstr
#घोषणा tdb_fetch ext2fs_tdb_fetch
#घोषणा tdb_parse_record ext2fs_tdb_parse_record
#घोषणा tdb_delete ext2fs_tdb_delete
#घोषणा tdb_store ext2fs_tdb_store
#घोषणा tdb_append ext2fs_tdb_append
#घोषणा tdb_close ext2fs_tdb_close
#घोषणा tdb_firstkey ext2fs_tdb_firstkey
#घोषणा tdb_nextkey ext2fs_tdb_nextkey
#घोषणा tdb_traverse ext2fs_tdb_traverse
#घोषणा tdb_traverse_read ext2fs_tdb_traverse_read
#घोषणा tdb_exists ext2fs_tdb_exists
#घोषणा tdb_lockall ext2fs_tdb_lockall
#घोषणा tdb_unlockall ext2fs_tdb_unlockall
#घोषणा tdb_lockall_read ext2fs_tdb_lockall_read
#घोषणा tdb_unlockall_read ext2fs_tdb_unlockall_read
#घोषणा tdb_name ext2fs_tdb_name
#घोषणा tdb_fd ext2fs_tdb_fd
#घोषणा tdb_log_fn ext2fs_tdb_log_fn
#घोषणा tdb_get_logging_private ext2fs_tdb_get_logging_private
#घोषणा tdb_transaction_start ext2fs_tdb_transaction_start
#घोषणा tdb_transaction_commit ext2fs_tdb_transaction_commit
#घोषणा tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#घोषणा tdb_transaction_recover ext2fs_tdb_transaction_recover
#घोषणा tdb_get_seqnum ext2fs_tdb_get_seqnum
#घोषणा tdb_hash_आकार ext2fs_tdb_hash_आकार
#घोषणा tdb_map_आकार ext2fs_tdb_map_आकार
#घोषणा tdb_get_flags ext2fs_tdb_get_flags
#घोषणा tdb_chainlock ext2fs_tdb_chainlock
#घोषणा tdb_chainunlock ext2fs_tdb_chainunlock
#घोषणा tdb_chainlock_read ext2fs_tdb_chainlock_read
#घोषणा tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#घोषणा tdb_dump_all ext2fs_tdb_dump_all
#घोषणा tdb_printfreelist ext2fs_tdb_printfreelist
#घोषणा tdb_validate_freelist ext2fs_tdb_validate_freelist
#घोषणा tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#घोषणा tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#घोषणा tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#घोषणा tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#घोषणा tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#घोषणा tdb_lock_nonblock ext2fs_tdb_lock_nonblock
#घोषणा tdb_lockall_mark ext2fs_tdb_lockall_mark
#घोषणा tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#घोषणा tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#घोषणा tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#घोषणा tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
प्रकार काष्ठा tdb_context TDB_CONTEXT;

प्रकार पूर्णांक (*tdb_traverse_func)(काष्ठा tdb_context *, TDB_तथ्य, TDB_तथ्य, व्योम *);
प्रकार व्योम (*tdb_log_func)(काष्ठा tdb_context *, क्रमागत tdb_debug_level, स्थिर अक्षर *, ...) PRINTF_ATTRIBUTE(3, 4);
प्रकार अचिन्हित पूर्णांक (*tdb_hash_func)(TDB_तथ्य *कुंजी);

काष्ठा tdb_logging_context {
        tdb_log_func log_fn;
        व्योम *log_private;
};

काष्ठा tdb_context *tdb_open(स्थिर अक्षर *name, पूर्णांक hash_आकार, पूर्णांक tdb_flags,
		      पूर्णांक open_flags, mode_t mode);
काष्ठा tdb_context *tdb_open_ex(स्थिर अक्षर *name, पूर्णांक hash_आकार, पूर्णांक tdb_flags,
			 पूर्णांक open_flags, mode_t mode,
			 स्थिर काष्ठा tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
व्योम tdb_set_max_dead(काष्ठा tdb_context *tdb, पूर्णांक max_dead);

पूर्णांक tdb_reopen(काष्ठा tdb_context *tdb);
पूर्णांक tdb_reopen_all(पूर्णांक parent_longlived);
व्योम tdb_set_logging_function(काष्ठा tdb_context *tdb, स्थिर काष्ठा tdb_logging_context *log_ctx);
क्रमागत TDB_ERROR tdb_error(काष्ठा tdb_context *tdb);
स्थिर अक्षर *tdb_errorstr(काष्ठा tdb_context *tdb);
TDB_तथ्य tdb_fetch(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_parse_record(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी,
		     पूर्णांक (*parser)(TDB_तथ्य कुंजी, TDB_तथ्य तथ्य,
				   व्योम *private_तथ्य),
		     व्योम *private_तथ्य);
पूर्णांक tdb_delete(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_store(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, पूर्णांक flag);
पूर्णांक tdb_append(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी, TDB_तथ्य new_dbuf);
पूर्णांक tdb_close(काष्ठा tdb_context *tdb);
TDB_तथ्य tdb_firstkey(काष्ठा tdb_context *tdb);
TDB_तथ्य tdb_nextkey(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_traverse(काष्ठा tdb_context *tdb, tdb_traverse_func fn, व्योम *);
पूर्णांक tdb_traverse_read(काष्ठा tdb_context *tdb, tdb_traverse_func fn, व्योम *);
पूर्णांक tdb_exists(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_lockall(काष्ठा tdb_context *tdb);
पूर्णांक tdb_lockall_nonblock(काष्ठा tdb_context *tdb);
पूर्णांक tdb_unlockall(काष्ठा tdb_context *tdb);
पूर्णांक tdb_lockall_read(काष्ठा tdb_context *tdb);
पूर्णांक tdb_lockall_read_nonblock(काष्ठा tdb_context *tdb);
पूर्णांक tdb_unlockall_read(काष्ठा tdb_context *tdb);
पूर्णांक tdb_lockall_mark(काष्ठा tdb_context *tdb);
पूर्णांक tdb_lockall_unmark(काष्ठा tdb_context *tdb);
स्थिर अक्षर *tdb_name(काष्ठा tdb_context *tdb);
पूर्णांक tdb_fd(काष्ठा tdb_context *tdb);
tdb_log_func tdb_log_fn(काष्ठा tdb_context *tdb);
व्योम *tdb_get_logging_private(काष्ठा tdb_context *tdb);
पूर्णांक tdb_transaction_start(काष्ठा tdb_context *tdb);
पूर्णांक tdb_transaction_commit(काष्ठा tdb_context *tdb);
पूर्णांक tdb_transaction_cancel(काष्ठा tdb_context *tdb);
पूर्णांक tdb_transaction_recover(काष्ठा tdb_context *tdb);
पूर्णांक tdb_get_seqnum(काष्ठा tdb_context *tdb);
पूर्णांक tdb_hash_आकार(काष्ठा tdb_context *tdb);
माप_प्रकार tdb_map_आकार(काष्ठा tdb_context *tdb);
पूर्णांक tdb_get_flags(काष्ठा tdb_context *tdb);
व्योम tdb_enable_seqnum(काष्ठा tdb_context *tdb);
व्योम tdb_increment_seqnum_nonblock(काष्ठा tdb_context *tdb);
पूर्णांक tdb_flush(काष्ठा tdb_context *tdb);

/* Low level locking functions: use with care */
पूर्णांक tdb_chainlock(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_chainlock_nonblock(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_chainunlock(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_chainlock_read(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_chainunlock_read(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_chainlock_mark(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);
पूर्णांक tdb_chainlock_unmark(काष्ठा tdb_context *tdb, TDB_तथ्य कुंजी);

/* Debug functions. Not used in production. */
व्योम tdb_dump_all(काष्ठा tdb_context *tdb);
पूर्णांक tdb_printfreelist(काष्ठा tdb_context *tdb);
पूर्णांक tdb_validate_freelist(काष्ठा tdb_context *tdb, पूर्णांक *pnum_entries);

#अगर_घोषित  __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर /* tdb.h */
