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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/mman.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <ctype.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <stdarg.h>

#அந்தர்பாவ "tdb.h"

நிலையான முழு do_command(வெறுமை);
மாறிலி எழுத்து *cmdname;
எழுத்து *arg1, *arg2;
size_t arg1len, arg2len;
முழு bIterate = 0;
எழுத்து *வரி;
TDB_DATA iterate_kbuf;
எழுத்து cmdline[1024];

எண்ணிக்கை commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_செருகு,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_சாவிகள்,
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

வகைவரை கட்டமைப்பு {
	மாறிலி எழுத்து *name;
	எண்ணிக்கை commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_செருகு},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_சாவிகள்},
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
	{வெற்று,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

நிலையான TDB_CONTEXT *tdb;

நிலையான முழு print_rec(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state);
நிலையான முழு print_சாவி(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state);
நிலையான முழு print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state);

நிலையான வெறுமை print_asc(மாறிலி எழுத்து *buf,முழு len)
{
	முழு i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	எனில் (buf[len - 1] == 0)
	        len--;

	க்கு (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

நிலையான வெறுமை print_data(மாறிலி எழுத்து *buf,முழு len)
{
	முழு i=0;
	எனில் (len<=0) திரும்பு;
	printf("[%03X] ",i);
	க்கு (i=0;i<len;) {
		printf("%02X ",(முழு)buf[i]);
		i++;
		எனில் (i%8 == 0) printf(" ");
		எனில் (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			எனில் (i<len) printf("[%03X] ",i);
		}
	}
	எனில் (i%16) {
		முழு n;

		n = 16 - (i%16);
		printf(" ");
		எனில் (n>8) printf(" ");
		போது (n--) printf("   ");

		n = i%16;
		எனில் (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		எனில் (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

நிலையான வெறுமை help(வெறுமை)
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

நிலையான வெறுமை terror(மாறிலி எழுத்து *why)
{
	printf("%s\n", why);
}

நிலையான வெறுமை create_tdb(மாறிலி எழுத்து *tdbname)
{
	எனில் (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	எனில் (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

நிலையான வெறுமை open_tdb(மாறிலி எழுத்து *tdbname)
{
	எனில் (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	எனில் (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

நிலையான வெறுமை செருகு_tdb(எழுத்து *keyname, size_t keylen, எழுத்து* data, size_t datalen)
{
	TDB_DATA சாவி, dbuf;

	எனில் ((keyname == வெற்று) || (keylen == 0)) {
		terror("need key");
		திரும்பு;
	}

	சாவி.dptr = (unsigned எழுத்து *)keyname;
	சாவி.dsize = keylen;
	dbuf.dptr = (unsigned எழுத்து *)data;
	dbuf.dsize = datalen;

	எனில் (tdb_store(tdb, சாவி, dbuf, TDB_செருகு) == -1) {
		terror("insert failed");
	}
}

நிலையான வெறுமை store_tdb(எழுத்து *keyname, size_t keylen, எழுத்து* data, size_t datalen)
{
	TDB_DATA சாவி, dbuf;

	எனில் ((keyname == வெற்று) || (keylen == 0)) {
		terror("need key");
		திரும்பு;
	}

	எனில் ((data == வெற்று) || (datalen == 0)) {
		terror("need data");
		திரும்பு;
	}

	சாவி.dptr = (unsigned எழுத்து *)keyname;
	சாவி.dsize = keylen;
	dbuf.dptr = (unsigned எழுத்து *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, சாவி, dbuf, வெற்று);

	எனில் (tdb_store(tdb, சாவி, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

நிலையான வெறுமை show_tdb(எழுத்து *keyname, size_t keylen)
{
	TDB_DATA சாவி, dbuf;

	எனில் ((keyname == வெற்று) || (keylen == 0)) {
		terror("need key");
		திரும்பு;
	}

	சாவி.dptr = (unsigned எழுத்து *)keyname;
	சாவி.dsize = keylen;

	dbuf = tdb_fetch(tdb, சாவி);
	எனில் (!dbuf.dptr) {
	    terror("fetch failed");
	    திரும்பு;
	}

	print_rec(tdb, சாவி, dbuf, வெற்று);

	விடுவி( dbuf.dptr );

	திரும்பு;
}

நிலையான வெறுமை delete_tdb(எழுத்து *keyname, size_t keylen)
{
	TDB_DATA சாவி;

	எனில் ((keyname == வெற்று) || (keylen == 0)) {
		terror("need key");
		திரும்பு;
	}

	சாவி.dptr = (unsigned எழுத்து *)keyname;
	சாவி.dsize = keylen;

	எனில் (tdb_delete(tdb, சாவி) != 0) {
		terror("delete failed");
	}
}

நிலையான வெறுமை move_rec(எழுத்து *keyname, size_t keylen, எழுத்து* tdbname)
{
	TDB_DATA சாவி, dbuf;
	TDB_CONTEXT *dst_tdb;

	எனில் ((keyname == வெற்று) || (keylen == 0)) {
		terror("need key");
		திரும்பு;
	}

	எனில் ( !tdbname ) {
		terror("need destination tdb name");
		திரும்பு;
	}

	சாவி.dptr = (unsigned எழுத்து *)keyname;
	சாவி.dsize = keylen;

	dbuf = tdb_fetch(tdb, சாவி);
	எனில் (!dbuf.dptr) {
		terror("fetch failed");
		திரும்பு;
	}

	print_rec(tdb, சாவி, dbuf, வெற்று);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	எனில் ( !dst_tdb ) {
		terror("unable to open destination tdb");
		திரும்பு;
	}

	எனில் ( tdb_store( dst_tdb, சாவி, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	இல்லையெனில்
		printf("record moved\n");

	tdb_close( dst_tdb );

	திரும்பு;
}

நிலையான முழு print_rec(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state)
{
	printf("\nkey %d bytes\n", (முழு)சாவி.dsize);
	print_asc((மாறிலி எழுத்து *)சாவி.dptr, சாவி.dsize);
	printf("\ndata %d bytes\n", (முழு)dbuf.dsize);
	print_data((மாறிலி எழுத்து *)dbuf.dptr, dbuf.dsize);
	திரும்பு 0;
}

நிலையான முழு print_சாவி(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state)
{
	printf("key %d bytes: ", (முழு)சாவி.dsize);
	print_asc((மாறிலி எழுத்து *)சாவி.dptr, சாவி.dsize);
	printf("\n");
	திரும்பு 0;
}

நிலையான முழு print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state)
{
	printf("key %d bytes\n", (முழு)சாவி.dsize);
	print_data((மாறிலி எழுத்து *)சாவி.dptr, சாவி.dsize);
	printf("\n");
	திரும்பு 0;
}

நிலையான முழு total_bytes;

நிலையான முழு traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf, வெறுமை *state)
{
	total_bytes += dbuf.dsize;
	திரும்பு 0;
}

நிலையான வெறுமை info_tdb(வெறுமை)
{
	முழு மொத்தம்;
	total_bytes = 0;
	எனில் ((மொத்தம் = tdb_traverse(tdb, traverse_fn, வெற்று)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	இல்லையெனில்
		printf("%d records totalling %d bytes\n", மொத்தம், total_bytes);
}

நிலையான எழுத்து *tdb_getline(மாறிலி எழுத்து *prompt)
{
	நிலையான எழுத்து thisline[1024];
	எழுத்து *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, அளவு(thisline)-1, stdin);
	எனில் (p) p = strchr(p, '\n');
	எனில் (p) *p = 0;
	திரும்பு p?thisline:வெற்று;
}

நிலையான முழு do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA சாவி, TDB_DATA dbuf,
                     வெறுமை *state)
{
    திரும்பு tdb_delete(the_tdb, சாவி);
}

நிலையான வெறுமை first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	எனில் (!dbuf.dptr) terror("fetch failed");
	இல்லையெனில் {
		print_rec(the_tdb, *pkey, dbuf, வெற்று);
	}
}

நிலையான வெறுமை next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	எனில் (!dbuf.dptr)
		terror("fetch failed");
	இல்லையெனில்
		print_rec(the_tdb, *pkey, dbuf, வெற்று);
}

நிலையான முழு do_command(வெறுமை)
{
	COMMAND_TABLE *ctp = cmd_table;
	எண்ணிக்கை commands mycmd = CMD_HELP;
	முழு cmd_len;

	எனில் (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} இல்லையெனில் {
	    போது (ctp->name) {
		cmd_len = strlen(ctp->name);
		எனில் (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			நிறுத்து;
		}
		ctp++;
	    }
	}

	தேர்வு (mycmd) {
	நிலை CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    திரும்பு 0;
	நிலை CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            திரும்பு 0;
	நிலை CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    திரும்பு 0;
	நிலை CMD_QUIT:
	    திரும்பு 1;
	இயல்பு:
	    /* all the rest require a open database */
	    எனில் (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		திரும்பு 0;
	    }
	    தேர்வு (mycmd) {
	    நிலை CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, வெற்று);
		திரும்பு 0;
	    நிலை CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, வெற்று);
		திரும்பு 0;
	    நிலை CMD_செருகு:
		bIterate = 0;
		செருகு_tdb(arg1, arg1len,arg2,arg2len);
		திரும்பு 0;
	    நிலை CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		திரும்பு 0;
	    நிலை CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		திரும்பு 0;
	    நிலை CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		திரும்பு 0;
	    நிலை CMD_சாவிகள்:
		tdb_traverse(tdb, print_சாவி, வெற்று);
		திரும்பு 0;
	    நிலை CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, வெற்று);
		திரும்பு 0;
	    நிலை CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		திரும்பு 0;
	    நிலை CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		திரும்பு 0;
	    நிலை CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		திரும்பு 0;
	    நிலை CMD_INFO:
		info_tdb();
		திரும்பு 0;
	    நிலை CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		திரும்பு 0;
	    நிலை CMD_NEXT:
	       எனில் (bIterate)
		  next_record(tdb, &iterate_kbuf);
		திரும்பு 0;
	    நிலை CMD_HELP:
		help();
		திரும்பு 0;
            நிலை CMD_CREATE_TDB:
            நிலை CMD_OPEN_TDB:
            நிலை CMD_SYSTEM:
            நிலை CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                திரும்பு 0;
	    }
	}

	திரும்பு 0;
}

நிலையான எழுத்து *convert_string(எழுத்து *instring, size_t *sizep)
{
    size_t length = 0;
    எழுத்து *outp, *inp;
    எழுத்து temp[3];


    outp = inp = instring;

    போது (*inp) {
	எனில் (*inp == '\\') {
	    inp++;
	    எனில் (*inp && strchr("0123456789abcdefABCDEF",(முழு)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		எனில் (*inp && strchr("0123456789abcdefABCDEF",(முழு)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (எழுத்து)strtol((மாறிலி எழுத்து *)temp,வெற்று,16);
	    } இல்லையெனில் {
		*outp++ = *inp++;
	    }
	} இல்லையெனில் {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    திரும்பு instring;
}

முழு முதன்மை(முழு argc, எழுத்து *argv[])
{
    cmdname = "";
    arg1 = வெற்று;
    arg1len = 0;
    arg2 = வெற்று;
    arg2len = 0;

    எனில் (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = வெற்று;
    }

    தேர்வு (argc) {
	நிலை 1:
	நிலை 2:
	    /* Interactive mode */
	    போது ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = வெற்று;
		எனில் ((arg1 = strchr((மாறிலி எழுத்து *)cmdname,' ')) != வெற்று) {
		    arg1++;
		    arg2 = arg1;
		    போது (*arg2) {
			எனில் (*arg2 == ' ') {
			    *arg2++ = '\0';
			    நிறுத்து;
			}
			எனில் ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		எனில் (arg1) arg1 = convert_string(arg1,&arg1len);
		எனில் (arg2) arg2 = convert_string(arg2,&arg2len);
		எனில் (do_command()) நிறுத்து;
	    }
	    நிறுத்து;
	நிலை 5:
	    arg2 = convert_string(argv[4],&arg2len);
	நிலை 4:
	    arg1 = convert_string(argv[3],&arg1len);
	நிலை 3:
	    cmdname = argv[2];
	இயல்பு:
	    do_command();
	    நிறுத்து;
    }

    எனில் (tdb) tdb_close(tdb);

    திரும்பு 0;
}
