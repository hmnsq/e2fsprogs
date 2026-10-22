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

static ចំនួនគត់ do_command(void);
const var_na *cmdname;
var_na *arg1, *arg2;
size_t arg1len, arg2len;
ចំនួនគត់ bIterate = 0;
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

static ចំនួនគត់ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);
static ចំនួនគត់ print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);
static ចំនួនគត់ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);

static void print_asc(const var_na *buf,ចំនួនគត់ len)
{
	ចំនួនគត់ i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	ប្រសិនបើ (buf[len - 1] == 0)
	        len--;

	សម្រាប់ (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

static void print_data(const var_na *buf,ចំនួនគត់ len)
{
	ចំនួនគត់ i=0;
	ប្រសិនបើ (len<=0) ត្រឡប់;
	printf("[%03X] ",i);
	សម្រាប់ (i=0;i<len;) {
		printf("%02X ",(ចំនួនគត់)buf[i]);
		i++;
		ប្រសិនបើ (i%8 == 0) printf(" ");
		ប្រសិនបើ (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			ប្រសិនបើ (i<len) printf("[%03X] ",i);
		}
	}
	ប្រសិនបើ (i%16) {
		ចំនួនគត់ n;

		n = 16 - (i%16);
		printf(" ");
		ប្រសិនបើ (n>8) printf(" ");
		ខណៈពេល (n--) printf("   ");

		n = i%16;
		ប្រសិនបើ (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		ប្រសិនបើ (n>0) print_asc(&buf[i-n],n);
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
	ប្រសិនបើ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	ប្រសិនបើ (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

static void open_tdb(const var_na *tdbname)
{
	ប្រសិនបើ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ប្រសិនបើ (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

static void insert_tdb(var_na *keyname, size_t keylen, var_na* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	ប្រសិនបើ ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ត្រឡប់;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned var_na *)data;
	dbuf.dsize = datalen;

	ប្រសិនបើ (tdb_store(tdb, key, dbuf, TDB_INSERT) == -1) {
		terror("insert failed");
	}
}

static void store_tdb(var_na *keyname, size_t keylen, var_na* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	ប្រសិនបើ ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ត្រឡប់;
	}

	ប្រសិនបើ ((data == NULL) || (datalen == 0)) {
		terror("need data");
		ត្រឡប់;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned var_na *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, key, dbuf, NULL);

	ប្រសិនបើ (tdb_store(tdb, key, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

static void show_tdb(var_na *keyname, size_t keylen)
{
	TDB_DATA key, dbuf;

	ប្រសិនបើ ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ត្រឡប់;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	ប្រសិនបើ (!dbuf.dptr) {
	    terror("fetch failed");
	    ត្រឡប់;
	}

	print_rec(tdb, key, dbuf, NULL);

	free( dbuf.dptr );

	ត្រឡប់;
}

static void delete_tdb(var_na *keyname, size_t keylen)
{
	TDB_DATA key;

	ប្រសិនបើ ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ត្រឡប់;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;

	ប្រសិនបើ (tdb_delete(tdb, key) != 0) {
		terror("delete failed");
	}
}

static void move_rec(var_na *keyname, size_t keylen, var_na* tdbname)
{
	TDB_DATA key, dbuf;
	TDB_CONTEXT *dst_tdb;

	ប្រសិនបើ ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		ត្រឡប់;
	}

	ប្រសិនបើ ( !tdbname ) {
		terror("need destination tdb name");
		ត្រឡប់;
	}

	key.dptr = (unsigned var_na *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	ប្រសិនបើ (!dbuf.dptr) {
		terror("fetch failed");
		ត្រឡប់;
	}

	print_rec(tdb, key, dbuf, NULL);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ប្រសិនបើ ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ត្រឡប់;
	}

	ប្រសិនបើ ( tdb_store( dst_tdb, key, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	បើមិនដូច្នេះ
		printf("record moved\n");

	tdb_close( dst_tdb );

	ត្រឡប់;
}

static ចំនួនគត់ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("\nkey %d bytes\n", (ចំនួនគត់)key.dsize);
	print_asc((const var_na *)key.dptr, key.dsize);
	printf("\ndata %d bytes\n", (ចំនួនគត់)dbuf.dsize);
	print_data((const var_na *)dbuf.dptr, dbuf.dsize);
	ត្រឡប់ 0;
}

static ចំនួនគត់ print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("key %d bytes: ", (ចំនួនគត់)key.dsize);
	print_asc((const var_na *)key.dptr, key.dsize);
	printf("\n");
	ត្រឡប់ 0;
}

static ចំនួនគត់ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("key %d bytes\n", (ចំនួនគត់)key.dsize);
	print_data((const var_na *)key.dptr, key.dsize);
	printf("\n");
	ត្រឡប់ 0;
}

static ចំនួនគត់ total_bytes;

static ចំនួនគត់ traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	total_bytes += dbuf.dsize;
	ត្រឡប់ 0;
}

static void info_tdb(void)
{
	ចំនួនគត់ count;
	total_bytes = 0;
	ប្រសិនបើ ((count = tdb_traverse(tdb, traverse_fn, NULL)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	បើមិនដូច្នេះ
		printf("%d records totalling %d bytes\n", count, total_bytes);
}

static var_na *tdb_getline(const var_na *prompt)
{
	static var_na thisline[1024];
	var_na *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, parimaa_na(thisline)-1, stdin);
	ប្រសិនបើ (p) p = strchr(p, '\n');
	ប្រសិនបើ (p) *p = 0;
	ត្រឡប់ p?thisline:NULL;
}

static ចំនួនគត់ do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf,
                     void *state)
{
    ត្រឡប់ tdb_delete(the_tdb, key);
}

static void first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ប្រសិនបើ (!dbuf.dptr) terror("fetch failed");
	បើមិនដូច្នេះ {
		print_rec(the_tdb, *pkey, dbuf, NULL);
	}
}

static void next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ប្រសិនបើ (!dbuf.dptr)
		terror("fetch failed");
	បើមិនដូច្នេះ
		print_rec(the_tdb, *pkey, dbuf, NULL);
}

static ចំនួនគត់ do_command(void)
{
	COMMAND_TABLE *ctp = cmd_table;
	ga_nanaa commands mycmd = CMD_HELP;
	ចំនួនគត់ cmd_len;

	ប្រសិនបើ (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} បើមិនដូច្នេះ {
	    ខណៈពេល (ctp->name) {
		cmd_len = strlen(ctp->name);
		ប្រសិនបើ (strncmp(ctp->name,cmdname,cmd_len) == 0) {
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
	    ត្រឡប់ 0;
	pak_sa CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ត្រឡប់ 0;
	pak_sa CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ត្រឡប់ 0;
	pak_sa CMD_QUIT:
	    ត្រឡប់ 1;
	default:
	    /* all the rest require a open database */
	    ប្រសិនបើ (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ត្រឡប់ 0;
	    }
	    switch (mycmd) {
	    pak_sa CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, NULL);
		ត្រឡប់ 0;
	    pak_sa CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, NULL);
		ត្រឡប់ 0;
	    pak_sa CMD_INSERT:
		bIterate = 0;
		insert_tdb(arg1, arg1len,arg2,arg2len);
		ត្រឡប់ 0;
	    pak_sa CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ត្រឡប់ 0;
	    pak_sa CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ត្រឡប់ 0;
	    pak_sa CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ត្រឡប់ 0;
	    pak_sa CMD_KEYS:
		tdb_traverse(tdb, print_key, NULL);
		ត្រឡប់ 0;
	    pak_sa CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, NULL);
		ត្រឡប់ 0;
	    pak_sa CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ត្រឡប់ 0;
	    pak_sa CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ត្រឡប់ 0;
	    pak_sa CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ត្រឡប់ 0;
	    pak_sa CMD_INFO:
		info_tdb();
		ត្រឡប់ 0;
	    pak_sa CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		ត្រឡប់ 0;
	    pak_sa CMD_NEXT:
	       ប្រសិនបើ (bIterate)
		  next_record(tdb, &iterate_kbuf);
		ត្រឡប់ 0;
	    pak_sa CMD_HELP:
		help();
		ត្រឡប់ 0;
            pak_sa CMD_CREATE_TDB:
            pak_sa CMD_OPEN_TDB:
            pak_sa CMD_SYSTEM:
            pak_sa CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ត្រឡប់ 0;
	    }
	}

	ត្រឡប់ 0;
}

static var_na *convert_string(var_na *instring, size_t *sizep)
{
    size_t length = 0;
    var_na *outp, *inp;
    var_na temp[3];


    outp = inp = instring;

    ខណៈពេល (*inp) {
	ប្រសិនបើ (*inp == '\\') {
	    inp++;
	    ប្រសិនបើ (*inp && strchr("0123456789abcdefABCDEF",(ចំនួនគត់)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		ប្រសិនបើ (*inp && strchr("0123456789abcdefABCDEF",(ចំនួនគត់)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (var_na)strtol((const var_na *)temp,NULL,16);
	    } បើមិនដូច្នេះ {
		*outp++ = *inp++;
	    }
	} បើមិនដូច្នេះ {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    ត្រឡប់ instring;
}

ចំនួនគត់ main(ចំនួនគត់ argc, var_na *argv[])
{
    cmdname = "";
    arg1 = NULL;
    arg1len = 0;
    arg2 = NULL;
    arg2len = 0;

    ប្រសិនបើ (argv[1]) {
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
	    ខណៈពេល ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = NULL;
		ប្រសិនបើ ((arg1 = strchr((const var_na *)cmdname,' ')) != NULL) {
		    arg1++;
		    arg2 = arg1;
		    ខណៈពេល (*arg2) {
			ប្រសិនបើ (*arg2 == ' ') {
			    *arg2++ = '\0';
			    bha_nga;
			}
			ប្រសិនបើ ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		ប្រសិនបើ (arg1) arg1 = convert_string(arg1,&arg1len);
		ប្រសិនបើ (arg2) arg2 = convert_string(arg2,&arg2len);
		ប្រសិនបើ (do_command()) bha_nga;
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

    ប្រសិនបើ (tdb) tdb_close(tdb);

    ត្រឡប់ 0;
}
