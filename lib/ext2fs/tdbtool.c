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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_command(ಶೂನ್ಯ);
ಧ್ರುವ ವರ್ಣ *cmdname;
ವರ್ಣ *arg1, *arg2;
size_t arg1len, arg2len;
ಪೂರ್ಣಾಙ್ಕ bIterate = 0;
ವರ್ಣ *ಸಾಲು;
TDB_DATA iterate_kbuf;
ವರ್ಣ cmdline[1024];

ಗಣನಾ commands {
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
	CMD_FIRST,
	CMD_NEXT,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
	ಧ್ರುವ ವರ್ಣ *name;
	ಗಣನಾ commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
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
	{"first",	CMD_FIRST},
	{"1",		CMD_FIRST},
	{"next",	CMD_NEXT},
	{"n",		CMD_NEXT},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{ರಿಕ್ತ,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ಸ್ಥಿರ TDB_CONTEXT *tdb;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_ಕೀಲಿ(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state);

ಸ್ಥಿರ ಶೂನ್ಯ print_asc(ಧ್ರುವ ವರ್ಣ *buf,ಪೂರ್ಣಾಙ್ಕ len)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	ಯದಿ (buf[len - 1] == 0)
	        len--;

	ಪ್ರತಿ (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

ಸ್ಥಿರ ಶೂನ್ಯ print_data(ಧ್ರುವ ವರ್ಣ *buf,ಪೂರ್ಣಾಙ್ಕ len)
{
	ಪೂರ್ಣಾಙ್ಕ i=0;
	ಯದಿ (len<=0) ಪ್ರತ್ಯಾಗಮ;
	printf("[%03X] ",i);
	ಪ್ರತಿ (i=0;i<len;) {
		printf("%02X ",(ಪೂರ್ಣಾಙ್ಕ)buf[i]);
		i++;
		ಯದಿ (i%8 == 0) printf(" ");
		ಯದಿ (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			ಯದಿ (i<len) printf("[%03X] ",i);
		}
	}
	ಯದಿ (i%16) {
		ಪೂರ್ಣಾಙ್ಕ n;

		n = 16 - (i%16);
		printf(" ");
		ಯದಿ (n>8) printf(" ");
		ಯಾವತ್ (n--) printf("   ");

		n = i%16;
		ಯದಿ (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		ಯದಿ (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ help(ಶೂನ್ಯ)
{
	printf("\n"
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

ಸ್ಥಿರ ಶೂನ್ಯ terror(ಧ್ರುವ ವರ್ಣ *why)
{
	printf("%s\n", why);
}

ಸ್ಥಿರ ಶೂನ್ಯ create_tdb(ಧ್ರುವ ವರ್ಣ *tdbname)
{
	ಯದಿ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	ಯದಿ (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ open_tdb(ಧ್ರುವ ವರ್ಣ *tdbname)
{
	ಯದಿ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ಯದಿ (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_tdb(ವರ್ಣ *keyname, size_t keylen, ವರ್ಣ* data, size_t datalen)
{
	TDB_DATA ಕೀಲಿ, dbuf;

	ಯದಿ ((keyname == ರಿಕ್ತ) || (keylen == 0)) {
		terror("need key");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *)keyname;
	ಕೀಲಿ.dsize = keylen;
	dbuf.dptr = (unsigned ವರ್ಣ *)data;
	dbuf.dsize = datalen;

	ಯದಿ (tdb_store(tdb, ಕೀಲಿ, dbuf, TDB_ಸೇರಿಸು) == -1) {
		terror("insert failed");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ store_tdb(ವರ್ಣ *keyname, size_t keylen, ವರ್ಣ* data, size_t datalen)
{
	TDB_DATA ಕೀಲಿ, dbuf;

	ಯದಿ ((keyname == ರಿಕ್ತ) || (keylen == 0)) {
		terror("need key");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ ((data == ರಿಕ್ತ) || (datalen == 0)) {
		terror("need data");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *)keyname;
	ಕೀಲಿ.dsize = keylen;
	dbuf.dptr = (unsigned ವರ್ಣ *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, ಕೀಲಿ, dbuf, ರಿಕ್ತ);

	ಯದಿ (tdb_store(tdb, ಕೀಲಿ, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_tdb(ವರ್ಣ *keyname, size_t keylen)
{
	TDB_DATA ಕೀಲಿ, dbuf;

	ಯದಿ ((keyname == ರಿಕ್ತ) || (keylen == 0)) {
		terror("need key");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *)keyname;
	ಕೀಲಿ.dsize = keylen;

	dbuf = tdb_fetch(tdb, ಕೀಲಿ);
	ಯದಿ (!dbuf.dptr) {
	    terror("fetch failed");
	    ಪ್ರತ್ಯಾಗಮ;
	}

	print_rec(tdb, ಕೀಲಿ, dbuf, ರಿಕ್ತ);

	ಮುಕ್ತ( dbuf.dptr );

	ಪ್ರತ್ಯಾಗಮ;
}

ಸ್ಥಿರ ಶೂನ್ಯ delete_tdb(ವರ್ಣ *keyname, size_t keylen)
{
	TDB_DATA ಕೀಲಿ;

	ಯದಿ ((keyname == ರಿಕ್ತ) || (keylen == 0)) {
		terror("need key");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *)keyname;
	ಕೀಲಿ.dsize = keylen;

	ಯದಿ (tdb_delete(tdb, ಕೀಲಿ) != 0) {
		terror("delete failed");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ move_rec(ವರ್ಣ *keyname, size_t keylen, ವರ್ಣ* tdbname)
{
	TDB_DATA ಕೀಲಿ, dbuf;
	TDB_CONTEXT *dst_tdb;

	ಯದಿ ((keyname == ರಿಕ್ತ) || (keylen == 0)) {
		terror("need key");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ ( !tdbname ) {
		terror("need destination tdb name");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಕೀಲಿ.dptr = (unsigned ವರ್ಣ *)keyname;
	ಕೀಲಿ.dsize = keylen;

	dbuf = tdb_fetch(tdb, ಕೀಲಿ);
	ಯದಿ (!dbuf.dptr) {
		terror("fetch failed");
		ಪ್ರತ್ಯಾಗಮ;
	}

	print_rec(tdb, ಕೀಲಿ, dbuf, ರಿಕ್ತ);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ಯದಿ ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ ( tdb_store( dst_tdb, ಕೀಲಿ, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	ಅನ್ಯಥಾ
		printf("record moved\n");

	tdb_close( dst_tdb );

	ಪ್ರತ್ಯಾಗಮ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state)
{
	printf("\nkey %d bytes\n", (ಪೂರ್ಣಾಙ್ಕ)ಕೀಲಿ.dsize);
	print_asc((ಧ್ರುವ ವರ್ಣ *)ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	printf("\ndata %d bytes\n", (ಪೂರ್ಣಾಙ್ಕ)dbuf.dsize);
	print_data((ಧ್ರುವ ವರ್ಣ *)dbuf.dptr, dbuf.dsize);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_ಕೀಲಿ(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state)
{
	printf("key %d bytes: ", (ಪೂರ್ಣಾಙ್ಕ)ಕೀಲಿ.dsize);
	print_asc((ಧ್ರುವ ವರ್ಣ *)ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	printf("\n");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state)
{
	printf("key %d bytes\n", (ಪೂರ್ಣಾಙ್ಕ)ಕೀಲಿ.dsize);
	print_data((ಧ್ರುವ ವರ್ಣ *)ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	printf("\n");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ total_bytes;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf, ಶೂನ್ಯ *state)
{
	total_bytes += dbuf.dsize;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ info_tdb(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;
	total_bytes = 0;
	ಯದಿ ((ಎಣಿಕೆ = tdb_traverse(tdb, traverse_fn, ರಿಕ್ತ)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	ಅನ್ಯಥಾ
		printf("%d records totalling %d bytes\n", ಎಣಿಕೆ, total_bytes);
}

ಸ್ಥಿರ ವರ್ಣ *tdb_getline(ಧ್ರುವ ವರ್ಣ *prompt)
{
	ಸ್ಥಿರ ವರ್ಣ thisline[1024];
	ವರ್ಣ *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, ಪರಿಮಾಣ(thisline)-1, stdin);
	ಯದಿ (p) p = strchr(p, '\n');
	ಯದಿ (p) *p = 0;
	ಪ್ರತ್ಯಾಗಮ p?thisline:ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA ಕೀಲಿ, TDB_DATA dbuf,
                     ಶೂನ್ಯ *state)
{
    ಪ್ರತ್ಯಾಗಮ tdb_delete(the_tdb, ಕೀಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ಯದಿ (!dbuf.dptr) terror("fetch failed");
	ಅನ್ಯಥಾ {
		print_rec(the_tdb, *pkey, dbuf, ರಿಕ್ತ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ಯದಿ (!dbuf.dptr)
		terror("fetch failed");
	ಅನ್ಯಥಾ
		print_rec(the_tdb, *pkey, dbuf, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_command(ಶೂನ್ಯ)
{
	COMMAND_TABLE *ctp = cmd_table;
	ಗಣನಾ commands mycmd = CMD_HELP;
	ಪೂರ್ಣಾಙ್ಕ cmd_len;

	ಯದಿ (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} ಅನ್ಯಥಾ {
	    ಯಾವತ್ (ctp->name) {
		cmd_len = strlen(ctp->name);
		ಯದಿ (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			ಭಙ್ಗ;
		}
		ctp++;
	    }
	}

	ವಿಕಲ್ಪ (mycmd) {
	ಪಕ್ಷ CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ CMD_QUIT:
	    ಪ್ರತ್ಯಾಗಮ 1;
	ಸ್ವಭಾವ:
	    /* all the rest require a open database */
	    ಯದಿ (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ಪ್ರತ್ಯಾಗಮ 0;
	    }
	    ವಿಕಲ್ಪ (mycmd) {
	    ಪಕ್ಷ CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_ಸೇರಿಸು:
		bIterate = 0;
		ಸೇರಿಸು_tdb(arg1, arg1len,arg2,arg2len);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_ಕೀಲಿಗಳು:
		tdb_traverse(tdb, print_ಕೀಲಿ, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_INFO:
		info_tdb();
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_NEXT:
	       ಯದಿ (bIterate)
		  next_record(tdb, &iterate_kbuf);
		ಪ್ರತ್ಯಾಗಮ 0;
	    ಪಕ್ಷ CMD_HELP:
		help();
		ಪ್ರತ್ಯಾಗಮ 0;
            ಪಕ್ಷ CMD_CREATE_TDB:
            ಪಕ್ಷ CMD_OPEN_TDB:
            ಪಕ್ಷ CMD_SYSTEM:
            ಪಕ್ಷ CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ಪ್ರತ್ಯಾಗಮ 0;
	    }
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ವರ್ಣ *convert_string(ವರ್ಣ *instring, size_t *sizep)
{
    size_t length = 0;
    ವರ್ಣ *outp, *inp;
    ವರ್ಣ temp[3];


    outp = inp = instring;

    ಯಾವತ್ (*inp) {
	ಯದಿ (*inp == '\\') {
	    inp++;
	    ಯದಿ (*inp && strchr("0123456789abcdefABCDEF",(ಪೂರ್ಣಾಙ್ಕ)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		ಯದಿ (*inp && strchr("0123456789abcdefABCDEF",(ಪೂರ್ಣಾಙ್ಕ)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (ವರ್ಣ)strtol((ಧ್ರುವ ವರ್ಣ *)temp,ರಿಕ್ತ,16);
	    } ಅನ್ಯಥಾ {
		*outp++ = *inp++;
	    }
	} ಅನ್ಯಥಾ {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    ಪ್ರತ್ಯಾಗಮ instring;
}

ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[])
{
    cmdname = "";
    arg1 = ರಿಕ್ತ;
    arg1len = 0;
    arg2 = ರಿಕ್ತ;
    arg2len = 0;

    ಯದಿ (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ರಿಕ್ತ;
    }

    ವಿಕಲ್ಪ (argc) {
	ಪಕ್ಷ 1:
	ಪಕ್ಷ 2:
	    /* Interactive mode */
	    ಯಾವತ್ ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ರಿಕ್ತ;
		ಯದಿ ((arg1 = strchr((ಧ್ರುವ ವರ್ಣ *)cmdname,' ')) != ರಿಕ್ತ) {
		    arg1++;
		    arg2 = arg1;
		    ಯಾವತ್ (*arg2) {
			ಯದಿ (*arg2 == ' ') {
			    *arg2++ = '\0';
			    ಭಙ್ಗ;
			}
			ಯದಿ ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		ಯದಿ (arg1) arg1 = convert_string(arg1,&arg1len);
		ಯದಿ (arg2) arg2 = convert_string(arg2,&arg2len);
		ಯದಿ (do_command()) ಭಙ್ಗ;
	    }
	    ಭಙ್ಗ;
	ಪಕ್ಷ 5:
	    arg2 = convert_string(argv[4],&arg2len);
	ಪಕ್ಷ 4:
	    arg1 = convert_string(argv[3],&arg1len);
	ಪಕ್ಷ 3:
	    cmdname = argv[2];
	ಸ್ವಭಾವ:
	    do_command();
	    ಭಙ್ಗ;
    }

    ಯದಿ (tdb) tdb_close(tdb);

    ಪ್ರತ್ಯಾಗಮ 0;
}
