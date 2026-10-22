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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/mman.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <ctype.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <stdarg.h>

#അന്തര്ഭാവ "tdb.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ do_command(ശൂന്യം);
സ്ഥിരം അക്ഷരം *cmdname;
അക്ഷരം *arg1, *arg2;
size_t arg1len, arg2len;
പൂർണ്ണസംഖ്യ bIterate = 0;
അക്ഷരം *വരി;
TDB_DATA iterate_kbuf;
അക്ഷരം cmdline[1024];

എണ്ണൽ commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_തിരുകുക,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_താക്കോലുകൾ,
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

തരനിർവചനം ഘടന {
	സ്ഥിരം അക്ഷരം *name;
	എണ്ണൽ commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_തിരുകുക},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_താക്കോലുകൾ},
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
	{ശൂന്യ,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

സ്ഥിതം TDB_CONTEXT *tdb;

സ്ഥിതം പൂർണ്ണസംഖ്യ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state);
സ്ഥിതം പൂർണ്ണസംഖ്യ print_താക്കോൽ(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state);
സ്ഥിതം പൂർണ്ണസംഖ്യ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state);

സ്ഥിതം ശൂന്യം print_asc(സ്ഥിരം അക്ഷരം *buf,പൂർണ്ണസംഖ്യ len)
{
	പൂർണ്ണസംഖ്യ i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	എങ്കിൽ (buf[len - 1] == 0)
	        len--;

	വേണ്ടി (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

സ്ഥിതം ശൂന്യം print_data(സ്ഥിരം അക്ഷരം *buf,പൂർണ്ണസംഖ്യ len)
{
	പൂർണ്ണസംഖ്യ i=0;
	എങ്കിൽ (len<=0) തിരികെ;
	printf("[%03X] ",i);
	വേണ്ടി (i=0;i<len;) {
		printf("%02X ",(പൂർണ്ണസംഖ്യ)buf[i]);
		i++;
		എങ്കിൽ (i%8 == 0) printf(" ");
		എങ്കിൽ (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			എങ്കിൽ (i<len) printf("[%03X] ",i);
		}
	}
	എങ്കിൽ (i%16) {
		പൂർണ്ണസംഖ്യ n;

		n = 16 - (i%16);
		printf(" ");
		എങ്കിൽ (n>8) printf(" ");
		ആകുമ്പോൾ (n--) printf("   ");

		n = i%16;
		എങ്കിൽ (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		എങ്കിൽ (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

സ്ഥിതം ശൂന്യം help(ശൂന്യം)
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

സ്ഥിതം ശൂന്യം terror(സ്ഥിരം അക്ഷരം *why)
{
	printf("%s\n", why);
}

സ്ഥിതം ശൂന്യം create_tdb(സ്ഥിരം അക്ഷരം *tdbname)
{
	എങ്കിൽ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	എങ്കിൽ (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

സ്ഥിതം ശൂന്യം open_tdb(സ്ഥിരം അക്ഷരം *tdbname)
{
	എങ്കിൽ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	എങ്കിൽ (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

സ്ഥിതം ശൂന്യം തിരുകുക_tdb(അക്ഷരം *keyname, size_t keylen, അക്ഷരം* data, size_t datalen)
{
	TDB_DATA താക്കോൽ, dbuf;

	എങ്കിൽ ((keyname == ശൂന്യ) || (keylen == 0)) {
		terror("need key");
		തിരികെ;
	}

	താക്കോൽ.dptr = (unsigned അക്ഷരം *)keyname;
	താക്കോൽ.dsize = keylen;
	dbuf.dptr = (unsigned അക്ഷരം *)data;
	dbuf.dsize = datalen;

	എങ്കിൽ (tdb_store(tdb, താക്കോൽ, dbuf, TDB_തിരുകുക) == -1) {
		terror("insert failed");
	}
}

സ്ഥിതം ശൂന്യം store_tdb(അക്ഷരം *keyname, size_t keylen, അക്ഷരം* data, size_t datalen)
{
	TDB_DATA താക്കോൽ, dbuf;

	എങ്കിൽ ((keyname == ശൂന്യ) || (keylen == 0)) {
		terror("need key");
		തിരികെ;
	}

	എങ്കിൽ ((data == ശൂന്യ) || (datalen == 0)) {
		terror("need data");
		തിരികെ;
	}

	താക്കോൽ.dptr = (unsigned അക്ഷരം *)keyname;
	താക്കോൽ.dsize = keylen;
	dbuf.dptr = (unsigned അക്ഷരം *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, താക്കോൽ, dbuf, ശൂന്യ);

	എങ്കിൽ (tdb_store(tdb, താക്കോൽ, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

സ്ഥിതം ശൂന്യം show_tdb(അക്ഷരം *keyname, size_t keylen)
{
	TDB_DATA താക്കോൽ, dbuf;

	എങ്കിൽ ((keyname == ശൂന്യ) || (keylen == 0)) {
		terror("need key");
		തിരികെ;
	}

	താക്കോൽ.dptr = (unsigned അക്ഷരം *)keyname;
	താക്കോൽ.dsize = keylen;

	dbuf = tdb_fetch(tdb, താക്കോൽ);
	എങ്കിൽ (!dbuf.dptr) {
	    terror("fetch failed");
	    തിരികെ;
	}

	print_rec(tdb, താക്കോൽ, dbuf, ശൂന്യ);

	സ്വതന്ത്രം( dbuf.dptr );

	തിരികെ;
}

സ്ഥിതം ശൂന്യം delete_tdb(അക്ഷരം *keyname, size_t keylen)
{
	TDB_DATA താക്കോൽ;

	എങ്കിൽ ((keyname == ശൂന്യ) || (keylen == 0)) {
		terror("need key");
		തിരികെ;
	}

	താക്കോൽ.dptr = (unsigned അക്ഷരം *)keyname;
	താക്കോൽ.dsize = keylen;

	എങ്കിൽ (tdb_delete(tdb, താക്കോൽ) != 0) {
		terror("delete failed");
	}
}

സ്ഥിതം ശൂന്യം move_rec(അക്ഷരം *keyname, size_t keylen, അക്ഷരം* tdbname)
{
	TDB_DATA താക്കോൽ, dbuf;
	TDB_CONTEXT *dst_tdb;

	എങ്കിൽ ((keyname == ശൂന്യ) || (keylen == 0)) {
		terror("need key");
		തിരികെ;
	}

	എങ്കിൽ ( !tdbname ) {
		terror("need destination tdb name");
		തിരികെ;
	}

	താക്കോൽ.dptr = (unsigned അക്ഷരം *)keyname;
	താക്കോൽ.dsize = keylen;

	dbuf = tdb_fetch(tdb, താക്കോൽ);
	എങ്കിൽ (!dbuf.dptr) {
		terror("fetch failed");
		തിരികെ;
	}

	print_rec(tdb, താക്കോൽ, dbuf, ശൂന്യ);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	എങ്കിൽ ( !dst_tdb ) {
		terror("unable to open destination tdb");
		തിരികെ;
	}

	എങ്കിൽ ( tdb_store( dst_tdb, താക്കോൽ, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	അല്ലെങ്കിൽ
		printf("record moved\n");

	tdb_close( dst_tdb );

	തിരികെ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_rec(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state)
{
	printf("\nkey %d bytes\n", (പൂർണ്ണസംഖ്യ)താക്കോൽ.dsize);
	print_asc((സ്ഥിരം അക്ഷരം *)താക്കോൽ.dptr, താക്കോൽ.dsize);
	printf("\ndata %d bytes\n", (പൂർണ്ണസംഖ്യ)dbuf.dsize);
	print_data((സ്ഥിരം അക്ഷരം *)dbuf.dptr, dbuf.dsize);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_താക്കോൽ(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state)
{
	printf("key %d bytes: ", (പൂർണ്ണസംഖ്യ)താക്കോൽ.dsize);
	print_asc((സ്ഥിരം അക്ഷരം *)താക്കോൽ.dptr, താക്കോൽ.dsize);
	printf("\n");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state)
{
	printf("key %d bytes\n", (പൂർണ്ണസംഖ്യ)താക്കോൽ.dsize);
	print_data((സ്ഥിരം അക്ഷരം *)താക്കോൽ.dptr, താക്കോൽ.dsize);
	printf("\n");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ total_bytes;

സ്ഥിതം പൂർണ്ണസംഖ്യ traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf, ശൂന്യം *state)
{
	total_bytes += dbuf.dsize;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം info_tdb(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ എണ്ണം;
	total_bytes = 0;
	എങ്കിൽ ((എണ്ണം = tdb_traverse(tdb, traverse_fn, ശൂന്യ)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	അല്ലെങ്കിൽ
		printf("%d records totalling %d bytes\n", എണ്ണം, total_bytes);
}

സ്ഥിതം അക്ഷരം *tdb_getline(സ്ഥിരം അക്ഷരം *prompt)
{
	സ്ഥിതം അക്ഷരം thisline[1024];
	അക്ഷരം *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, വലുപ്പം(thisline)-1, stdin);
	എങ്കിൽ (p) p = strchr(p, '\n');
	എങ്കിൽ (p) *p = 0;
	തിരികെ p?thisline:ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA താക്കോൽ, TDB_DATA dbuf,
                     ശൂന്യം *state)
{
    തിരികെ tdb_delete(the_tdb, താക്കോൽ);
}

സ്ഥിതം ശൂന്യം first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	എങ്കിൽ (!dbuf.dptr) terror("fetch failed");
	അല്ലെങ്കിൽ {
		print_rec(the_tdb, *pkey, dbuf, ശൂന്യ);
	}
}

സ്ഥിതം ശൂന്യം next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	എങ്കിൽ (!dbuf.dptr)
		terror("fetch failed");
	അല്ലെങ്കിൽ
		print_rec(the_tdb, *pkey, dbuf, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_command(ശൂന്യം)
{
	COMMAND_TABLE *ctp = cmd_table;
	എണ്ണൽ commands mycmd = CMD_HELP;
	പൂർണ്ണസംഖ്യ cmd_len;

	എങ്കിൽ (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} അല്ലെങ്കിൽ {
	    ആകുമ്പോൾ (ctp->name) {
		cmd_len = strlen(ctp->name);
		എങ്കിൽ (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			നിർത്തുക;
		}
		ctp++;
	    }
	}

	തിരഞ്ഞെടുപ്പ് (mycmd) {
	അവസ്ഥ CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    തിരികെ 0;
	അവസ്ഥ CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            തിരികെ 0;
	അവസ്ഥ CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    തിരികെ 0;
	അവസ്ഥ CMD_QUIT:
	    തിരികെ 1;
	സ്ഥിരസ്ഥിതി:
	    /* all the rest require a open database */
	    എങ്കിൽ (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		തിരികെ 0;
	    }
	    തിരഞ്ഞെടുപ്പ് (mycmd) {
	    അവസ്ഥ CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ശൂന്യ);
		തിരികെ 0;
	    അവസ്ഥ CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ശൂന്യ);
		തിരികെ 0;
	    അവസ്ഥ CMD_തിരുകുക:
		bIterate = 0;
		തിരുകുക_tdb(arg1, arg1len,arg2,arg2len);
		തിരികെ 0;
	    അവസ്ഥ CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		തിരികെ 0;
	    അവസ്ഥ CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		തിരികെ 0;
	    അവസ്ഥ CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		തിരികെ 0;
	    അവസ്ഥ CMD_താക്കോലുകൾ:
		tdb_traverse(tdb, print_താക്കോൽ, ശൂന്യ);
		തിരികെ 0;
	    അവസ്ഥ CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ശൂന്യ);
		തിരികെ 0;
	    അവസ്ഥ CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		തിരികെ 0;
	    അവസ്ഥ CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		തിരികെ 0;
	    അവസ്ഥ CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		തിരികെ 0;
	    അവസ്ഥ CMD_INFO:
		info_tdb();
		തിരികെ 0;
	    അവസ്ഥ CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		തിരികെ 0;
	    അവസ്ഥ CMD_NEXT:
	       എങ്കിൽ (bIterate)
		  next_record(tdb, &iterate_kbuf);
		തിരികെ 0;
	    അവസ്ഥ CMD_HELP:
		help();
		തിരികെ 0;
            അവസ്ഥ CMD_CREATE_TDB:
            അവസ്ഥ CMD_OPEN_TDB:
            അവസ്ഥ CMD_SYSTEM:
            അവസ്ഥ CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                തിരികെ 0;
	    }
	}

	തിരികെ 0;
}

സ്ഥിതം അക്ഷരം *convert_string(അക്ഷരം *instring, size_t *sizep)
{
    size_t length = 0;
    അക്ഷരം *outp, *inp;
    അക്ഷരം temp[3];


    outp = inp = instring;

    ആകുമ്പോൾ (*inp) {
	എങ്കിൽ (*inp == '\\') {
	    inp++;
	    എങ്കിൽ (*inp && strchr("0123456789abcdefABCDEF",(പൂർണ്ണസംഖ്യ)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		എങ്കിൽ (*inp && strchr("0123456789abcdefABCDEF",(പൂർണ്ണസംഖ്യ)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (അക്ഷരം)strtol((സ്ഥിരം അക്ഷരം *)temp,ശൂന്യ,16);
	    } അല്ലെങ്കിൽ {
		*outp++ = *inp++;
	    }
	} അല്ലെങ്കിൽ {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    തിരികെ instring;
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[])
{
    cmdname = "";
    arg1 = ശൂന്യ;
    arg1len = 0;
    arg2 = ശൂന്യ;
    arg2len = 0;

    എങ്കിൽ (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ശൂന്യ;
    }

    തിരഞ്ഞെടുപ്പ് (argc) {
	അവസ്ഥ 1:
	അവസ്ഥ 2:
	    /* Interactive mode */
	    ആകുമ്പോൾ ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ശൂന്യ;
		എങ്കിൽ ((arg1 = strchr((സ്ഥിരം അക്ഷരം *)cmdname,' ')) != ശൂന്യ) {
		    arg1++;
		    arg2 = arg1;
		    ആകുമ്പോൾ (*arg2) {
			എങ്കിൽ (*arg2 == ' ') {
			    *arg2++ = '\0';
			    നിർത്തുക;
			}
			എങ്കിൽ ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		എങ്കിൽ (arg1) arg1 = convert_string(arg1,&arg1len);
		എങ്കിൽ (arg2) arg2 = convert_string(arg2,&arg2len);
		എങ്കിൽ (do_command()) നിർത്തുക;
	    }
	    നിർത്തുക;
	അവസ്ഥ 5:
	    arg2 = convert_string(argv[4],&arg2len);
	അവസ്ഥ 4:
	    arg1 = convert_string(argv[3],&arg1len);
	അവസ്ഥ 3:
	    cmdname = argv[2];
	സ്ഥിരസ്ഥിതി:
	    do_command();
	    നിർത്തുക;
    }

    എങ്കിൽ (tdb) tdb_close(tdb);

    തിരികെ 0;
}
