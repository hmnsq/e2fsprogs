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

#شامل "config.h"
#شامل <errno.h>
#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <string.h>
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/mman.h>
#شامل <sys/stat.h>
#شامل <sys/time.h>
#شامل <ctype.h>
#شامل <signal.h>
#شامل <stdarg.h>

#شامل "tdb.h"

ساکن صحیح do_command(خالی);
ثابت حرف *cmdname;
حرف *arg1, *arg2;
size_t arg1len, arg2len;
صحیح bIterate = 0;
حرف *سطر;
TDB_ڈیٹا iterate_kbuf;
حرف cmdline[1024];

شمار commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_داخل_کرو,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_کلیدیں,
	CMD_HEXKEYS,
	CMD_DELETE,
	CMD_LIST_HASH_FREE,
	CMD_LIST_FREE,
	CMD_INFO,
	CMD_پہلا,
	CMD_اگلا,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

قسمتعریف ساخت {
	ثابت حرف *name;
	شمار commands cmd;
} COMMAND_جدول;

COMMAND_جدول cmd_جدول[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_داخل_کرو},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_کلیدیں},
	{"hexkeys",	CMD_HEXKEYS},
	{"delete",	CMD_DELETE},
	{"list",	CMD_LIST_HASH_FREE},
	{"free",	CMD_LIST_FREE},
	{"info",	CMD_INFO},
	{"first",	CMD_پہلا},
	{"1",		CMD_پہلا},
	{"next",	CMD_اگلا},
	{"n",		CMD_اگلا},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{تہی,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ساکن TDB_CONTEXT *tdb;

ساکن صحیح print_rec(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state);
ساکن صحیح print_کلید(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state);
ساکن صحیح print_hexkey(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state);

ساکن خالی print_asc(ثابت حرف *buf,صحیح len)
{
	صحیح i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	اگر (buf[len - 1] == 0)
	        len--;

	برائے (i=0;i<len;i++)
		لکھو("%c",isprint(buf[i])?buf[i]:'.');
}

ساکن خالی print_ڈیٹا(ثابت حرف *buf,صحیح len)
{
	صحیح i=0;
	اگر (len<=0) واپس;
	لکھو("[%03X] ",i);
	برائے (i=0;i<len;) {
		لکھو("%02X ",(صحیح)buf[i]);
		i++;
		اگر (i%8 == 0) لکھو(" ");
		اگر (i%16 == 0) {
			print_asc(&buf[i-16],8); لکھو(" ");
			print_asc(&buf[i-8],8); لکھو("\n");
			اگر (i<len) لکھو("[%03X] ",i);
		}
	}
	اگر (i%16) {
		صحیح n;

		n = 16 - (i%16);
		لکھو(" ");
		اگر (n>8) لکھو(" ");
		جب (n--) لکھو("   ");

		n = i%16;
		اگر (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); لکھو(" ");
		n = (i%16) - n;
		اگر (n>0) print_asc(&buf[i-n],n);
		لکھو("\n");
	}
}

ساکن خالی help(خالی)
{
	لکھو("\n"
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

ساکن خالی terror(ثابت حرف *why)
{
	لکھو("%s\n", why);
}

ساکن خالی create_tdb(ثابت حرف *tdbname)
{
	اگر (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_پہلا,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	اگر (!tdb) {
		لکھو("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ساکن خالی open_tdb(ثابت حرف *tdbname)
{
	اگر (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	اگر (!tdb) {
		لکھو("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ساکن خالی داخل_کرو_tdb(حرف *keyname, size_t keylen, حرف* ڈیٹا, size_t datalen)
{
	TDB_ڈیٹا کلید, dbuf;

	اگر ((keyname == تہی) || (keylen == 0)) {
		terror("need key");
		واپس;
	}

	کلید.dptr = (غیرعلامتی حرف *)keyname;
	کلید.dsize = keylen;
	dbuf.dptr = (غیرعلامتی حرف *)ڈیٹا;
	dbuf.dsize = datalen;

	اگر (tdb_store(tdb, کلید, dbuf, TDB_داخل_کرو) == -1) {
		terror("insert failed");
	}
}

ساکن خالی store_tdb(حرف *keyname, size_t keylen, حرف* ڈیٹا, size_t datalen)
{
	TDB_ڈیٹا کلید, dbuf;

	اگر ((keyname == تہی) || (keylen == 0)) {
		terror("need key");
		واپس;
	}

	اگر ((ڈیٹا == تہی) || (datalen == 0)) {
		terror("need data");
		واپس;
	}

	کلید.dptr = (غیرعلامتی حرف *)keyname;
	کلید.dsize = keylen;
	dbuf.dptr = (غیرعلامتی حرف *)ڈیٹا;
	dbuf.dsize = datalen;

	لکھو("Storing key:\n");
	print_rec(tdb, کلید, dbuf, تہی);

	اگر (tdb_store(tdb, کلید, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ساکن خالی show_tdb(حرف *keyname, size_t keylen)
{
	TDB_ڈیٹا کلید, dbuf;

	اگر ((keyname == تہی) || (keylen == 0)) {
		terror("need key");
		واپس;
	}

	کلید.dptr = (غیرعلامتی حرف *)keyname;
	کلید.dsize = keylen;

	dbuf = tdb_fetch(tdb, کلید);
	اگر (!dbuf.dptr) {
	    terror("fetch failed");
	    واپس;
	}

	print_rec(tdb, کلید, dbuf, تہی);

	آزاد( dbuf.dptr );

	واپس;
}

ساکن خالی delete_tdb(حرف *keyname, size_t keylen)
{
	TDB_ڈیٹا کلید;

	اگر ((keyname == تہی) || (keylen == 0)) {
		terror("need key");
		واپس;
	}

	کلید.dptr = (غیرعلامتی حرف *)keyname;
	کلید.dsize = keylen;

	اگر (tdb_delete(tdb, کلید) != 0) {
		terror("delete failed");
	}
}

ساکن خالی move_rec(حرف *keyname, size_t keylen, حرف* tdbname)
{
	TDB_ڈیٹا کلید, dbuf;
	TDB_CONTEXT *dst_tdb;

	اگر ((keyname == تہی) || (keylen == 0)) {
		terror("need key");
		واپس;
	}

	اگر ( !tdbname ) {
		terror("need destination tdb name");
		واپس;
	}

	کلید.dptr = (غیرعلامتی حرف *)keyname;
	کلید.dsize = keylen;

	dbuf = tdb_fetch(tdb, کلید);
	اگر (!dbuf.dptr) {
		terror("fetch failed");
		واپس;
	}

	print_rec(tdb, کلید, dbuf, تہی);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	اگر ( !dst_tdb ) {
		terror("unable to open destination tdb");
		واپس;
	}

	اگر ( tdb_store( dst_tdb, کلید, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	ورنہ
		لکھو("record moved\n");

	tdb_close( dst_tdb );

	واپس;
}

ساکن صحیح print_rec(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state)
{
	لکھو("\nkey %d bytes\n", (صحیح)کلید.dsize);
	print_asc((ثابت حرف *)کلید.dptr, کلید.dsize);
	لکھو("\ndata %d bytes\n", (صحیح)dbuf.dsize);
	print_ڈیٹا((ثابت حرف *)dbuf.dptr, dbuf.dsize);
	واپس 0;
}

ساکن صحیح print_کلید(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state)
{
	لکھو("key %d bytes: ", (صحیح)کلید.dsize);
	print_asc((ثابت حرف *)کلید.dptr, کلید.dsize);
	لکھو("\n");
	واپس 0;
}

ساکن صحیح print_hexkey(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state)
{
	لکھو("key %d bytes\n", (صحیح)کلید.dsize);
	print_ڈیٹا((ثابت حرف *)کلید.dptr, کلید.dsize);
	لکھو("\n");
	واپس 0;
}

ساکن صحیح کل_بائٹیں;

ساکن صحیح traverse_fn(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf, خالی *state)
{
	کل_بائٹیں += dbuf.dsize;
	واپس 0;
}

ساکن خالی info_tdb(خالی)
{
	صحیح گنتی;
	کل_بائٹیں = 0;
	اگر ((گنتی = tdb_traverse(tdb, traverse_fn, تہی)) == -1)
		لکھو("Error = %s\n", tdb_errorstr(tdb));
	ورنہ
		لکھو("%d records totalling %d bytes\n", گنتی, کل_بائٹیں);
}

ساکن حرف *tdb_getline(ثابت حرف *prompt)
{
	ساکن حرف thisline[1024];
	حرف *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, حجم(thisline)-1, stdin);
	اگر (p) p = strchr(p, '\n');
	اگر (p) *p = 0;
	واپس p?thisline:تہی;
}

ساکن صحیح do_delete_fn(TDB_CONTEXT *the_tdb, TDB_ڈیٹا کلید, TDB_ڈیٹا dbuf,
                     خالی *state)
{
    واپس tdb_delete(the_tdb, کلید);
}

ساکن خالی پہلا_ریکارڈ(TDB_CONTEXT *the_tdb, TDB_ڈیٹا *pkey)
{
	TDB_ڈیٹا dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	اگر (!dbuf.dptr) terror("fetch failed");
	ورنہ {
		print_rec(the_tdb, *pkey, dbuf, تہی);
	}
}

ساکن خالی اگلا_ریکارڈ(TDB_CONTEXT *the_tdb, TDB_ڈیٹا *pkey)
{
	TDB_ڈیٹا dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	اگر (!dbuf.dptr)
		terror("fetch failed");
	ورنہ
		print_rec(the_tdb, *pkey, dbuf, تہی);
}

ساکن صحیح do_command(خالی)
{
	COMMAND_جدول *ctp = cmd_جدول;
	شمار commands mycmd = CMD_HELP;
	صحیح cmd_len;

	اگر (cmdname && لڑی_لمبائی(cmdname) == 0) {
	    mycmd = CMD_اگلا;
	} ورنہ {
	    جب (ctp->name) {
		cmd_len = لڑی_لمبائی(ctp->name);
		اگر (لڑی_موازنہ(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			توڑو;
		}
		ctp++;
	    }
	}

	انتخاب (mycmd) {
	حالت CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    واپس 0;
	حالت CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            واپس 0;
	حالت CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    واپس 0;
	حالت CMD_QUIT:
	    واپس 1;
	طےشدہ:
	    /* all the rest require a open database */
	    اگر (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		واپس 0;
	    }
	    انتخاب (mycmd) {
	    حالت CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, تہی);
		واپس 0;
	    حالت CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, تہی);
		واپس 0;
	    حالت CMD_داخل_کرو:
		bIterate = 0;
		داخل_کرو_tdb(arg1, arg1len,arg2,arg2len);
		واپس 0;
	    حالت CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		واپس 0;
	    حالت CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		واپس 0;
	    حالت CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		واپس 0;
	    حالت CMD_کلیدیں:
		tdb_traverse(tdb, print_کلید, تہی);
		واپس 0;
	    حالت CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, تہی);
		واپس 0;
	    حالت CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		واپس 0;
	    حالت CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		واپس 0;
	    حالت CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		واپس 0;
	    حالت CMD_INFO:
		info_tdb();
		واپس 0;
	    حالت CMD_پہلا:
		bIterate = 1;
		پہلا_ریکارڈ(tdb, &iterate_kbuf);
		واپس 0;
	    حالت CMD_اگلا:
	       اگر (bIterate)
		  اگلا_ریکارڈ(tdb, &iterate_kbuf);
		واپس 0;
	    حالت CMD_HELP:
		help();
		واپس 0;
            حالت CMD_CREATE_TDB:
            حالت CMD_OPEN_TDB:
            حالت CMD_SYSTEM:
            حالت CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                واپس 0;
	    }
	}

	واپس 0;
}

ساکن حرف *convert_string(حرف *instring, size_t *sizep)
{
    size_t لمبائی = 0;
    حرف *outp, *inp;
    حرف عارضی[3];


    outp = inp = instring;

    جب (*inp) {
	اگر (*inp == '\\') {
	    inp++;
	    اگر (*inp && strchr("0123456789abcdefABCDEF",(صحیح)*inp)) {
		عارضی[0] = *inp++;
		عارضی[1] = '\0';
		اگر (*inp && strchr("0123456789abcdefABCDEF",(صحیح)*inp)) {
		    عارضی[1] = *inp++;
		    عارضی[2] = '\0';
		}
		*outp++ = (حرف)strtol((ثابت حرف *)عارضی,تہی,16);
	    } ورنہ {
		*outp++ = *inp++;
	    }
	} ورنہ {
	    *outp++ = *inp++;
	}
	لمبائی++;
    }
    *sizep = لمبائی;
    واپس instring;
}

صحیح مین(صحیح argc, حرف *argv[])
{
    cmdname = "";
    arg1 = تہی;
    arg1len = 0;
    arg2 = تہی;
    arg2len = 0;

    اگر (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = تہی;
    }

    انتخاب (argc) {
	حالت 1:
	حالت 2:
	    /* Interactive mode */
	    جب ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = تہی;
		اگر ((arg1 = strchr((ثابت حرف *)cmdname,' ')) != تہی) {
		    arg1++;
		    arg2 = arg1;
		    جب (*arg2) {
			اگر (*arg2 == ' ') {
			    *arg2++ = '\0';
			    توڑو;
			}
			اگر ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		اگر (arg1) arg1 = convert_string(arg1,&arg1len);
		اگر (arg2) arg2 = convert_string(arg2,&arg2len);
		اگر (do_command()) توڑو;
	    }
	    توڑو;
	حالت 5:
	    arg2 = convert_string(argv[4],&arg2len);
	حالت 4:
	    arg1 = convert_string(argv[3],&arg1len);
	حالت 3:
	    cmdname = argv[2];
	طےشدہ:
	    do_command();
	    توڑو;
    }

    اگر (tdb) tdb_close(tdb);

    واپس 0;
}
