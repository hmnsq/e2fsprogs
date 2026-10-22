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

ایستا صحیح do_command(تهی);
ثابت نویسه *cmdname;
نویسه *arg1, *arg2;
size_t arg1len, arg2len;
صحیح bIterate = 0;
نویسه *سطر;
TDB_DATA iterate_kbuf;
نویسه cmdline[1024];

شمارش commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_درج,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_کلیدها,
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

تعریفنوع ساختار {
	ثابت نویسه *name;
	شمارش commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_درج},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_کلیدها},
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
	{پوچ,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ایستا TDB_CONTEXT *tdb;

ایستا صحیح print_rec(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state);
ایستا صحیح print_کلید(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state);
ایستا صحیح print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state);

ایستا تهی print_asc(ثابت نویسه *buf,صحیح len)
{
	صحیح i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	اگر (buf[len - 1] == 0)
	        len--;

	برای (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

ایستا تهی print_data(ثابت نویسه *buf,صحیح len)
{
	صحیح i=0;
	اگر (len<=0) بازگشت;
	printf("[%03X] ",i);
	برای (i=0;i<len;) {
		printf("%02X ",(صحیح)buf[i]);
		i++;
		اگر (i%8 == 0) printf(" ");
		اگر (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			اگر (i<len) printf("[%03X] ",i);
		}
	}
	اگر (i%16) {
		صحیح n;

		n = 16 - (i%16);
		printf(" ");
		اگر (n>8) printf(" ");
		وقتی (n--) printf("   ");

		n = i%16;
		اگر (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		اگر (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

ایستا تهی help(تهی)
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

ایستا تهی terror(ثابت نویسه *why)
{
	printf("%s\n", why);
}

ایستا تهی create_tdb(ثابت نویسه *tdbname)
{
	اگر (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	اگر (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ایستا تهی open_tdb(ثابت نویسه *tdbname)
{
	اگر (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	اگر (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ایستا تهی درج_tdb(نویسه *keyname, size_t keylen, نویسه* data, size_t datalen)
{
	TDB_DATA کلید, dbuf;

	اگر ((keyname == پوچ) || (keylen == 0)) {
		terror("need key");
		بازگشت;
	}

	کلید.dptr = (unsigned نویسه *)keyname;
	کلید.dsize = keylen;
	dbuf.dptr = (unsigned نویسه *)data;
	dbuf.dsize = datalen;

	اگر (tdb_store(tdb, کلید, dbuf, TDB_درج) == -1) {
		terror("insert failed");
	}
}

ایستا تهی store_tdb(نویسه *keyname, size_t keylen, نویسه* data, size_t datalen)
{
	TDB_DATA کلید, dbuf;

	اگر ((keyname == پوچ) || (keylen == 0)) {
		terror("need key");
		بازگشت;
	}

	اگر ((data == پوچ) || (datalen == 0)) {
		terror("need data");
		بازگشت;
	}

	کلید.dptr = (unsigned نویسه *)keyname;
	کلید.dsize = keylen;
	dbuf.dptr = (unsigned نویسه *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, کلید, dbuf, پوچ);

	اگر (tdb_store(tdb, کلید, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ایستا تهی show_tdb(نویسه *keyname, size_t keylen)
{
	TDB_DATA کلید, dbuf;

	اگر ((keyname == پوچ) || (keylen == 0)) {
		terror("need key");
		بازگشت;
	}

	کلید.dptr = (unsigned نویسه *)keyname;
	کلید.dsize = keylen;

	dbuf = tdb_fetch(tdb, کلید);
	اگر (!dbuf.dptr) {
	    terror("fetch failed");
	    بازگشت;
	}

	print_rec(tdb, کلید, dbuf, پوچ);

	آزاد( dbuf.dptr );

	بازگشت;
}

ایستا تهی delete_tdb(نویسه *keyname, size_t keylen)
{
	TDB_DATA کلید;

	اگر ((keyname == پوچ) || (keylen == 0)) {
		terror("need key");
		بازگشت;
	}

	کلید.dptr = (unsigned نویسه *)keyname;
	کلید.dsize = keylen;

	اگر (tdb_delete(tdb, کلید) != 0) {
		terror("delete failed");
	}
}

ایستا تهی move_rec(نویسه *keyname, size_t keylen, نویسه* tdbname)
{
	TDB_DATA کلید, dbuf;
	TDB_CONTEXT *dst_tdb;

	اگر ((keyname == پوچ) || (keylen == 0)) {
		terror("need key");
		بازگشت;
	}

	اگر ( !tdbname ) {
		terror("need destination tdb name");
		بازگشت;
	}

	کلید.dptr = (unsigned نویسه *)keyname;
	کلید.dsize = keylen;

	dbuf = tdb_fetch(tdb, کلید);
	اگر (!dbuf.dptr) {
		terror("fetch failed");
		بازگشت;
	}

	print_rec(tdb, کلید, dbuf, پوچ);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	اگر ( !dst_tdb ) {
		terror("unable to open destination tdb");
		بازگشت;
	}

	اگر ( tdb_store( dst_tdb, کلید, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	وگرنه
		printf("record moved\n");

	tdb_close( dst_tdb );

	بازگشت;
}

ایستا صحیح print_rec(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state)
{
	printf("\nkey %d bytes\n", (صحیح)کلید.dsize);
	print_asc((ثابت نویسه *)کلید.dptr, کلید.dsize);
	printf("\ndata %d bytes\n", (صحیح)dbuf.dsize);
	print_data((ثابت نویسه *)dbuf.dptr, dbuf.dsize);
	بازگشت 0;
}

ایستا صحیح print_کلید(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state)
{
	printf("key %d bytes: ", (صحیح)کلید.dsize);
	print_asc((ثابت نویسه *)کلید.dptr, کلید.dsize);
	printf("\n");
	بازگشت 0;
}

ایستا صحیح print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state)
{
	printf("key %d bytes\n", (صحیح)کلید.dsize);
	print_data((ثابت نویسه *)کلید.dptr, کلید.dsize);
	printf("\n");
	بازگشت 0;
}

ایستا صحیح total_bytes;

ایستا صحیح traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf, تهی *state)
{
	total_bytes += dbuf.dsize;
	بازگشت 0;
}

ایستا تهی info_tdb(تهی)
{
	صحیح شمار;
	total_bytes = 0;
	اگر ((شمار = tdb_traverse(tdb, traverse_fn, پوچ)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	وگرنه
		printf("%d records totalling %d bytes\n", شمار, total_bytes);
}

ایستا نویسه *tdb_getline(ثابت نویسه *prompt)
{
	ایستا نویسه thisline[1024];
	نویسه *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, اندازه(thisline)-1, stdin);
	اگر (p) p = strchr(p, '\n');
	اگر (p) *p = 0;
	بازگشت p?thisline:پوچ;
}

ایستا صحیح do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA کلید, TDB_DATA dbuf,
                     تهی *state)
{
    بازگشت tdb_delete(the_tdb, کلید);
}

ایستا تهی first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	اگر (!dbuf.dptr) terror("fetch failed");
	وگرنه {
		print_rec(the_tdb, *pkey, dbuf, پوچ);
	}
}

ایستا تهی next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	اگر (!dbuf.dptr)
		terror("fetch failed");
	وگرنه
		print_rec(the_tdb, *pkey, dbuf, پوچ);
}

ایستا صحیح do_command(تهی)
{
	COMMAND_TABLE *ctp = cmd_table;
	شمارش commands mycmd = CMD_HELP;
	صحیح cmd_len;

	اگر (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} وگرنه {
	    وقتی (ctp->name) {
		cmd_len = strlen(ctp->name);
		اگر (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			بشکن;
		}
		ctp++;
	    }
	}

	انتخاب (mycmd) {
	حالت CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    بازگشت 0;
	حالت CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            بازگشت 0;
	حالت CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    بازگشت 0;
	حالت CMD_QUIT:
	    بازگشت 1;
	پیشفرض:
	    /* all the rest require a open database */
	    اگر (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		بازگشت 0;
	    }
	    انتخاب (mycmd) {
	    حالت CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, پوچ);
		بازگشت 0;
	    حالت CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, پوچ);
		بازگشت 0;
	    حالت CMD_درج:
		bIterate = 0;
		درج_tdb(arg1, arg1len,arg2,arg2len);
		بازگشت 0;
	    حالت CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		بازگشت 0;
	    حالت CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		بازگشت 0;
	    حالت CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		بازگشت 0;
	    حالت CMD_کلیدها:
		tdb_traverse(tdb, print_کلید, پوچ);
		بازگشت 0;
	    حالت CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, پوچ);
		بازگشت 0;
	    حالت CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		بازگشت 0;
	    حالت CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		بازگشت 0;
	    حالت CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		بازگشت 0;
	    حالت CMD_INFO:
		info_tdb();
		بازگشت 0;
	    حالت CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		بازگشت 0;
	    حالت CMD_NEXT:
	       اگر (bIterate)
		  next_record(tdb, &iterate_kbuf);
		بازگشت 0;
	    حالت CMD_HELP:
		help();
		بازگشت 0;
            حالت CMD_CREATE_TDB:
            حالت CMD_OPEN_TDB:
            حالت CMD_SYSTEM:
            حالت CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                بازگشت 0;
	    }
	}

	بازگشت 0;
}

ایستا نویسه *convert_string(نویسه *instring, size_t *sizep)
{
    size_t length = 0;
    نویسه *outp, *inp;
    نویسه temp[3];


    outp = inp = instring;

    وقتی (*inp) {
	اگر (*inp == '\\') {
	    inp++;
	    اگر (*inp && strchr("0123456789abcdefABCDEF",(صحیح)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		اگر (*inp && strchr("0123456789abcdefABCDEF",(صحیح)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (نویسه)strtol((ثابت نویسه *)temp,پوچ,16);
	    } وگرنه {
		*outp++ = *inp++;
	    }
	} وگرنه {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    بازگشت instring;
}

صحیح اصلی(صحیح argc, نویسه *argv[])
{
    cmdname = "";
    arg1 = پوچ;
    arg1len = 0;
    arg2 = پوچ;
    arg2len = 0;

    اگر (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = پوچ;
    }

    انتخاب (argc) {
	حالت 1:
	حالت 2:
	    /* Interactive mode */
	    وقتی ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = پوچ;
		اگر ((arg1 = strchr((ثابت نویسه *)cmdname,' ')) != پوچ) {
		    arg1++;
		    arg2 = arg1;
		    وقتی (*arg2) {
			اگر (*arg2 == ' ') {
			    *arg2++ = '\0';
			    بشکن;
			}
			اگر ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		اگر (arg1) arg1 = convert_string(arg1,&arg1len);
		اگر (arg2) arg2 = convert_string(arg2,&arg2len);
		اگر (do_command()) بشکن;
	    }
	    بشکن;
	حالت 5:
	    arg2 = convert_string(argv[4],&arg2len);
	حالت 4:
	    arg1 = convert_string(argv[3],&arg1len);
	حالت 3:
	    cmdname = argv[2];
	پیشفرض:
	    do_command();
	    بشکن;
    }

    اگر (tdb) tdb_close(tdb);

    بازگشت 0;
}
