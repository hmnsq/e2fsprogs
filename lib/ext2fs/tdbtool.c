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

static 整数 do_command(void);
const char *cmdname;
char *arg1, *arg2;
size_t arg1len, arg2len;
整数 bIterate = 0;
char *line;
TDB_DATA iterate_kbuf;
char cmdline[1024];

enum commands {
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

typedef struct {
	const char *name;
	enum commands cmd;
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

static 整数 print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);
static 整数 print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);
static 整数 print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state);

static void print_asc(const char *buf,整数 len)
{
	整数 i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	如果 (buf[len - 1] == 0)
	        len--;

	为 (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

static void print_data(const char *buf,整数 len)
{
	整数 i=0;
	如果 (len<=0) 返回;
	printf("[%03X] ",i);
	为 (i=0;i<len;) {
		printf("%02X ",(整数)buf[i]);
		i++;
		如果 (i%8 == 0) printf(" ");
		如果 (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			如果 (i<len) printf("[%03X] ",i);
		}
	}
	如果 (i%16) {
		整数 n;

		n = 16 - (i%16);
		printf(" ");
		如果 (n>8) printf(" ");
		当 (n--) printf("   ");

		n = i%16;
		如果 (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		如果 (n>0) print_asc(&buf[i-n],n);
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

static void terror(const char *why)
{
	printf("%s\n", why);
}

static void create_tdb(const char *tdbname)
{
	如果 (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	如果 (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

static void open_tdb(const char *tdbname)
{
	如果 (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	如果 (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

static void insert_tdb(char *keyname, size_t keylen, char* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	如果 ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		返回;
	}

	key.dptr = (unsigned char *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned char *)data;
	dbuf.dsize = datalen;

	如果 (tdb_store(tdb, key, dbuf, TDB_INSERT) == -1) {
		terror("insert failed");
	}
}

static void store_tdb(char *keyname, size_t keylen, char* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	如果 ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		返回;
	}

	如果 ((data == NULL) || (datalen == 0)) {
		terror("need data");
		返回;
	}

	key.dptr = (unsigned char *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned char *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, key, dbuf, NULL);

	如果 (tdb_store(tdb, key, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

static void show_tdb(char *keyname, size_t keylen)
{
	TDB_DATA key, dbuf;

	如果 ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		返回;
	}

	key.dptr = (unsigned char *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	如果 (!dbuf.dptr) {
	    terror("fetch failed");
	    返回;
	}

	print_rec(tdb, key, dbuf, NULL);

	free( dbuf.dptr );

	返回;
}

static void delete_tdb(char *keyname, size_t keylen)
{
	TDB_DATA key;

	如果 ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		返回;
	}

	key.dptr = (unsigned char *)keyname;
	key.dsize = keylen;

	如果 (tdb_delete(tdb, key) != 0) {
		terror("delete failed");
	}
}

static void move_rec(char *keyname, size_t keylen, char* tdbname)
{
	TDB_DATA key, dbuf;
	TDB_CONTEXT *dst_tdb;

	如果 ((keyname == NULL) || (keylen == 0)) {
		terror("need key");
		返回;
	}

	如果 ( !tdbname ) {
		terror("need destination tdb name");
		返回;
	}

	key.dptr = (unsigned char *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	如果 (!dbuf.dptr) {
		terror("fetch failed");
		返回;
	}

	print_rec(tdb, key, dbuf, NULL);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	如果 ( !dst_tdb ) {
		terror("unable to open destination tdb");
		返回;
	}

	如果 ( tdb_store( dst_tdb, key, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	否则
		printf("record moved\n");

	tdb_close( dst_tdb );

	返回;
}

static 整数 print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("\nkey %d bytes\n", (整数)key.dsize);
	print_asc((const char *)key.dptr, key.dsize);
	printf("\ndata %d bytes\n", (整数)dbuf.dsize);
	print_data((const char *)dbuf.dptr, dbuf.dsize);
	返回 0;
}

static 整数 print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("key %d bytes: ", (整数)key.dsize);
	print_asc((const char *)key.dptr, key.dsize);
	printf("\n");
	返回 0;
}

static 整数 print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	printf("key %d bytes\n", (整数)key.dsize);
	print_data((const char *)key.dptr, key.dsize);
	printf("\n");
	返回 0;
}

static 整数 total_bytes;

static 整数 traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, void *state)
{
	total_bytes += dbuf.dsize;
	返回 0;
}

static void info_tdb(void)
{
	整数 count;
	total_bytes = 0;
	如果 ((count = tdb_traverse(tdb, traverse_fn, NULL)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	否则
		printf("%d records totalling %d bytes\n", count, total_bytes);
}

static char *tdb_getline(const char *prompt)
{
	static char thisline[1024];
	char *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, sizeof(thisline)-1, stdin);
	如果 (p) p = strchr(p, '\n');
	如果 (p) *p = 0;
	返回 p?thisline:NULL;
}

static 整数 do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf,
                     void *state)
{
    返回 tdb_delete(the_tdb, key);
}

static void first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	如果 (!dbuf.dptr) terror("fetch failed");
	否则 {
		print_rec(the_tdb, *pkey, dbuf, NULL);
	}
}

static void next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	如果 (!dbuf.dptr)
		terror("fetch failed");
	否则
		print_rec(the_tdb, *pkey, dbuf, NULL);
}

static 整数 do_command(void)
{
	COMMAND_TABLE *ctp = cmd_table;
	enum commands mycmd = CMD_HELP;
	整数 cmd_len;

	如果 (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} 否则 {
	    当 (ctp->name) {
		cmd_len = strlen(ctp->name);
		如果 (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			break;
		}
		ctp++;
	    }
	}

	switch (mycmd) {
	case CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    返回 0;
	case CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            返回 0;
	case CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    返回 0;
	case CMD_QUIT:
	    返回 1;
	default:
	    /* all the rest require a open database */
	    如果 (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		返回 0;
	    }
	    switch (mycmd) {
	    case CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, NULL);
		返回 0;
	    case CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, NULL);
		返回 0;
	    case CMD_INSERT:
		bIterate = 0;
		insert_tdb(arg1, arg1len,arg2,arg2len);
		返回 0;
	    case CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		返回 0;
	    case CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		返回 0;
	    case CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		返回 0;
	    case CMD_KEYS:
		tdb_traverse(tdb, print_key, NULL);
		返回 0;
	    case CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, NULL);
		返回 0;
	    case CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		返回 0;
	    case CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		返回 0;
	    case CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		返回 0;
	    case CMD_INFO:
		info_tdb();
		返回 0;
	    case CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		返回 0;
	    case CMD_NEXT:
	       如果 (bIterate)
		  next_record(tdb, &iterate_kbuf);
		返回 0;
	    case CMD_HELP:
		help();
		返回 0;
            case CMD_CREATE_TDB:
            case CMD_OPEN_TDB:
            case CMD_SYSTEM:
            case CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                返回 0;
	    }
	}

	返回 0;
}

static char *convert_string(char *instring, size_t *sizep)
{
    size_t length = 0;
    char *outp, *inp;
    char temp[3];


    outp = inp = instring;

    当 (*inp) {
	如果 (*inp == '\\') {
	    inp++;
	    如果 (*inp && strchr("0123456789abcdefABCDEF",(整数)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		如果 (*inp && strchr("0123456789abcdefABCDEF",(整数)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (char)strtol((const char *)temp,NULL,16);
	    } 否则 {
		*outp++ = *inp++;
	    }
	} 否则 {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    返回 instring;
}

整数 主要(整数 argc, char *argv[])
{
    cmdname = "";
    arg1 = NULL;
    arg1len = 0;
    arg2 = NULL;
    arg2len = 0;

    如果 (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = NULL;
    }

    switch (argc) {
	case 1:
	case 2:
	    /* Interactive mode */
	    当 ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = NULL;
		如果 ((arg1 = strchr((const char *)cmdname,' ')) != NULL) {
		    arg1++;
		    arg2 = arg1;
		    当 (*arg2) {
			如果 (*arg2 == ' ') {
			    *arg2++ = '\0';
			    break;
			}
			如果 ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		如果 (arg1) arg1 = convert_string(arg1,&arg1len);
		如果 (arg2) arg2 = convert_string(arg2,&arg2len);
		如果 (do_command()) break;
	    }
	    break;
	case 5:
	    arg2 = convert_string(argv[4],&arg2len);
	case 4:
	    arg1 = convert_string(argv[3],&arg1len);
	case 3:
	    cmdname = argv[2];
	default:
	    do_command();
	    break;
    }

    如果 (tdb) tdb_close(tdb);

    返回 0;
}
