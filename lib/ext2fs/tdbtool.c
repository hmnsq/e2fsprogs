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

#include "config.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ctype.h>
#include <signal.h>
#include <stdarg.h>

#include "tdb.h"

static ཧྲིལ do_command(void);
const var_na *cmdname;
var_na *arg1, *arg2;
size_t arg1len, arg2len;
ཧྲིལ bIterate = 0;
var_na *line;
TDB_DATA iterate_kbuf;
var_na cmdline[1024];

ga_nanaa commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_INSERT,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_KEYS,
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

typedef sa_nracanaa {
	const var_na *name;
	ga_nanaa commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_INSERT},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_KEYS},
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
	{NULL,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

static TDB_CONTEXT *tdb;

static ཧྲིལ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);
static ཧྲིལ print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);
static ཧྲིལ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);

static void print_asc(const var_na *buf,ཧྲིལ len)
{
	ཧྲིལ i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	གལ་སྲིད (buf[len - 1] == 0)
	        len--;

	དོན་ལུ (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

static void print_data(const var_na *buf,ཧྲིལ len)
{
	ཧྲིལ i=0;
	གལ་སྲིད (len<=0) ལོག;
	printf("[%03X] ",i);
	དོན་ལུ (i=0;i<len;) {
		printf("%02X ",(ཧྲིལ)buf[i]);
		i++;
		གལ་སྲིད (i%8 == 0) printf(" ");
		གལ་སྲིད (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			གལ་སྲིད (i<len) printf("[%03X] ",i);
		}
	}
	གལ་སྲིད (i%16) {
		ཧྲིལ n;

		n = 16 - (i%16);
		printf(" ");
		གལ་སྲིད (n>8) printf(" ");
		སྐབས (n--) printf("   ");

		n = i%16;
		གལ་སྲིད (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		གལ་སྲིད (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

static void help(void)
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

static void terror(const var_na *why)
{
	printf("%s\n", why);
}

static void create_tdb(const var_na *tdbname)
{
	གལ་སྲིད (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	གལ་སྲིད (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

static void open_tdb(const var_na *tdbname)
{
	གལ་སྲིད (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	གལ་སྲིད (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

static void insert_tdb(var_na *keyname, size_t keylen, var_na* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	གལ་སྲིད ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ལོག;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned var_na *)data;
	dbuf.dsize = datalen;

	གལ་སྲིད (tdb_store(tdb, key, dbuf, TDB_INSERT) == -1) {
		terror("insert failed");
	}
}

static void store_tdb(var_na *keyname, size_t keylen, var_na* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	གལ་སྲིད ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ལོག;
	}

	གལ་སྲིད ((data == NULL) || (datalen == 0)) {
		terror("need data");
		ལོག;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned var_na *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, key, dbuf, NULL);

	གལ་སྲིད (tdb_store(tdb, key, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

static void show_tdb(var_na *keyname, size_t keylen)
{
	TDB_DATA key, dbuf;

	གལ་སྲིད ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ལོག;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	གལ་སྲིད (!dbuf.dptr) {
	    terror("fetch failed");
	    ལོག;
	}

	print_rec(tdb, key, dbuf, NULL);

	free( dbuf.dptr );

	ལོག;
}

static void delete_tdb(var_na *keyname, size_t keylen)
{
	TDB_DATA key;

	གལ་སྲིད ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ལོག;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;

	གལ་སྲིད (tdb_delete(tdb, key) != 0) {
		terror("delete failed");
	}
}

static void move_rec(var_na *keyname, size_t keylen, var_na* tdbname)
{
	TDB_DATA key, dbuf;
	TDB_CONTEXT *dst_tdb;

	གལ་སྲིད ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ལོག;
	}

	གལ་སྲིད ( !tdbname ) {
		terror("need destination tdb name");
		ལོག;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	གལ་སྲིད (!dbuf.dptr) {
		terror("fetch failed");
		ལོག;
	}

	print_rec(tdb, key, dbuf, NULL);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	གལ་སྲིད ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ལོག;
	}

	གལ་སྲིད ( tdb_store( dst_tdb, key, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	དེ་མིན
		printf("record moved\n");

	tdb_close( dst_tdb );

	ལོག;
}

static ཧྲིལ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("\nkey %d bytes\n", (ཧྲིལ)key.dsize);
	print_asc((const var_na *)key.dptr, key.dsize);
	printf("\ndata %d bytes\n", (ཧྲིལ)dbuf.dsize);
	print_data((const var_na *)dbuf.dptr, dbuf.dsize);
	ལོག 0;
}

static ཧྲིལ print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("key %d bytes: ", (ཧྲིལ)key.dsize);
	print_asc((const var_na *)key.dptr, key.dsize);
	printf("\n");
	ལོག 0;
}

static ཧྲིལ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("key %d bytes\n", (ཧྲིལ)key.dsize);
	print_data((const var_na *)key.dptr, key.dsize);
	printf("\n");
	ལོག 0;
}

static ཧྲིལ total_bytes;

static ཧྲིལ traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	total_bytes += dbuf.dsize;
	ལོག 0;
}

static void info_tdb(void)
{
	ཧྲིལ count;
	total_bytes = 0;
	གལ་སྲིད ((count = tdb_traverse(tdb, traverse_fn, NULL)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	དེ་མིན
		printf("%d records totalling %d bytes\n", count, total_bytes);
}

static var_na *tdb_getline(const var_na *prompt)
{
	static var_na thisline[1024];
	var_na *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, parimaa_na(thisline)-1, stdin);
	གལ་སྲིད (p) p = strchr(p, '\n');
	གལ་སྲིད (p) *p = 0;
	ལོག p?thisline:NULL;
}

static ཧྲིལ do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf,
                     void *state)
{
    ལོག tdb_delete(the_tdb, key);
}

static void first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	གལ་སྲིད (!dbuf.dptr) terror("fetch failed");
	དེ་མིན {
		print_rec(the_tdb, *pkey, dbuf, NULL);
	}
}

static void next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	གལ་སྲིད (!dbuf.dptr)
		terror("fetch failed");
	དེ་མིན
		print_rec(the_tdb, *pkey, dbuf, NULL);
}

static ཧྲིལ do_command(void)
{
	COMMAND_TABLE *ctp = cmd_table;
	ga_nanaa commands mycmd = CMD_HELP;
	ཧྲིལ cmd_len;

	གལ་སྲིད (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} དེ་མིན {
	    སྐབས (ctp->name) {
		cmd_len = strlen(ctp->name);
		གལ་སྲིད (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			bha_nga;
		}
		ctp++;
	    }
	}

	switch (mycmd) {
	pak_sa CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    ལོག 0;
	pak_sa CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ལོག 0;
	pak_sa CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ལོག 0;
	pak_sa CMD_QUIT:
	    ལོག 1;
	default:
	    /* all the rest require a open database */
	    གལ་སྲིད (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ལོག 0;
	    }
	    switch (mycmd) {
	    pak_sa CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, NULL);
		ལོག 0;
	    pak_sa CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, NULL);
		ལོག 0;
	    pak_sa CMD_INSERT:
		bIterate = 0;
		insert_tdb(arg1, arg1len,arg2,arg2len);
		ལོག 0;
	    pak_sa CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ལོག 0;
	    pak_sa CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ལོག 0;
	    pak_sa CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ལོག 0;
	    pak_sa CMD_KEYS:
		tdb_traverse(tdb, print_key, NULL);
		ལོག 0;
	    pak_sa CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, NULL);
		ལོག 0;
	    pak_sa CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ལོག 0;
	    pak_sa CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ལོག 0;
	    pak_sa CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ལོག 0;
	    pak_sa CMD_INFO:
		info_tdb();
		ལོག 0;
	    pak_sa CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		ལོག 0;
	    pak_sa CMD_NEXT:
	       གལ་སྲིད (bIterate)
		  next_record(tdb, &iterate_kbuf);
		ལོག 0;
	    pak_sa CMD_HELP:
		help();
		ལོག 0;
            pak_sa CMD_CREATE_TDB:
            pak_sa CMD_OPEN_TDB:
            pak_sa CMD_SYSTEM:
            pak_sa CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ལོག 0;
	    }
	}

	ལོག 0;
}

static var_na *convert_string(var_na *instring, size_t *sizep)
{
    size_t length = 0;
    var_na *outp, *inp;
    var_na temp[3];


    outp = inp = instring;

    སྐབས (*inp) {
	གལ་སྲིད (*inp == '\\') {
	    inp++;
	    གལ་སྲིད (*inp && strchr("0123456789abcdefABCDEF",(ཧྲིལ)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		གལ་སྲིད (*inp && strchr("0123456789abcdefABCDEF",(ཧྲིལ)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (var_na)strtol((const var_na *)temp,NULL,16);
	    } དེ་མིན {
		*outp++ = *inp++;
	    }
	} དེ་མིན {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    ལོག instring;
}

ཧྲིལ main(ཧྲིལ argc, var_na *argv[])
{
    cmdname = "";
    arg1 = NULL;
    arg1len = 0;
    arg2 = NULL;
    arg2len = 0;

    གལ་སྲིད (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = NULL;
    }

    switch (argc) {
	pak_sa 1:
	pak_sa 2:
	    /* Interactive mode */
	    སྐབས ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = NULL;
		གལ་སྲིད ((arg1 = strchr((const var_na *)cmdname,' ')) != NULL) {
		    arg1++;
		    arg2 = arg1;
		    སྐབས (*arg2) {
			གལ་སྲིད (*arg2 == ' ') {
			    *arg2++ = '\0';
			    bha_nga;
			}
			གལ་སྲིད ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		གལ་སྲིད (arg1) arg1 = convert_string(arg1,&arg1len);
		གལ་སྲིད (arg2) arg2 = convert_string(arg2,&arg2len);
		གལ་སྲིད (do_command()) bha_nga;
	    }
	    bha_nga;
	pak_sa 5:
	    arg2 = convert_string(argv[4],&arg2len);
	pak_sa 4:
	    arg1 = convert_string(argv[3],&arg1len);
	pak_sa 3:
	    cmdname = argv[2];
	default:
	    do_command();
	    bha_nga;
    }

    གལ་སྲིད (tdb) tdb_close(tdb);

    ལོག 0;
}
