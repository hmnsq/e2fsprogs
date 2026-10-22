/*
   Unix SMB/CIFS implementation.
   Samba database functions
   Copyright (C) Andrew Tridgell              1999-2000
   Copyright (C) Paul `Rusty' Russell		   2000
   Copyright (C) Jeremy Allison			   2000
   Copyright (C) Andrew Esh                        2001

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "tdb.h"

ਸਥਿਰ ਪੂਰਨ do_command(ਖਾਲੀ);
ਅਚਲ ਅੱਖਰ *cmdname;
ਅੱਖਰ *arg1, *arg2;
size_t arg1len, arg2len;
ਪੂਰਨ bIterate = 0;
ਅੱਖਰ *ਸਤਰ;
TDB_ਅੰਕੜੇ iterate_kbuf;
ਅੱਖਰ cmdline[1024];

ਗਿਣਤੀ commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_ਪਾਓ,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_ਕੁੰਜੀਆਂ,
	CMD_HEXKEYS,
	CMD_DELETE,
	CMD_LIST_HASH_FREE,
	CMD_LIST_FREE,
	CMD_INFO,
	CMD_ਪਹਿਲਾ,
	CMD_ਅਗਲਾ,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
	ਅਚਲ ਅੱਖਰ *name;
	ਗਿਣਤੀ commands cmd;
} COMMAND_ਸਾਰਣੀ;

COMMAND_ਸਾਰਣੀ cmd_ਸਾਰਣੀ[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_ਪਾਓ},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_ਕੁੰਜੀਆਂ},
	{"hexkeys",	CMD_HEXKEYS},
	{"delete",	CMD_DELETE},
	{"list",	CMD_LIST_HASH_FREE},
	{"free",	CMD_LIST_FREE},
	{"info",	CMD_INFO},
	{"first",	CMD_ਪਹਿਲਾ},
	{"1",		CMD_ਪਹਿਲਾ},
	{"next",	CMD_ਅਗਲਾ},
	{"n",		CMD_ਅਗਲਾ},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{ਨਿਰਾ,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ਸਥਿਰ TDB_CONTEXT *tdb;

ਸਥਿਰ ਪੂਰਨ print_rec(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state);
ਸਥਿਰ ਪੂਰਨ print_ਕੁੰਜੀ(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state);
ਸਥਿਰ ਪੂਰਨ print_hexkey(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state);

ਸਥਿਰ ਖਾਲੀ print_asc(ਅਚਲ ਅੱਖਰ *buf,ਪੂਰਨ len)
{
	ਪੂਰਨ i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	ਜੇ (buf[len - 1] == 0)
	        len--;

	ਲਈ (i=0;i<len;i++)
		ਲਿਖੋ("%c",isprint(buf[i])?buf[i]:'.');
}

ਸਥਿਰ ਖਾਲੀ print_ਅੰਕੜੇ(ਅਚਲ ਅੱਖਰ *buf,ਪੂਰਨ len)
{
	ਪੂਰਨ i=0;
	ਜੇ (len<=0) ਵਾਪਸ;
	ਲਿਖੋ("[%03X] ",i);
	ਲਈ (i=0;i<len;) {
		ਲਿਖੋ("%02X ",(ਪੂਰਨ)buf[i]);
		i++;
		ਜੇ (i%8 == 0) ਲਿਖੋ(" ");
		ਜੇ (i%16 == 0) {
			print_asc(&buf[i-16],8); ਲਿਖੋ(" ");
			print_asc(&buf[i-8],8); ਲਿਖੋ("\n");
			ਜੇ (i<len) ਲਿਖੋ("[%03X] ",i);
		}
	}
	ਜੇ (i%16) {
		ਪੂਰਨ n;

		n = 16 - (i%16);
		ਲਿਖੋ(" ");
		ਜੇ (n>8) ਲਿਖੋ(" ");
		ਜਦੋਂ (n--) ਲਿਖੋ("   ");

		n = i%16;
		ਜੇ (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); ਲਿਖੋ(" ");
		n = (i%16) - n;
		ਜੇ (n>0) print_asc(&buf[i-n],n);
		ਲਿਖੋ("\n");
	}
}

ਸਥਿਰ ਖਾਲੀ help(ਖਾਲੀ)
{
	ਲਿਖੋ("\n"
"tdbtool: \n"
"  create    dbname     : create a database\n"
"  open      dbname     : open an existing database\n"
"  erase                : erase the database\n"
"  dump                 : dump the database as strings\n"
"  keys                 : dump the database keys as strings\n"
"  hexkeys              : dump the database keys as hex values\n"
"  info                 : print summary info about the database\n"
"  insert    key  data  : insert a record\n"
"  move      key  file  : move a record to a destination tdb\n"
"  store     key  data  : store a record (replace)\n"
"  show      key        : show a record by key\n"
"  delete    key        : delete a record by key\n"
"  list                 : print the database hash table and freelist\n"
"  free                 : print the database freelist\n"
"  ! command            : execute system command\n"
"  1 | first            : print the first record\n"
"  n | next             : print the next record\n"
"  q | quit             : terminate\n"
"  \\n                   : repeat 'next' command\n"
"\n");
}

ਸਥਿਰ ਖਾਲੀ terror(ਅਚਲ ਅੱਖਰ *why)
{
	ਲਿਖੋ("%s\n", why);
}

ਸਥਿਰ ਖਾਲੀ create_tdb(ਅਚਲ ਅੱਖਰ *tdbname)
{
	ਜੇ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_ਪਹਿਲਾ,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	ਜੇ (!tdb) {
		ਲਿਖੋ("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ਸਥਿਰ ਖਾਲੀ open_tdb(ਅਚਲ ਅੱਖਰ *tdbname)
{
	ਜੇ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ਜੇ (!tdb) {
		ਲਿਖੋ("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_tdb(ਅੱਖਰ *keyname, size_t keylen, ਅੱਖਰ* ਅੰਕੜੇ, size_t datalen)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, dbuf;

	ਜੇ ((keyname == ਨਿਰਾ) || (keylen == 0)) {
		terror("need key");
		ਵਾਪਸ;
	}

	ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)keyname;
	ਕੁੰਜੀ.dsize = keylen;
	dbuf.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਅੰਕੜੇ;
	dbuf.dsize = datalen;

	ਜੇ (tdb_store(tdb, ਕੁੰਜੀ, dbuf, TDB_ਪਾਓ) == -1) {
		terror("insert failed");
	}
}

ਸਥਿਰ ਖਾਲੀ store_tdb(ਅੱਖਰ *keyname, size_t keylen, ਅੱਖਰ* ਅੰਕੜੇ, size_t datalen)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, dbuf;

	ਜੇ ((keyname == ਨਿਰਾ) || (keylen == 0)) {
		terror("need key");
		ਵਾਪਸ;
	}

	ਜੇ ((ਅੰਕੜੇ == ਨਿਰਾ) || (datalen == 0)) {
		terror("need data");
		ਵਾਪਸ;
	}

	ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)keyname;
	ਕੁੰਜੀ.dsize = keylen;
	dbuf.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਅੰਕੜੇ;
	dbuf.dsize = datalen;

	ਲਿਖੋ("Storing key:\n");
	print_rec(tdb, ਕੁੰਜੀ, dbuf, ਨਿਰਾ);

	ਜੇ (tdb_store(tdb, ਕੁੰਜੀ, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ਸਥਿਰ ਖਾਲੀ show_tdb(ਅੱਖਰ *keyname, size_t keylen)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, dbuf;

	ਜੇ ((keyname == ਨਿਰਾ) || (keylen == 0)) {
		terror("need key");
		ਵਾਪਸ;
	}

	ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)keyname;
	ਕੁੰਜੀ.dsize = keylen;

	dbuf = tdb_fetch(tdb, ਕੁੰਜੀ);
	ਜੇ (!dbuf.dptr) {
	    terror("fetch failed");
	    ਵਾਪਸ;
	}

	print_rec(tdb, ਕੁੰਜੀ, dbuf, ਨਿਰਾ);

	ਮੁਕਤ( dbuf.dptr );

	ਵਾਪਸ;
}

ਸਥਿਰ ਖਾਲੀ delete_tdb(ਅੱਖਰ *keyname, size_t keylen)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ;

	ਜੇ ((keyname == ਨਿਰਾ) || (keylen == 0)) {
		terror("need key");
		ਵਾਪਸ;
	}

	ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)keyname;
	ਕੁੰਜੀ.dsize = keylen;

	ਜੇ (tdb_delete(tdb, ਕੁੰਜੀ) != 0) {
		terror("delete failed");
	}
}

ਸਥਿਰ ਖਾਲੀ move_rec(ਅੱਖਰ *keyname, size_t keylen, ਅੱਖਰ* tdbname)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, dbuf;
	TDB_CONTEXT *dst_tdb;

	ਜੇ ((keyname == ਨਿਰਾ) || (keylen == 0)) {
		terror("need key");
		ਵਾਪਸ;
	}

	ਜੇ ( !tdbname ) {
		terror("need destination tdb name");
		ਵਾਪਸ;
	}

	ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)keyname;
	ਕੁੰਜੀ.dsize = keylen;

	dbuf = tdb_fetch(tdb, ਕੁੰਜੀ);
	ਜੇ (!dbuf.dptr) {
		terror("fetch failed");
		ਵਾਪਸ;
	}

	print_rec(tdb, ਕੁੰਜੀ, dbuf, ਨਿਰਾ);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ਜੇ ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ਵਾਪਸ;
	}

	ਜੇ ( tdb_store( dst_tdb, ਕੁੰਜੀ, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("record moved\n");

	tdb_close( dst_tdb );

	ਵਾਪਸ;
}

ਸਥਿਰ ਪੂਰਨ print_rec(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state)
{
	ਲਿਖੋ("\nkey %d bytes\n", (ਪੂਰਨ)ਕੁੰਜੀ.dsize);
	print_asc((ਅਚਲ ਅੱਖਰ *)ਕੁੰਜੀ.dptr, ਕੁੰਜੀ.dsize);
	ਲਿਖੋ("\ndata %d bytes\n", (ਪੂਰਨ)dbuf.dsize);
	print_ਅੰਕੜੇ((ਅਚਲ ਅੱਖਰ *)dbuf.dptr, dbuf.dsize);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ print_ਕੁੰਜੀ(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state)
{
	ਲਿਖੋ("key %d bytes: ", (ਪੂਰਨ)ਕੁੰਜੀ.dsize);
	print_asc((ਅਚਲ ਅੱਖਰ *)ਕੁੰਜੀ.dptr, ਕੁੰਜੀ.dsize);
	ਲਿਖੋ("\n");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ print_hexkey(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state)
{
	ਲਿਖੋ("key %d bytes\n", (ਪੂਰਨ)ਕੁੰਜੀ.dsize);
	print_ਅੰਕੜੇ((ਅਚਲ ਅੱਖਰ *)ਕੁੰਜੀ.dptr, ਕੁੰਜੀ.dsize);
	ਲਿਖੋ("\n");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਕੁੱਲ_ਬਾਈਟਾਂ;

ਸਥਿਰ ਪੂਰਨ traverse_fn(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਖਾਲੀ *state)
{
	ਕੁੱਲ_ਬਾਈਟਾਂ += dbuf.dsize;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ info_tdb(ਖਾਲੀ)
{
	ਪੂਰਨ ਗਿਣਤੀ_2;
	ਕੁੱਲ_ਬਾਈਟਾਂ = 0;
	ਜੇ ((ਗਿਣਤੀ_2 = tdb_traverse(tdb, traverse_fn, ਨਿਰਾ)) == -1)
		ਲਿਖੋ("Error = %s\n", tdb_errorstr(tdb));
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%d records totalling %d bytes\n", ਗਿਣਤੀ_2, ਕੁੱਲ_ਬਾਈਟਾਂ);
}

ਸਥਿਰ ਅੱਖਰ *tdb_getline(ਅਚਲ ਅੱਖਰ *prompt)
{
	ਸਥਿਰ ਅੱਖਰ thisline[1024];
	ਅੱਖਰ *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, ਆਕਾਰ(thisline)-1, stdin);
	ਜੇ (p) p = strchr(p, '\n');
	ਜੇ (p) *p = 0;
	ਵਾਪਸ p?thisline:ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ do_delete_fn(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf,
                     ਖਾਲੀ *state)
{
    ਵਾਪਸ tdb_delete(the_tdb, ਕੁੰਜੀ);
}

ਸਥਿਰ ਖਾਲੀ ਪਹਿਲਾ_ਰਿਕਾਰਡ(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ *pkey)
{
	TDB_ਅੰਕੜੇ dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ਜੇ (!dbuf.dptr) terror("fetch failed");
	ਨਹੀਂਤਾਂ {
		print_rec(the_tdb, *pkey, dbuf, ਨਿਰਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ ਅਗਲਾ_ਰਿਕਾਰਡ(TDB_CONTEXT *the_tdb, TDB_ਅੰਕੜੇ *pkey)
{
	TDB_ਅੰਕੜੇ dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ਜੇ (!dbuf.dptr)
		terror("fetch failed");
	ਨਹੀਂਤਾਂ
		print_rec(the_tdb, *pkey, dbuf, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ do_command(ਖਾਲੀ)
{
	COMMAND_ਸਾਰਣੀ *ctp = cmd_ਸਾਰਣੀ;
	ਗਿਣਤੀ commands mycmd = CMD_HELP;
	ਪੂਰਨ cmd_len;

	ਜੇ (cmdname && ਲੜੀ_ਲੰਬਾਈ(cmdname) == 0) {
	    mycmd = CMD_ਅਗਲਾ;
	} ਨਹੀਂਤਾਂ {
	    ਜਦੋਂ (ctp->name) {
		cmd_len = ਲੜੀ_ਲੰਬਾਈ(ctp->name);
		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			ਰੋਕੋ;
		}
		ctp++;
	    }
	}

	ਚੋਣ (mycmd) {
	ਹਾਲਤ CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    ਵਾਪਸ 0;
	ਹਾਲਤ CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ਵਾਪਸ 0;
	ਹਾਲਤ CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ਵਾਪਸ 0;
	ਹਾਲਤ CMD_QUIT:
	    ਵਾਪਸ 1;
	ਮੂਲ:
	    /* all the rest require a open database */
	    ਜੇ (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ਵਾਪਸ 0;
	    }
	    ਚੋਣ (mycmd) {
	    ਹਾਲਤ CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ਨਿਰਾ);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ਨਿਰਾ);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_ਪਾਓ:
		bIterate = 0;
		ਪਾਓ_tdb(arg1, arg1len,arg2,arg2len);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_ਕੁੰਜੀਆਂ:
		tdb_traverse(tdb, print_ਕੁੰਜੀ, ਨਿਰਾ);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ਨਿਰਾ);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_INFO:
		info_tdb();
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_ਪਹਿਲਾ:
		bIterate = 1;
		ਪਹਿਲਾ_ਰਿਕਾਰਡ(tdb, &iterate_kbuf);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_ਅਗਲਾ:
	       ਜੇ (bIterate)
		  ਅਗਲਾ_ਰਿਕਾਰਡ(tdb, &iterate_kbuf);
		ਵਾਪਸ 0;
	    ਹਾਲਤ CMD_HELP:
		help();
		ਵਾਪਸ 0;
            ਹਾਲਤ CMD_CREATE_TDB:
            ਹਾਲਤ CMD_OPEN_TDB:
            ਹਾਲਤ CMD_SYSTEM:
            ਹਾਲਤ CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ਵਾਪਸ 0;
	    }
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ *convert_string(ਅੱਖਰ *instring, size_t *sizep)
{
    size_t ਲੰਮਾਈ = 0;
    ਅੱਖਰ *outp, *inp;
    ਅੱਖਰ ਆਰਜ਼ੀ[3];


    outp = inp = instring;

    ਜਦੋਂ (*inp) {
	ਜੇ (*inp == '\\') {
	    inp++;
	    ਜੇ (*inp && strchr("0123456789abcdefABCDEF",(ਪੂਰਨ)*inp)) {
		ਆਰਜ਼ੀ[0] = *inp++;
		ਆਰਜ਼ੀ[1] = '\0';
		ਜੇ (*inp && strchr("0123456789abcdefABCDEF",(ਪੂਰਨ)*inp)) {
		    ਆਰਜ਼ੀ[1] = *inp++;
		    ਆਰਜ਼ੀ[2] = '\0';
		}
		*outp++ = (ਅੱਖਰ)strtol((ਅਚਲ ਅੱਖਰ *)ਆਰਜ਼ੀ,ਨਿਰਾ,16);
	    } ਨਹੀਂਤਾਂ {
		*outp++ = *inp++;
	    }
	} ਨਹੀਂਤਾਂ {
	    *outp++ = *inp++;
	}
	ਲੰਮਾਈ++;
    }
    *sizep = ਲੰਮਾਈ;
    ਵਾਪਸ instring;
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ *argv[])
{
    cmdname = "";
    arg1 = ਨਿਰਾ;
    arg1len = 0;
    arg2 = ਨਿਰਾ;
    arg2len = 0;

    ਜੇ (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ਨਿਰਾ;
    }

    ਚੋਣ (argc) {
	ਹਾਲਤ 1:
	ਹਾਲਤ 2:
	    /* Interactive mode */
	    ਜਦੋਂ ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ਨਿਰਾ;
		ਜੇ ((arg1 = strchr((ਅਚਲ ਅੱਖਰ *)cmdname,' ')) != ਨਿਰਾ) {
		    arg1++;
		    arg2 = arg1;
		    ਜਦੋਂ (*arg2) {
			ਜੇ (*arg2 == ' ') {
			    *arg2++ = '\0';
			    ਰੋਕੋ;
			}
			ਜੇ ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		ਜੇ (arg1) arg1 = convert_string(arg1,&arg1len);
		ਜੇ (arg2) arg2 = convert_string(arg2,&arg2len);
		ਜੇ (do_command()) ਰੋਕੋ;
	    }
	    ਰੋਕੋ;
	ਹਾਲਤ 5:
	    arg2 = convert_string(argv[4],&arg2len);
	ਹਾਲਤ 4:
	    arg1 = convert_string(argv[3],&arg1len);
	ਹਾਲਤ 3:
	    cmdname = argv[2];
	ਮੂਲ:
	    do_command();
	    ਰੋਕੋ;
    }

    ਜੇ (tdb) tdb_close(tdb);

    ਵਾਪਸ 0;
}
