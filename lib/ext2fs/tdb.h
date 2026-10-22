#यदि_अपरिभाषितम् __TDB_H__
#परिभाषय __TDB_H__

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

#यदि_परिभाषितम्  __cplusplus
बाह्य "C" {
#यदि_अन्तः


/* flags to tdb_store() */
#परिभाषय TDB_REPLACE 1
#परिभाषय TDB_निवेशय 2
#परिभाषय TDB_MODIFY 3

/* flags for tdb_open() */
#परिभाषय TDB_DEFAULT 0 /* just a readability place holder */
#परिभाषय TDB_CLEAR_IF_FIRST 1
#परिभाषय TDB_आन्तरिकम् 2 /* don't store on disk */
#परिभाषय TDB_NOLOCK   4 /* don't do any locking */
#परिभाषय TDB_NOMMAP   8 /* don't use mmap */
#परिभाषय TDB_CONVERT 16 /* convert endian (internal use) */
#परिभाषय TDB_BIGENDIAN 32 /* header is big-endian (internal use) */
#परिभाषय TDB_NOSYNC   64 /* don't use synchronous transactions */
#परिभाषय TDB_SEQNUM   128 /* maintain a sequence number */

#परिभाषय TDB_ERRCODE(संहिता, ret) ((tdb->ecode = (संहिता)), ret)

/* error codes */
गणना TDB_ERROR {TDB_SUCCESS=0, TDB_ERR_CORRUPT, TDB_ERR_IO, TDB_ERR_तालकम्,
		TDB_ERR_OOM, TDB_ERR_EXISTS, TDB_ERR_NOLOCK, TDB_ERR_तालकम्_TIMEOUT,
		TDB_ERR_NOEXIST, TDB_ERR_EINVAL, TDB_ERR_RDONLY};

/* debugging uses one of the following levels */
गणना tdb_debug_level {TDB_DEBUG_FATAL = 0, TDB_DEBUG_ERROR,
		      TDB_DEBUG_WARNING, TDB_DEBUG_TRACE};

प्रकारनिर्देश संरचना TDB_दत्त {
	अचिह्नित वर्ण *dptr;
	size_t dsize;
} TDB_दत्त;

#यदि_अपरिभाषितम् PRINTF_ATTRIBUTE
#यदि (__GNUC__ >= 3)
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#परिभाषय PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#अन्यथा
#परिभाषय PRINTF_ATTRIBUTE(a1, a2)
#यदि_अन्तः
#यदि_अन्तः

/* ext2fs tdb renames */
#परिभाषय tdb_open ext2fs_tdb_open
#परिभाषय tdb_open_ex ext2fs_tdb_open_ex
#परिभाषय tdb_समुच्चयः_max_dead ext2fs_tdb_समुच्चयः_max_dead
#परिभाषय tdb_reopen ext2fs_tdb_reopen
#परिभाषय tdb_reopen_all ext2fs_tdb_reopen_all
#परिभाषय tdb_समुच्चयः_logging_function ext2fs_tdb_समुच्चयः_logging_function
#परिभाषय tdb_error ext2fs_tdb_error
#परिभाषय tdb_errorstr ext2fs_tdb_errorstr
#परिभाषय tdb_fetch ext2fs_tdb_fetch
#परिभाषय tdb_parse_लेखः ext2fs_tdb_parse_लेखः
#परिभाषय tdb_delete ext2fs_tdb_delete
#परिभाषय tdb_store ext2fs_tdb_store
#परिभाषय tdb_append ext2fs_tdb_append
#परिभाषय tdb_close ext2fs_tdb_close
#परिभाषय tdb_firstkey ext2fs_tdb_firstkey
#परिभाषय tdb_nextkey ext2fs_tdb_nextkey
#परिभाषय tdb_traverse ext2fs_tdb_traverse
#परिभाषय tdb_traverse_read ext2fs_tdb_traverse_read
#परिभाषय tdb_exists ext2fs_tdb_exists
#परिभाषय tdb_lockall ext2fs_tdb_lockall
#परिभाषय tdb_unlockall ext2fs_tdb_unlockall
#परिभाषय tdb_lockall_read ext2fs_tdb_lockall_read
#परिभाषय tdb_unlockall_read ext2fs_tdb_unlockall_read
#परिभाषय tdb_name ext2fs_tdb_name
#परिभाषय tdb_fd ext2fs_tdb_fd
#परिभाषय tdb_log_fn ext2fs_tdb_log_fn
#परिभाषय tdb_get_logging_वैयक्तिकम् ext2fs_tdb_get_logging_वैयक्तिकम्
#परिभाषय tdb_transaction_start ext2fs_tdb_transaction_start
#परिभाषय tdb_transaction_commit ext2fs_tdb_transaction_commit
#परिभाषय tdb_transaction_cancel ext2fs_tdb_transaction_cancel
#परिभाषय tdb_transaction_recover ext2fs_tdb_transaction_recover
#परिभाषय tdb_get_seqnum ext2fs_tdb_get_seqnum
#परिभाषय tdb_hash_आकार ext2fs_tdb_hash_आकार
#परिभाषय tdb_map_आकार ext2fs_tdb_map_आकार
#परिभाषय tdb_get_flags ext2fs_tdb_get_flags
#परिभाषय tdb_chainlock ext2fs_tdb_chainlock
#परिभाषय tdb_chainunlock ext2fs_tdb_chainunlock
#परिभाषय tdb_chainlock_read ext2fs_tdb_chainlock_read
#परिभाषय tdb_chainunlock_read ext2fs_tdb_chainunlock_read
#परिभाषय tdb_dump_all ext2fs_tdb_dump_all
#परिभाषय tdb_printfreelist ext2fs_tdb_printfreelist
#परिभाषय tdb_validate_freelist ext2fs_tdb_validate_freelist
#परिभाषय tdb_chainlock_mark ext2fs_tdb_chainlock_mark
#परिभाषय tdb_chainlock_nonblock ext2fs_tdb_chainlock_nonblock
#परिभाषय tdb_chainlock_unmark ext2fs_tdb_chainlock_unmark
#परिभाषय tdb_enable_seqnum ext2fs_tdb_enable_seqnum
#परिभाषय tdb_increment_seqnum_nonblock ext2fs_tdb_increment_seqnum_nonblock
#परिभाषय tdb_तालकम्_nonblock ext2fs_tdb_तालकम्_nonblock
#परिभाषय tdb_lockall_mark ext2fs_tdb_lockall_mark
#परिभाषय tdb_lockall_nonblock ext2fs_tdb_lockall_nonblock
#परिभाषय tdb_lockall_read_nonblock ext2fs_tdb_lockall_read_nonblock
#परिभाषय tdb_lockall_unmark ext2fs_tdb_lockall_unmark
#परिभाषय tdb_flush ext2fs_tdb_flush

/* this is the context structure that is returned from a db open */
प्रकारनिर्देश संरचना tdb_context TDB_CONTEXT;

प्रकारनिर्देश पूर्णाङ्क (*tdb_traverse_func)(संरचना tdb_context *, TDB_दत्त, TDB_दत्त, शून्य *);
प्रकारनिर्देश शून्य (*tdb_log_func)(संरचना tdb_context *, गणना tdb_debug_level, ध्रुव वर्ण *, ...) PRINTF_ATTRIBUTE(3, 4);
प्रकारनिर्देश अचिह्नित पूर्णाङ्क (*tdb_hash_func)(TDB_दत्त *कुञ्चिका);

संरचना tdb_logging_context {
        tdb_log_func log_fn;
        शून्य *log_वैयक्तिकम्;
};

संरचना tdb_context *tdb_open(ध्रुव वर्ण *name, पूर्णाङ्क hash_आकार, पूर्णाङ्क tdb_flags,
		      पूर्णाङ्क open_flags, mode_t बहुलकम्);
संरचना tdb_context *tdb_open_ex(ध्रुव वर्ण *name, पूर्णाङ्क hash_आकार, पूर्णाङ्क tdb_flags,
			 पूर्णाङ्क open_flags, mode_t बहुलकम्,
			 ध्रुव संरचना tdb_logging_context *log_ctx,
			 tdb_hash_func hash_fn);
शून्य tdb_समुच्चयः_max_dead(संरचना tdb_context *tdb, पूर्णाङ्क max_dead);

पूर्णाङ्क tdb_reopen(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_reopen_all(पूर्णाङ्क parent_longlived);
शून्य tdb_समुच्चयः_logging_function(संरचना tdb_context *tdb, ध्रुव संरचना tdb_logging_context *log_ctx);
गणना TDB_ERROR tdb_error(संरचना tdb_context *tdb);
ध्रुव वर्ण *tdb_errorstr(संरचना tdb_context *tdb);
TDB_दत्त tdb_fetch(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_parse_लेखः(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका,
		     पूर्णाङ्क (*parser)(TDB_दत्त कुञ्चिका, TDB_दत्त दत्त,
				   शून्य *वैयक्तिकम्_दत्त),
		     शून्य *वैयक्तिकम्_दत्त);
पूर्णाङ्क tdb_delete(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_store(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, पूर्णाङ्क flag);
पूर्णाङ्क tdb_append(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका, TDB_दत्त new_dbuf);
पूर्णाङ्क tdb_close(संरचना tdb_context *tdb);
TDB_दत्त tdb_firstkey(संरचना tdb_context *tdb);
TDB_दत्त tdb_nextkey(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_traverse(संरचना tdb_context *tdb, tdb_traverse_func fn, शून्य *);
पूर्णाङ्क tdb_traverse_read(संरचना tdb_context *tdb, tdb_traverse_func fn, शून्य *);
पूर्णाङ्क tdb_exists(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_lockall(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_lockall_nonblock(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_unlockall(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_lockall_read(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_lockall_read_nonblock(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_unlockall_read(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_lockall_mark(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_lockall_unmark(संरचना tdb_context *tdb);
ध्रुव वर्ण *tdb_name(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_fd(संरचना tdb_context *tdb);
tdb_log_func tdb_log_fn(संरचना tdb_context *tdb);
शून्य *tdb_get_logging_वैयक्तिकम्(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_transaction_start(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_transaction_commit(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_transaction_cancel(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_transaction_recover(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_get_seqnum(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_hash_आकार(संरचना tdb_context *tdb);
size_t tdb_map_आकार(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_get_flags(संरचना tdb_context *tdb);
शून्य tdb_enable_seqnum(संरचना tdb_context *tdb);
शून्य tdb_increment_seqnum_nonblock(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_flush(संरचना tdb_context *tdb);

/* Low level locking functions: use with care */
पूर्णाङ्क tdb_chainlock(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_chainlock_nonblock(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_chainunlock(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_chainlock_read(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_chainunlock_read(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_chainlock_mark(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);
पूर्णाङ्क tdb_chainlock_unmark(संरचना tdb_context *tdb, TDB_दत्त कुञ्चिका);

/* Debug functions. Not used in production. */
शून्य tdb_dump_all(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_printfreelist(संरचना tdb_context *tdb);
पूर्णाङ्क tdb_validate_freelist(संरचना tdb_context *tdb, पूर्णाङ्क *pnum_entries);

#यदि_परिभाषितम्  __cplusplus
}
#यदि_अन्तः

#यदि_अन्तः /* tdb.h */
