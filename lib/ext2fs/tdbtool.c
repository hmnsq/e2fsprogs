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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/mman.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <ctype.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <stdarg.h>

#অন্তর্ভাব "tdb.h"

স্থির পূর্ণাঙ্ক do_command(শূন্য);
ধ্রুব বর্ণ *cmdname;
বর্ণ *arg1, *arg2;
size_t arg1len, arg2len;
পূর্ণাঙ্ক bIterate = 0;
বর্ণ *সারি;
TDB_DATA iterate_kbuf;
বর্ণ cmdline[1024];

গণনা commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_ঢোকাও,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_চাবিগুলি,
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

প্রকারনির্দেশ সংরচনা {
	ধ্রুব বর্ণ *name;
	গণনা commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_ঢোকাও},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_চাবিগুলি},
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
	{রিক্ত,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

স্থির TDB_CONTEXT *tdb;

স্থির পূর্ণাঙ্ক print_rec(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state);
স্থির পূর্ণাঙ্ক print_চাবি(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state);
স্থির পূর্ণাঙ্ক print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state);

স্থির শূন্য print_asc(ধ্রুব বর্ণ *buf,পূর্ণাঙ্ক len)
{
	পূর্ণাঙ্ক i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	যদি (buf[len - 1] == 0)
	        len--;

	প্রতি (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

স্থির শূন্য print_data(ধ্রুব বর্ণ *buf,পূর্ণাঙ্ক len)
{
	পূর্ণাঙ্ক i=0;
	যদি (len<=0) প্রত্যাগম;
	printf("[%03X] ",i);
	প্রতি (i=0;i<len;) {
		printf("%02X ",(পূর্ণাঙ্ক)buf[i]);
		i++;
		যদি (i%8 == 0) printf(" ");
		যদি (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			যদি (i<len) printf("[%03X] ",i);
		}
	}
	যদি (i%16) {
		পূর্ণাঙ্ক n;

		n = 16 - (i%16);
		printf(" ");
		যদি (n>8) printf(" ");
		যাবত্ (n--) printf("   ");

		n = i%16;
		যদি (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		যদি (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

স্থির শূন্য help(শূন্য)
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

স্থির শূন্য terror(ধ্রুব বর্ণ *why)
{
	printf("%s\n", why);
}

স্থির শূন্য create_tdb(ধ্রুব বর্ণ *tdbname)
{
	যদি (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	যদি (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

স্থির শূন্য open_tdb(ধ্রুব বর্ণ *tdbname)
{
	যদি (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	যদি (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

স্থির শূন্য ঢোকাও_tdb(বর্ণ *keyname, size_t keylen, বর্ণ* data, size_t datalen)
{
	TDB_DATA চাবি, dbuf;

	যদি ((keyname == রিক্ত) || (keylen == 0)) {
		terror("need key");
		প্রত্যাগম;
	}

	চাবি.dptr = (unsigned বর্ণ *)keyname;
	চাবি.dsize = keylen;
	dbuf.dptr = (unsigned বর্ণ *)data;
	dbuf.dsize = datalen;

	যদি (tdb_store(tdb, চাবি, dbuf, TDB_ঢোকাও) == -1) {
		terror("insert failed");
	}
}

স্থির শূন্য store_tdb(বর্ণ *keyname, size_t keylen, বর্ণ* data, size_t datalen)
{
	TDB_DATA চাবি, dbuf;

	যদি ((keyname == রিক্ত) || (keylen == 0)) {
		terror("need key");
		প্রত্যাগম;
	}

	যদি ((data == রিক্ত) || (datalen == 0)) {
		terror("need data");
		প্রত্যাগম;
	}

	চাবি.dptr = (unsigned বর্ণ *)keyname;
	চাবি.dsize = keylen;
	dbuf.dptr = (unsigned বর্ণ *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, চাবি, dbuf, রিক্ত);

	যদি (tdb_store(tdb, চাবি, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

স্থির শূন্য show_tdb(বর্ণ *keyname, size_t keylen)
{
	TDB_DATA চাবি, dbuf;

	যদি ((keyname == রিক্ত) || (keylen == 0)) {
		terror("need key");
		প্রত্যাগম;
	}

	চাবি.dptr = (unsigned বর্ণ *)keyname;
	চাবি.dsize = keylen;

	dbuf = tdb_fetch(tdb, চাবি);
	যদি (!dbuf.dptr) {
	    terror("fetch failed");
	    প্রত্যাগম;
	}

	print_rec(tdb, চাবি, dbuf, রিক্ত);

	মুক্ত( dbuf.dptr );

	প্রত্যাগম;
}

স্থির শূন্য delete_tdb(বর্ণ *keyname, size_t keylen)
{
	TDB_DATA চাবি;

	যদি ((keyname == রিক্ত) || (keylen == 0)) {
		terror("need key");
		প্রত্যাগম;
	}

	চাবি.dptr = (unsigned বর্ণ *)keyname;
	চাবি.dsize = keylen;

	যদি (tdb_delete(tdb, চাবি) != 0) {
		terror("delete failed");
	}
}

স্থির শূন্য move_rec(বর্ণ *keyname, size_t keylen, বর্ণ* tdbname)
{
	TDB_DATA চাবি, dbuf;
	TDB_CONTEXT *dst_tdb;

	যদি ((keyname == রিক্ত) || (keylen == 0)) {
		terror("need key");
		প্রত্যাগম;
	}

	যদি ( !tdbname ) {
		terror("need destination tdb name");
		প্রত্যাগম;
	}

	চাবি.dptr = (unsigned বর্ণ *)keyname;
	চাবি.dsize = keylen;

	dbuf = tdb_fetch(tdb, চাবি);
	যদি (!dbuf.dptr) {
		terror("fetch failed");
		প্রত্যাগম;
	}

	print_rec(tdb, চাবি, dbuf, রিক্ত);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	যদি ( !dst_tdb ) {
		terror("unable to open destination tdb");
		প্রত্যাগম;
	}

	যদি ( tdb_store( dst_tdb, চাবি, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	অন্যথা
		printf("record moved\n");

	tdb_close( dst_tdb );

	প্রত্যাগম;
}

স্থির পূর্ণাঙ্ক print_rec(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state)
{
	printf("\nkey %d bytes\n", (পূর্ণাঙ্ক)চাবি.dsize);
	print_asc((ধ্রুব বর্ণ *)চাবি.dptr, চাবি.dsize);
	printf("\ndata %d bytes\n", (পূর্ণাঙ্ক)dbuf.dsize);
	print_data((ধ্রুব বর্ণ *)dbuf.dptr, dbuf.dsize);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক print_চাবি(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state)
{
	printf("key %d bytes: ", (পূর্ণাঙ্ক)চাবি.dsize);
	print_asc((ধ্রুব বর্ণ *)চাবি.dptr, চাবি.dsize);
	printf("\n");
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state)
{
	printf("key %d bytes\n", (পূর্ণাঙ্ক)চাবি.dsize);
	print_data((ধ্রুব বর্ণ *)চাবি.dptr, চাবি.dsize);
	printf("\n");
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক total_bytes;

স্থির পূর্ণাঙ্ক traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf, শূন্য *state)
{
	total_bytes += dbuf.dsize;
	প্রত্যাগম 0;
}

স্থির শূন্য info_tdb(শূন্য)
{
	পূর্ণাঙ্ক গুনতি;
	total_bytes = 0;
	যদি ((গুনতি = tdb_traverse(tdb, traverse_fn, রিক্ত)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	অন্যথা
		printf("%d records totalling %d bytes\n", গুনতি, total_bytes);
}

স্থির বর্ণ *tdb_getline(ধ্রুব বর্ণ *prompt)
{
	স্থির বর্ণ thisline[1024];
	বর্ণ *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, পরিমাণ(thisline)-1, stdin);
	যদি (p) p = strchr(p, '\n');
	যদি (p) *p = 0;
	প্রত্যাগম p?thisline:রিক্ত;
}

স্থির পূর্ণাঙ্ক do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA চাবি, TDB_DATA dbuf,
                     শূন্য *state)
{
    প্রত্যাগম tdb_delete(the_tdb, চাবি);
}

স্থির শূন্য first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	যদি (!dbuf.dptr) terror("fetch failed");
	অন্যথা {
		print_rec(the_tdb, *pkey, dbuf, রিক্ত);
	}
}

স্থির শূন্য next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	যদি (!dbuf.dptr)
		terror("fetch failed");
	অন্যথা
		print_rec(the_tdb, *pkey, dbuf, রিক্ত);
}

স্থির পূর্ণাঙ্ক do_command(শূন্য)
{
	COMMAND_TABLE *ctp = cmd_table;
	গণনা commands mycmd = CMD_HELP;
	পূর্ণাঙ্ক cmd_len;

	যদি (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} অন্যথা {
	    যাবত্ (ctp->name) {
		cmd_len = strlen(ctp->name);
		যদি (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			ভঙ্গ;
		}
		ctp++;
	    }
	}

	বিকল্প (mycmd) {
	পক্ষ CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    প্রত্যাগম 0;
	পক্ষ CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            প্রত্যাগম 0;
	পক্ষ CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    প্রত্যাগম 0;
	পক্ষ CMD_QUIT:
	    প্রত্যাগম 1;
	স্বভাব:
	    /* all the rest require a open database */
	    যদি (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		প্রত্যাগম 0;
	    }
	    বিকল্প (mycmd) {
	    পক্ষ CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, রিক্ত);
		প্রত্যাগম 0;
	    পক্ষ CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, রিক্ত);
		প্রত্যাগম 0;
	    পক্ষ CMD_ঢোকাও:
		bIterate = 0;
		ঢোকাও_tdb(arg1, arg1len,arg2,arg2len);
		প্রত্যাগম 0;
	    পক্ষ CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		প্রত্যাগম 0;
	    পক্ষ CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		প্রত্যাগম 0;
	    পক্ষ CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		প্রত্যাগম 0;
	    পক্ষ CMD_চাবিগুলি:
		tdb_traverse(tdb, print_চাবি, রিক্ত);
		প্রত্যাগম 0;
	    পক্ষ CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, রিক্ত);
		প্রত্যাগম 0;
	    পক্ষ CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		প্রত্যাগম 0;
	    পক্ষ CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		প্রত্যাগম 0;
	    পক্ষ CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		প্রত্যাগম 0;
	    পক্ষ CMD_INFO:
		info_tdb();
		প্রত্যাগম 0;
	    পক্ষ CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		প্রত্যাগম 0;
	    পক্ষ CMD_NEXT:
	       যদি (bIterate)
		  next_record(tdb, &iterate_kbuf);
		প্রত্যাগম 0;
	    পক্ষ CMD_HELP:
		help();
		প্রত্যাগম 0;
            পক্ষ CMD_CREATE_TDB:
            পক্ষ CMD_OPEN_TDB:
            পক্ষ CMD_SYSTEM:
            পক্ষ CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                প্রত্যাগম 0;
	    }
	}

	প্রত্যাগম 0;
}

স্থির বর্ণ *convert_string(বর্ণ *instring, size_t *sizep)
{
    size_t length = 0;
    বর্ণ *outp, *inp;
    বর্ণ temp[3];


    outp = inp = instring;

    যাবত্ (*inp) {
	যদি (*inp == '\\') {
	    inp++;
	    যদি (*inp && strchr("0123456789abcdefABCDEF",(পূর্ণাঙ্ক)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		যদি (*inp && strchr("0123456789abcdefABCDEF",(পূর্ণাঙ্ক)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (বর্ণ)strtol((ধ্রুব বর্ণ *)temp,রিক্ত,16);
	    } অন্যথা {
		*outp++ = *inp++;
	    }
	} অন্যথা {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    প্রত্যাগম instring;
}

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, বর্ণ *argv[])
{
    cmdname = "";
    arg1 = রিক্ত;
    arg1len = 0;
    arg2 = রিক্ত;
    arg2len = 0;

    যদি (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = রিক্ত;
    }

    বিকল্প (argc) {
	পক্ষ 1:
	পক্ষ 2:
	    /* Interactive mode */
	    যাবত্ ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = রিক্ত;
		যদি ((arg1 = strchr((ধ্রুব বর্ণ *)cmdname,' ')) != রিক্ত) {
		    arg1++;
		    arg2 = arg1;
		    যাবত্ (*arg2) {
			যদি (*arg2 == ' ') {
			    *arg2++ = '\0';
			    ভঙ্গ;
			}
			যদি ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		যদি (arg1) arg1 = convert_string(arg1,&arg1len);
		যদি (arg2) arg2 = convert_string(arg2,&arg2len);
		যদি (do_command()) ভঙ্গ;
	    }
	    ভঙ্গ;
	পক্ষ 5:
	    arg2 = convert_string(argv[4],&arg2len);
	পক্ষ 4:
	    arg1 = convert_string(argv[3],&arg1len);
	পক্ষ 3:
	    cmdname = argv[2];
	স্বভাব:
	    do_command();
	    ভঙ্গ;
    }

    যদি (tdb) tdb_close(tdb);

    প্রত্যাগম 0;
}
