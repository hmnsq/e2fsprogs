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
حرف *سٽ;
TDB_معلومات iterate_kbuf;
حرف cmdline[1024];

تعداد commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_وجهو,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_ڪنجيون,
	CMD_HEXKEYS,
	CMD_DELETE,
	CMD_LIST_HASH_FREE,
	CMD_LIST_FREE,
	CMD_INFO,
	CMD_پهريون,
	CMD_ايندڙ,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

تعريفنوع بنية {
	ثابت حرف *name;
	تعداد commands cmd;
} COMMAND_جدول;

COMMAND_جدول cmd_جدول[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_وجهو},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_ڪنجيون},
	{"hexkeys",	CMD_HEXKEYS},
	{"delete",	CMD_DELETE},
	{"list",	CMD_LIST_HASH_FREE},
	{"free",	CMD_LIST_FREE},
	{"info",	CMD_INFO},
	{"first",	CMD_پهريون},
	{"1",		CMD_پهريون},
	{"next",	CMD_ايندڙ},
	{"n",		CMD_ايندڙ},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{عدم,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ساكن TDB_CONTEXT *tdb;

ساكن صحيح print_rec(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state);
ساكن صحيح print_ڪنجي(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state);
ساكن صحيح print_hexkey(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state);

ساكن فراغ print_asc(ثابت حرف *buf,صحيح len)
{
	صحيح i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	إذا (buf[len - 1] == 0)
	        len--;

	لكل (i=0;i<len;i++)
		لکو("%c",isprint(buf[i])?buf[i]:'.');
}

ساكن فراغ print_معلومات(ثابت حرف *buf,صحيح len)
{
	صحيح i=0;
	إذا (len<=0) أرجع;
	لکو("[%03X] ",i);
	لكل (i=0;i<len;) {
		لکو("%02X ",(صحيح)buf[i]);
		i++;
		إذا (i%8 == 0) لکو(" ");
		إذا (i%16 == 0) {
			print_asc(&buf[i-16],8); لکو(" ");
			print_asc(&buf[i-8],8); لکو("\n");
			إذا (i<len) لکو("[%03X] ",i);
		}
	}
	إذا (i%16) {
		صحيح n;

		n = 16 - (i%16);
		لکو(" ");
		إذا (n>8) لکو(" ");
		طالما (n--) لکو("   ");

		n = i%16;
		إذا (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); لکو(" ");
		n = (i%16) - n;
		إذا (n>0) print_asc(&buf[i-n],n);
		لکو("\n");
	}
}

ساكن فراغ help(فراغ)
{
	لکو("\n"
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
	لکو("%s\n", why);
}

ساكن فراغ create_tdb(ثابت حرف *tdbname)
{
	إذا (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_پهريون,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	إذا (!tdb) {
		لکو("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ساكن فراغ open_tdb(ثابت حرف *tdbname)
{
	إذا (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	إذا (!tdb) {
		لکو("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ساكن فراغ وجهو_tdb(حرف *keyname, size_t keylen, حرف* معلومات, size_t datalen)
{
	TDB_معلومات ڪنجي, dbuf;

	إذا ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	ڪنجي.dptr = (بنانشان حرف *)keyname;
	ڪنجي.dsize = keylen;
	dbuf.dptr = (بنانشان حرف *)معلومات;
	dbuf.dsize = datalen;

	إذا (tdb_store(tdb, ڪنجي, dbuf, TDB_وجهو) == -1) {
		terror("insert failed");
	}
}

ساكن فراغ store_tdb(حرف *keyname, size_t keylen, حرف* معلومات, size_t datalen)
{
	TDB_معلومات ڪنجي, dbuf;

	إذا ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	إذا ((معلومات == عدم) || (datalen == 0)) {
		terror("need data");
		أرجع;
	}

	ڪنجي.dptr = (بنانشان حرف *)keyname;
	ڪنجي.dsize = keylen;
	dbuf.dptr = (بنانشان حرف *)معلومات;
	dbuf.dsize = datalen;

	لکو("Storing key:\n");
	print_rec(tdb, ڪنجي, dbuf, عدم);

	إذا (tdb_store(tdb, ڪنجي, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ساكن فراغ show_tdb(حرف *keyname, size_t keylen)
{
	TDB_معلومات ڪنجي, dbuf;

	إذا ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	ڪنجي.dptr = (بنانشان حرف *)keyname;
	ڪنجي.dsize = keylen;

	dbuf = tdb_fetch(tdb, ڪنجي);
	إذا (!dbuf.dptr) {
	    terror("fetch failed");
	    أرجع;
	}

	print_rec(tdb, ڪنجي, dbuf, عدم);

	حرر( dbuf.dptr );

	أرجع;
}

ساكن فراغ delete_tdb(حرف *keyname, size_t keylen)
{
	TDB_معلومات ڪنجي;

	إذا ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	ڪنجي.dptr = (بنانشان حرف *)keyname;
	ڪنجي.dsize = keylen;

	إذا (tdb_delete(tdb, ڪنجي) != 0) {
		terror("delete failed");
	}
}

ساكن فراغ move_rec(حرف *keyname, size_t keylen, حرف* tdbname)
{
	TDB_معلومات ڪنجي, dbuf;
	TDB_CONTEXT *dst_tdb;

	إذا ((keyname == عدم) || (keylen == 0)) {
		terror("need key");
		أرجع;
	}

	إذا ( !tdbname ) {
		terror("need destination tdb name");
		أرجع;
	}

	ڪنجي.dptr = (بنانشان حرف *)keyname;
	ڪنجي.dsize = keylen;

	dbuf = tdb_fetch(tdb, ڪنجي);
	إذا (!dbuf.dptr) {
		terror("fetch failed");
		أرجع;
	}

	print_rec(tdb, ڪنجي, dbuf, عدم);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	إذا ( !dst_tdb ) {
		terror("unable to open destination tdb");
		أرجع;
	}

	إذا ( tdb_store( dst_tdb, ڪنجي, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	وإلا
		لکو("record moved\n");

	tdb_close( dst_tdb );

	أرجع;
}

ساكن صحيح print_rec(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state)
{
	لکو("\nkey %d bytes\n", (صحيح)ڪنجي.dsize);
	print_asc((ثابت حرف *)ڪنجي.dptr, ڪنجي.dsize);
	لکو("\ndata %d bytes\n", (صحيح)dbuf.dsize);
	print_معلومات((ثابت حرف *)dbuf.dptr, dbuf.dsize);
	أرجع 0;
}

ساكن صحيح print_ڪنجي(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state)
{
	لکو("key %d bytes: ", (صحيح)ڪنجي.dsize);
	print_asc((ثابت حرف *)ڪنجي.dptr, ڪنجي.dsize);
	لکو("\n");
	أرجع 0;
}

ساكن صحيح print_hexkey(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state)
{
	لکو("key %d bytes\n", (صحيح)ڪنجي.dsize);
	print_معلومات((ثابت حرف *)ڪنجي.dptr, ڪنجي.dsize);
	لکو("\n");
	أرجع 0;
}

ساكن صحيح ڪل_بائيٽون;

ساكن صحيح traverse_fn(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf, فراغ *state)
{
	ڪل_بائيٽون += dbuf.dsize;
	أرجع 0;
}

ساكن فراغ info_tdb(فراغ)
{
	صحيح ڳڻپ;
	ڪل_بائيٽون = 0;
	إذا ((ڳڻپ = tdb_traverse(tdb, traverse_fn, عدم)) == -1)
		لکو("Error = %s\n", tdb_errorstr(tdb));
	وإلا
		لکو("%d records totalling %d bytes\n", ڳڻپ, ڪل_بائيٽون);
}

ساكن حرف *tdb_getline(ثابت حرف *prompt)
{
	ساكن حرف thisline[1024];
	حرف *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, حجم(thisline)-1, stdin);
	إذا (p) p = strchr(p, '\n');
	إذا (p) *p = 0;
	أرجع p?thisline:عدم;
}

ساكن صحيح do_delete_fn(TDB_CONTEXT *the_tdb, TDB_معلومات ڪنجي, TDB_معلومات dbuf,
                     فراغ *state)
{
    أرجع tdb_delete(the_tdb, ڪنجي);
}

ساكن فراغ پهريون_رڪارڊ(TDB_CONTEXT *the_tdb, TDB_معلومات *pkey)
{
	TDB_معلومات dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	إذا (!dbuf.dptr) terror("fetch failed");
	وإلا {
		print_rec(the_tdb, *pkey, dbuf, عدم);
	}
}

ساكن فراغ ايندڙ_رڪارڊ(TDB_CONTEXT *the_tdb, TDB_معلومات *pkey)
{
	TDB_معلومات dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	إذا (!dbuf.dptr)
		terror("fetch failed");
	وإلا
		print_rec(the_tdb, *pkey, dbuf, عدم);
}

ساكن صحيح do_command(فراغ)
{
	COMMAND_جدول *ctp = cmd_جدول;
	تعداد commands mycmd = CMD_HELP;
	صحيح cmd_len;

	إذا (cmdname && لڙي_ڊيگهه(cmdname) == 0) {
	    mycmd = CMD_ايندڙ;
	} وإلا {
	    طالما (ctp->name) {
		cmd_len = لڙي_ڊيگهه(ctp->name);
		إذا (لڙي_ڀيٽ(ctp->name,cmdname,cmd_len) == 0) {
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
	    إذا (!tdb) {
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
	    حالة CMD_وجهو:
		bIterate = 0;
		وجهو_tdb(arg1, arg1len,arg2,arg2len);
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
	    حالة CMD_ڪنجيون:
		tdb_traverse(tdb, print_ڪنجي, عدم);
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
	    حالة CMD_پهريون:
		bIterate = 1;
		پهريون_رڪارڊ(tdb, &iterate_kbuf);
		أرجع 0;
	    حالة CMD_ايندڙ:
	       إذا (bIterate)
		  ايندڙ_رڪارڊ(tdb, &iterate_kbuf);
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
    size_t ڊيگهائي = 0;
    حرف *outp, *inp;
    حرف عارضي[3];


    outp = inp = instring;

    طالما (*inp) {
	إذا (*inp == '\\') {
	    inp++;
	    إذا (*inp && strchr("0123456789abcdefABCDEF",(صحيح)*inp)) {
		عارضي[0] = *inp++;
		عارضي[1] = '\0';
		إذا (*inp && strchr("0123456789abcdefABCDEF",(صحيح)*inp)) {
		    عارضي[1] = *inp++;
		    عارضي[2] = '\0';
		}
		*outp++ = (حرف)strtol((ثابت حرف *)عارضي,عدم,16);
	    } وإلا {
		*outp++ = *inp++;
	    }
	} وإلا {
	    *outp++ = *inp++;
	}
	ڊيگهائي++;
    }
    *sizep = ڊيگهائي;
    أرجع instring;
}

صحيح مکيه(صحيح argc, حرف *argv[])
{
    cmdname = "";
    arg1 = عدم;
    arg1len = 0;
    arg2 = عدم;
    arg2len = 0;

    إذا (argv[1]) {
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
		إذا ((arg1 = strchr((ثابت حرف *)cmdname,' ')) != عدم) {
		    arg1++;
		    arg2 = arg1;
		    طالما (*arg2) {
			إذا (*arg2 == ' ') {
			    *arg2++ = '\0';
			    اكسر;
			}
			إذا ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		إذا (arg1) arg1 = convert_string(arg1,&arg1len);
		إذا (arg2) arg2 = convert_string(arg2,&arg2len);
		إذا (do_command()) اكسر;
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

    إذا (tdb) tdb_close(tdb);

    أرجع 0;
}
