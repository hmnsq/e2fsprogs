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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <ctype.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಅನ್ತರ್ಭಾವ "tdb.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_command(ಶೂನ್ಯ);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmdname;
ಅಕ್ಷರ *arg1, *arg2;
size_t arg1len, arg2len;
ಪೂರ್ಣಾಂಕ bIterate = 0;
ಅಕ್ಷರ *ಸಾಲು;
TDB_ದತ್ತಾಂಶ iterate_kbuf;
ಅಕ್ಷರ cmdline[1024];

ಎಣಿಕೆ commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_ಸೇರಿಸು,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_ಕೀಲಿಗಳು,
	CMD_HEXKEYS,
	CMD_DELETE,
	CMD_LIST_HASH_FREE,
	CMD_LIST_FREE,
	CMD_INFO,
	CMD_ಮೊದಲ,
	CMD_ಮುಂದಿನ,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಎಣಿಕೆ commands cmd;
} COMMAND_ಕೋಷ್ಟಕ;

COMMAND_ಕೋಷ್ಟಕ cmd_ಕೋಷ್ಟಕ[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_ಸೇರಿಸು},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_ಕೀಲಿಗಳು},
	{"hexkeys",	CMD_HEXKEYS},
	{"delete",	CMD_DELETE},
	{"list",	CMD_LIST_HASH_FREE},
	{"free",	CMD_LIST_FREE},
	{"info",	CMD_INFO},
	{"first",	CMD_ಮೊದಲ},
	{"1",		CMD_ಮೊದಲ},
	{"next",	CMD_ಮುಂದಿನ},
	{"n",		CMD_ಮುಂದಿನ},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{ಖಾಲಿ,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ಸ್ಥಿರ TDB_CONTEXT *tdb;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_rec(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_ಕೀಲಿ(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_hexkey(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state);

ಸ್ಥಿರ ಶೂನ್ಯ print_asc(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf,ಪೂರ್ಣಾಂಕ len)
{
	ಪೂರ್ಣಾಂಕ i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	ಆದರೆ (buf[len - 1] == 0)
	        len--;

	ಗಾಗಿ (i=0;i<len;i++)
		ಬರೆ("%c",isprint(buf[i])?buf[i]:'.');
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf,ಪೂರ್ಣಾಂಕ len)
{
	ಪೂರ್ಣಾಂಕ i=0;
	ಆದರೆ (len<=0) ಹಿಂತಿರುಗಿ;
	ಬರೆ("[%03X] ",i);
	ಗಾಗಿ (i=0;i<len;) {
		ಬರೆ("%02X ",(ಪೂರ್ಣಾಂಕ)buf[i]);
		i++;
		ಆದರೆ (i%8 == 0) ಬರೆ(" ");
		ಆದರೆ (i%16 == 0) {
			print_asc(&buf[i-16],8); ಬರೆ(" ");
			print_asc(&buf[i-8],8); ಬರೆ("\n");
			ಆದರೆ (i<len) ಬರೆ("[%03X] ",i);
		}
	}
	ಆದರೆ (i%16) {
		ಪೂರ್ಣಾಂಕ n;

		n = 16 - (i%16);
		ಬರೆ(" ");
		ಆದರೆ (n>8) ಬರೆ(" ");
		ಆಗ (n--) ಬರೆ("   ");

		n = i%16;
		ಆದರೆ (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); ಬರೆ(" ");
		n = (i%16) - n;
		ಆದರೆ (n>0) print_asc(&buf[i-n],n);
		ಬರೆ("\n");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ help(ಶೂನ್ಯ)
{
	ಬರೆ("\n"
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

ಸ್ಥಿರ ಶೂನ್ಯ terror(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *why)
{
	ಬರೆ("%s\n", why);
}

ಸ್ಥಿರ ಶೂನ್ಯ create_tdb(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tdbname)
{
	ಆದರೆ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_ಮೊದಲ,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	ಆದರೆ (!tdb) {
		ಬರೆ("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ open_tdb(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tdbname)
{
	ಆದರೆ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ಆದರೆ (!tdb) {
		ಬರೆ("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_tdb(ಅಕ್ಷರ *keyname, size_t keylen, ಅಕ್ಷರ* ದತ್ತಾಂಶ, size_t datalen)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, dbuf;

	ಆದರೆ ((keyname == ಖಾಲಿ) || (keylen == 0)) {
		terror("need key");
		ಹಿಂತಿರುಗಿ;
	}

	ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)keyname;
	ಕೀಲಿ.dsize = keylen;
	dbuf.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ದತ್ತಾಂಶ;
	dbuf.dsize = datalen;

	ಆದರೆ (tdb_store(tdb, ಕೀಲಿ, dbuf, TDB_ಸೇರಿಸು) == -1) {
		terror("insert failed");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ store_tdb(ಅಕ್ಷರ *keyname, size_t keylen, ಅಕ್ಷರ* ದತ್ತಾಂಶ, size_t datalen)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, dbuf;

	ಆದರೆ ((keyname == ಖಾಲಿ) || (keylen == 0)) {
		terror("need key");
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ ((ದತ್ತಾಂಶ == ಖಾಲಿ) || (datalen == 0)) {
		terror("need data");
		ಹಿಂತಿರುಗಿ;
	}

	ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)keyname;
	ಕೀಲಿ.dsize = keylen;
	dbuf.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ದತ್ತಾಂಶ;
	dbuf.dsize = datalen;

	ಬರೆ("Storing key:\n");
	print_rec(tdb, ಕೀಲಿ, dbuf, ಖಾಲಿ);

	ಆದರೆ (tdb_store(tdb, ಕೀಲಿ, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_tdb(ಅಕ್ಷರ *keyname, size_t keylen)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, dbuf;

	ಆದರೆ ((keyname == ಖಾಲಿ) || (keylen == 0)) {
		terror("need key");
		ಹಿಂತಿರುಗಿ;
	}

	ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)keyname;
	ಕೀಲಿ.dsize = keylen;

	dbuf = tdb_fetch(tdb, ಕೀಲಿ);
	ಆದರೆ (!dbuf.dptr) {
	    terror("fetch failed");
	    ಹಿಂತಿರುಗಿ;
	}

	print_rec(tdb, ಕೀಲಿ, dbuf, ಖಾಲಿ);

	ಬಿಡುಗಡೆ( dbuf.dptr );

	ಹಿಂತಿರುಗಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ delete_tdb(ಅಕ್ಷರ *keyname, size_t keylen)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ;

	ಆದರೆ ((keyname == ಖಾಲಿ) || (keylen == 0)) {
		terror("need key");
		ಹಿಂತಿರುಗಿ;
	}

	ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)keyname;
	ಕೀಲಿ.dsize = keylen;

	ಆದರೆ (tdb_delete(tdb, ಕೀಲಿ) != 0) {
		terror("delete failed");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ move_rec(ಅಕ್ಷರ *keyname, size_t keylen, ಅಕ್ಷರ* tdbname)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, dbuf;
	TDB_CONTEXT *dst_tdb;

	ಆದರೆ ((keyname == ಖಾಲಿ) || (keylen == 0)) {
		terror("need key");
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ ( !tdbname ) {
		terror("need destination tdb name");
		ಹಿಂತಿರುಗಿ;
	}

	ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)keyname;
	ಕೀಲಿ.dsize = keylen;

	dbuf = tdb_fetch(tdb, ಕೀಲಿ);
	ಆದರೆ (!dbuf.dptr) {
		terror("fetch failed");
		ಹಿಂತಿರುಗಿ;
	}

	print_rec(tdb, ಕೀಲಿ, dbuf, ಖಾಲಿ);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ಆದರೆ ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ ( tdb_store( dst_tdb, ಕೀಲಿ, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("record moved\n");

	tdb_close( dst_tdb );

	ಹಿಂತಿರುಗಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_rec(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state)
{
	ಬರೆ("\nkey %d bytes\n", (ಪೂರ್ಣಾಂಕ)ಕೀಲಿ.dsize);
	print_asc((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	ಬರೆ("\ndata %d bytes\n", (ಪೂರ್ಣಾಂಕ)dbuf.dsize);
	print_ದತ್ತಾಂಶ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)dbuf.dptr, dbuf.dsize);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_ಕೀಲಿ(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state)
{
	ಬರೆ("key %d bytes: ", (ಪೂರ್ಣಾಂಕ)ಕೀಲಿ.dsize);
	print_asc((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	ಬರೆ("\n");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_hexkey(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state)
{
	ಬರೆ("key %d bytes\n", (ಪೂರ್ಣಾಂಕ)ಕೀಲಿ.dsize);
	print_ದತ್ತಾಂಶ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	ಬರೆ("\n");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಬೈಟುಗಳು;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ traverse_fn(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಶೂನ್ಯ *state)
{
	ಒಟ್ಟು_ಬೈಟುಗಳು += dbuf.dsize;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ info_tdb(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;
	ಒಟ್ಟು_ಬೈಟುಗಳು = 0;
	ಆದರೆ ((ಎಣಿಕೆ_2 = tdb_traverse(tdb, traverse_fn, ಖಾಲಿ)) == -1)
		ಬರೆ("Error = %s\n", tdb_errorstr(tdb));
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%d records totalling %d bytes\n", ಎಣಿಕೆ_2, ಒಟ್ಟು_ಬೈಟುಗಳು);
}

ಸ್ಥಿರ ಅಕ್ಷರ *tdb_getline(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prompt)
{
	ಸ್ಥಿರ ಅಕ್ಷರ thisline[1024];
	ಅಕ್ಷರ *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, ಗಾತ್ರ(thisline)-1, stdin);
	ಆದರೆ (p) p = strchr(p, '\n');
	ಆದರೆ (p) *p = 0;
	ಹಿಂತಿರುಗಿ p?thisline:ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_delete_fn(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf,
                     ಶೂನ್ಯ *state)
{
    ಹಿಂತಿರುಗಿ tdb_delete(the_tdb, ಕೀಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಮೊದಲ_ದಾಖಲೆ(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ *pkey)
{
	TDB_ದತ್ತಾಂಶ dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ಆದರೆ (!dbuf.dptr) terror("fetch failed");
	ಇಲ್ಲದಿದ್ದರೆ {
		print_rec(the_tdb, *pkey, dbuf, ಖಾಲಿ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಮುಂದಿನ_ದಾಖಲೆ(TDB_CONTEXT *the_tdb, TDB_ದತ್ತಾಂಶ *pkey)
{
	TDB_ದತ್ತಾಂಶ dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ಆದರೆ (!dbuf.dptr)
		terror("fetch failed");
	ಇಲ್ಲದಿದ್ದರೆ
		print_rec(the_tdb, *pkey, dbuf, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_command(ಶೂನ್ಯ)
{
	COMMAND_ಕೋಷ್ಟಕ *ctp = cmd_ಕೋಷ್ಟಕ;
	ಎಣಿಕೆ commands mycmd = CMD_HELP;
	ಪೂರ್ಣಾಂಕ cmd_len;

	ಆದರೆ (cmdname && ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(cmdname) == 0) {
	    mycmd = CMD_ಮುಂದಿನ;
	} ಇಲ್ಲದಿದ್ದರೆ {
	    ಆಗ (ctp->name) {
		cmd_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ctp->name);
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			ನಿಲ್ಲಿಸು;
		}
		ctp++;
	    }
	}

	ಆಯ್ಕೆ (mycmd) {
	ಸ್ಥಿತಿ CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ CMD_QUIT:
	    ಹಿಂತಿರುಗಿ 1;
	ಪೂರ್ವನಿಯೋಜಿತ:
	    /* all the rest require a open database */
	    ಆದರೆ (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ಹಿಂತಿರುಗಿ 0;
	    }
	    ಆಯ್ಕೆ (mycmd) {
	    ಸ್ಥಿತಿ CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_ಸೇರಿಸು:
		bIterate = 0;
		ಸೇರಿಸು_tdb(arg1, arg1len,arg2,arg2len);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_ಕೀಲಿಗಳು:
		tdb_traverse(tdb, print_ಕೀಲಿ, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_INFO:
		info_tdb();
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_ಮೊದಲ:
		bIterate = 1;
		ಮೊದಲ_ದಾಖಲೆ(tdb, &iterate_kbuf);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_ಮುಂದಿನ:
	       ಆದರೆ (bIterate)
		  ಮುಂದಿನ_ದಾಖಲೆ(tdb, &iterate_kbuf);
		ಹಿಂತಿರುಗಿ 0;
	    ಸ್ಥಿತಿ CMD_HELP:
		help();
		ಹಿಂತಿರುಗಿ 0;
            ಸ್ಥಿತಿ CMD_CREATE_TDB:
            ಸ್ಥಿತಿ CMD_OPEN_TDB:
            ಸ್ಥಿತಿ CMD_SYSTEM:
            ಸ್ಥಿತಿ CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ಹಿಂತಿರುಗಿ 0;
	    }
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ *convert_string(ಅಕ್ಷರ *instring, size_t *sizep)
{
    size_t ಉದ್ದಳತೆ = 0;
    ಅಕ್ಷರ *outp, *inp;
    ಅಕ್ಷರ ತಾತ್ಕಾಲಿಕ[3];


    outp = inp = instring;

    ಆಗ (*inp) {
	ಆದರೆ (*inp == '\\') {
	    inp++;
	    ಆದರೆ (*inp && strchr("0123456789abcdefABCDEF",(ಪೂರ್ಣಾಂಕ)*inp)) {
		ತಾತ್ಕಾಲಿಕ[0] = *inp++;
		ತಾತ್ಕಾಲಿಕ[1] = '\0';
		ಆದರೆ (*inp && strchr("0123456789abcdefABCDEF",(ಪೂರ್ಣಾಂಕ)*inp)) {
		    ತಾತ್ಕಾಲಿಕ[1] = *inp++;
		    ತಾತ್ಕಾಲಿಕ[2] = '\0';
		}
		*outp++ = (ಅಕ್ಷರ)strtol((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)ತಾತ್ಕಾಲಿಕ,ಖಾಲಿ,16);
	    } ಇಲ್ಲದಿದ್ದರೆ {
		*outp++ = *inp++;
	    }
	} ಇಲ್ಲದಿದ್ದರೆ {
	    *outp++ = *inp++;
	}
	ಉದ್ದಳತೆ++;
    }
    *sizep = ಉದ್ದಳತೆ;
    ಹಿಂತಿರುಗಿ instring;
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
    cmdname = "";
    arg1 = ಖಾಲಿ;
    arg1len = 0;
    arg2 = ಖಾಲಿ;
    arg2len = 0;

    ಆದರೆ (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ಖಾಲಿ;
    }

    ಆಯ್ಕೆ (argc) {
	ಸ್ಥಿತಿ 1:
	ಸ್ಥಿತಿ 2:
	    /* Interactive mode */
	    ಆಗ ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ಖಾಲಿ;
		ಆದರೆ ((arg1 = strchr((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)cmdname,' ')) != ಖಾಲಿ) {
		    arg1++;
		    arg2 = arg1;
		    ಆಗ (*arg2) {
			ಆದರೆ (*arg2 == ' ') {
			    *arg2++ = '\0';
			    ನಿಲ್ಲಿಸು;
			}
			ಆದರೆ ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		ಆದರೆ (arg1) arg1 = convert_string(arg1,&arg1len);
		ಆದರೆ (arg2) arg2 = convert_string(arg2,&arg2len);
		ಆದರೆ (do_command()) ನಿಲ್ಲಿಸು;
	    }
	    ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 5:
	    arg2 = convert_string(argv[4],&arg2len);
	ಸ್ಥಿತಿ 4:
	    arg1 = convert_string(argv[3],&arg1len);
	ಸ್ಥಿತಿ 3:
	    cmdname = argv[2];
	ಪೂರ್ವನಿಯೋಜಿತ:
	    do_command();
	    ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ (tdb) tdb_close(tdb);

    ಹಿಂತಿರುಗಿ 0;
}
