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

#ضمن "config.h"
#ضمن <errno.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <string.h>
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/mman.h>
#ضمن <sys/stat.h>
#ضمن <sys/time.h>
#ضمن <ctype.h>
#ضمن <signal.h>
#ضمن <stdarg.h>

#ضمن "tdb.h"

ساكن صحيح do_command(فراغ);
ثابت حرف *cmdname;
حرف *arg1, *arg2;
size_t arg1len, arg2len;
صحيح bIterate = 0;
حرف *line;
TDB_DATA iterate_kbuf;
حرف cmdline[1024];

تعداد commands {
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

تعريفنوع بنية {
	ثابت حرف *name;
	تعداد commands cmd;
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
	{عدم,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ساكن TDB_CONTEXT *tdb;

ساكن صحيح print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state);
ساكن صحيح print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state);
ساكن صحيح print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state);

ساكن فراغ print_asc(ثابت حرف *buf,صحيح len)
{
	صحيح i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	𐡄𐡍 (buf[len - 1] == 0)
	        len--;

	لكل (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

ساكن فراغ print_data(ثابت حرف *buf,صحيح len)
{
	صحيح i=0;
	𐡄𐡍 (len<=0) أرجع;
	printf("[%03X] ",i);
	لكل (i=0;i<len;) {
		printf("%02X ",(صحيح)buf[i]);
		i++;
		𐡄𐡍 (i%8 == 0) printf(" ");
		𐡄𐡍 (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			𐡄𐡍 (i<len) printf("[%03X] ",i);
		}
	}
	𐡄𐡍 (i%16) {
		صحيح n;

		n = 16 - (i%16);
		printf(" ");
		𐡄𐡍 (n>8) printf(" ");
		طالما (n--) printf("   ");

		n = i%16;
		𐡄𐡍 (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		𐡄𐡍 (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

ساكن فراغ help(فراغ)
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

ساكن فراغ terror(ثابت حرف *why)
{
	printf("%s\n", why);
}

ساكن فراغ create_tdb(ثابت حرف *tdbname)
{
	𐡄𐡍 (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	𐡄𐡍 (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ساكن فراغ open_tdb(ثابت حرف *tdbname)
{
	𐡄𐡍 (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	𐡄𐡍 (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ساكن فراغ insert_tdb(حرف *keyname, size_t keylen, حرف* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	𐡄𐡍 ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	key.dptr = (unsigned حرف *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned حرف *)data;
	dbuf.dsize = datalen;

	𐡄𐡍 (tdb_store(tdb, key, dbuf, TDB_INSERT) == -1) {
		terror("insert failed");
	}
}

ساكن فراغ store_tdb(حرف *keyname, size_t keylen, حرف* data, size_t datalen)
{
	TDB_DATA key, dbuf;

	𐡄𐡍 ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	𐡄𐡍 ((data == عدم) || (datalen == 0)) {
		terror("need data");
		أرجع;
	}

	key.dptr = (unsigned حرف *)keyname;
	key.dsize = keylen;
	dbuf.dptr = (unsigned حرف *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, key, dbuf, عدم);

	𐡄𐡍 (tdb_store(tdb, key, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ساكن فراغ show_tdb(حرف *keyname, size_t keylen)
{
	TDB_DATA key, dbuf;

	𐡄𐡍 ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	key.dptr = (unsigned حرف *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	𐡄𐡍 (!dbuf.dptr) {
	    terror("fetch failed");
	    أرجع;
	}

	print_rec(tdb, key, dbuf, عدم);

	حرر( dbuf.dptr );

	أرجع;
}

ساكن فراغ delete_tdb(حرف *keyname, size_t keylen)
{
	TDB_DATA key;

	𐡄𐡍 ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	key.dptr = (unsigned حرف *)keyname;
	key.dsize = keylen;

	𐡄𐡍 (tdb_delete(tdb, key) != 0) {
		terror("delete failed");
	}
}

ساكن فراغ move_rec(حرف *keyname, size_t keylen, حرف* tdbname)
{
	TDB_DATA key, dbuf;
	TDB_CONTEXT *dst_tdb;

	𐡄𐡍 ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	𐡄𐡍 ( !tdbname ) {
		terror("need destination tdb name");
		أرجع;
	}

	key.dptr = (unsigned حرف *)keyname;
	key.dsize = keylen;

	dbuf = tdb_fetch(tdb, key);
	𐡄𐡍 (!dbuf.dptr) {
		terror("fetch failed");
		أرجع;
	}

	print_rec(tdb, key, dbuf, عدم);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	𐡄𐡍 ( !dst_tdb ) {
		terror("unable to open destination tdb");
		أرجع;
	}

	𐡄𐡍 ( tdb_store( dst_tdb, key, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	وإلا
		printf("record moved\n");

	tdb_close( dst_tdb );

	أرجع;
}

ساكن صحيح print_rec(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state)
{
	printf("\nkey %d bytes\n", (صحيح)key.dsize);
	print_asc((ثابت حرف *)key.dptr, key.dsize);
	printf("\ndata %d bytes\n", (صحيح)dbuf.dsize);
	print_data((ثابت حرف *)dbuf.dptr, dbuf.dsize);
	أرجع 0;
}

ساكن صحيح print_key(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state)
{
	printf("key %d bytes: ", (صحيح)key.dsize);
	print_asc((ثابت حرف *)key.dptr, key.dsize);
	printf("\n");
	أرجع 0;
}

ساكن صحيح print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state)
{
	printf("key %d bytes\n", (صحيح)key.dsize);
	print_data((ثابت حرف *)key.dptr, key.dsize);
	printf("\n");
	أرجع 0;
}

ساكن صحيح total_bytes;

ساكن صحيح traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf, فراغ *state)
{
	total_bytes += dbuf.dsize;
	أرجع 0;
}

ساكن فراغ info_tdb(فراغ)
{
	صحيح count;
	total_bytes = 0;
	𐡄𐡍 ((count = tdb_traverse(tdb, traverse_fn, عدم)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	وإلا
		printf("%d records totalling %d bytes\n", count, total_bytes);
}

ساكن حرف *tdb_getline(ثابت حرف *prompt)
{
	ساكن حرف thisline[1024];
	حرف *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, حجم(thisline)-1, stdin);
	𐡄𐡍 (p) p = strchr(p, '\n');
	𐡄𐡍 (p) *p = 0;
	أرجع p?thisline:عدم;
}

ساكن صحيح do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA key, TDB_DATA dbuf,
                     فراغ *state)
{
    أرجع tdb_delete(the_tdb, key);
}

ساكن فراغ first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	𐡄𐡍 (!dbuf.dptr) terror("fetch failed");
	وإلا {
		print_rec(the_tdb, *pkey, dbuf, عدم);
	}
}

ساكن فراغ next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	𐡄𐡍 (!dbuf.dptr)
		terror("fetch failed");
	وإلا
		print_rec(the_tdb, *pkey, dbuf, عدم);
}

ساكن صحيح do_command(فراغ)
{
	COMMAND_TABLE *ctp = cmd_table;
	تعداد commands mycmd = CMD_HELP;
	صحيح cmd_len;

	𐡄𐡍 (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} وإلا {
	    طالما (ctp->name) {
		cmd_len = strlen(ctp->name);
		𐡄𐡍 (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			اكسر;
		}
		ctp++;
	    }
	}

	بدل (mycmd) {
	حالة CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    أرجع 0;
	حالة CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            أرجع 0;
	حالة CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    أرجع 0;
	حالة CMD_QUIT:
	    أرجع 1;
	افتراضي:
	    /* all the rest require a open database */
	    𐡄𐡍 (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		أرجع 0;
	    }
	    بدل (mycmd) {
	    حالة CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, عدم);
		أرجع 0;
	    حالة CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, عدم);
		أرجع 0;
	    حالة CMD_INSERT:
		bIterate = 0;
		insert_tdb(arg1, arg1len,arg2,arg2len);
		أرجع 0;
	    حالة CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		أرجع 0;
	    حالة CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		أرجع 0;
	    حالة CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		أرجع 0;
	    حالة CMD_KEYS:
		tdb_traverse(tdb, print_key, عدم);
		أرجع 0;
	    حالة CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, عدم);
		أرجع 0;
	    حالة CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		أرجع 0;
	    حالة CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		أرجع 0;
	    حالة CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		أرجع 0;
	    حالة CMD_INFO:
		info_tdb();
		أرجع 0;
	    حالة CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		أرجع 0;
	    حالة CMD_NEXT:
	       𐡄𐡍 (bIterate)
		  next_record(tdb, &iterate_kbuf);
		أرجع 0;
	    حالة CMD_HELP:
		help();
		أرجع 0;
            حالة CMD_CREATE_TDB:
            حالة CMD_OPEN_TDB:
            حالة CMD_SYSTEM:
            حالة CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                أرجع 0;
	    }
	}

	أرجع 0;
}

ساكن حرف *convert_string(حرف *instring, size_t *sizep)
{
    size_t length = 0;
    حرف *outp, *inp;
    حرف temp[3];


    outp = inp = instring;

    طالما (*inp) {
	𐡄𐡍 (*inp == '\\') {
	    inp++;
	    𐡄𐡍 (*inp && strchr("0123456789abcdefABCDEF",(صحيح)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		𐡄𐡍 (*inp && strchr("0123456789abcdefABCDEF",(صحيح)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (حرف)strtol((ثابت حرف *)temp,عدم,16);
	    } وإلا {
		*outp++ = *inp++;
	    }
	} وإلا {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    أرجع instring;
}

صحيح main(صحيح argc, حرف *argv[])
{
    cmdname = "";
    arg1 = عدم;
    arg1len = 0;
    arg2 = عدم;
    arg2len = 0;

    𐡄𐡍 (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = عدم;
    }

    بدل (argc) {
	حالة 1:
	حالة 2:
	    /* Interactive mode */
	    طالما ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = عدم;
		𐡄𐡍 ((arg1 = strchr((ثابت حرف *)cmdname,' ')) != عدم) {
		    arg1++;
		    arg2 = arg1;
		    طالما (*arg2) {
			𐡄𐡍 (*arg2 == ' ') {
			    *arg2++ = '\0';
			    اكسر;
			}
			𐡄𐡍 ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		𐡄𐡍 (arg1) arg1 = convert_string(arg1,&arg1len);
		𐡄𐡍 (arg2) arg2 = convert_string(arg2,&arg2len);
		𐡄𐡍 (do_command()) اكسر;
	    }
	    اكسر;
	حالة 5:
	    arg2 = convert_string(argv[4],&arg2len);
	حالة 4:
	    arg1 = convert_string(argv[3],&arg1len);
	حالة 3:
	    cmdname = argv[2];
	افتراضي:
	    do_command();
	    اكسر;
    }

    𐡄𐡍 (tdb) tdb_close(tdb);

    أرجع 0;
}
