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

স্থির পূর্ণসংখ্যা do_command(শূন্য);
ধ্রুবক অক্ষর *cmdname;
অক্ষর *arg1, *arg2;
size_t arg1len, arg2len;
পূর্ণসংখ্যা bIterate = 0;
অক্ষর *সারি;
TDB_তথ্য iterate_kbuf;
অক্ষর cmdline[1024];

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
	CMD_পরবর্তী,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

প্রকারলেখ কাঠামো {
	ধ্রুবক অক্ষর *name;
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
	{"next",	CMD_পরবর্তী},
	{"n",		CMD_পরবর্তী},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{ফাঁকা,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

স্থির TDB_CONTEXT *tdb;

স্থির পূর্ণসংখ্যা print_rec(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state);
স্থির পূর্ণসংখ্যা print_চাবি(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state);
স্থির পূর্ণসংখ্যা print_hexkey(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state);

স্থির শূন্য print_asc(ধ্রুবক অক্ষর *buf,পূর্ণসংখ্যা len)
{
	পূর্ণসংখ্যা i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	যদি (buf[len - 1] == 0)
	        len--;

	জন্য (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

স্থির শূন্য print_তথ্য(ধ্রুবক অক্ষর *buf,পূর্ণসংখ্যা len)
{
	পূর্ণসংখ্যা i=0;
	যদি (len<=0) ফেরত;
	printf("[%03X] ",i);
	জন্য (i=0;i<len;) {
		printf("%02X ",(পূর্ণসংখ্যা)buf[i]);
		i++;
		যদি (i%8 == 0) printf(" ");
		যদি (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			যদি (i<len) printf("[%03X] ",i);
		}
	}
	যদি (i%16) {
		পূর্ণসংখ্যা n;

		n = 16 - (i%16);
		printf(" ");
		যদি (n>8) printf(" ");
		যখন (n--) printf("   ");

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

স্থির শূন্য terror(ধ্রুবক অক্ষর *why)
{
	printf("%s\n", why);
}

স্থির শূন্য create_tdb(ধ্রুবক অক্ষর *tdbname)
{
	যদি (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	যদি (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

স্থির শূন্য open_tdb(ধ্রুবক অক্ষর *tdbname)
{
	যদি (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	যদি (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

স্থির শূন্য ঢোকাও_tdb(অক্ষর *keyname, size_t keylen, অক্ষর* তথ্য, size_t datalen)
{
	TDB_তথ্য চাবি, dbuf;

	যদি ((keyname == ফাঁকা) || (keylen == 0)) {
		terror("need key");
		ফেরত;
	}

	চাবি.dptr = (unsigned অক্ষর *)keyname;
	চাবি.dsize = keylen;
	dbuf.dptr = (unsigned অক্ষর *)তথ্য;
	dbuf.dsize = datalen;

	যদি (tdb_store(tdb, চাবি, dbuf, TDB_ঢোকাও) == -1) {
		terror("insert failed");
	}
}

স্থির শূন্য store_tdb(অক্ষর *keyname, size_t keylen, অক্ষর* তথ্য, size_t datalen)
{
	TDB_তথ্য চাবি, dbuf;

	যদি ((keyname == ফাঁকা) || (keylen == 0)) {
		terror("need key");
		ফেরত;
	}

	যদি ((তথ্য == ফাঁকা) || (datalen == 0)) {
		terror("need data");
		ফেরত;
	}

	চাবি.dptr = (unsigned অক্ষর *)keyname;
	চাবি.dsize = keylen;
	dbuf.dptr = (unsigned অক্ষর *)তথ্য;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, চাবি, dbuf, ফাঁকা);

	যদি (tdb_store(tdb, চাবি, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

স্থির শূন্য show_tdb(অক্ষর *keyname, size_t keylen)
{
	TDB_তথ্য চাবি, dbuf;

	যদি ((keyname == ফাঁকা) || (keylen == 0)) {
		terror("need key");
		ফেরত;
	}

	চাবি.dptr = (unsigned অক্ষর *)keyname;
	চাবি.dsize = keylen;

	dbuf = tdb_fetch(tdb, চাবি);
	যদি (!dbuf.dptr) {
	    terror("fetch failed");
	    ফেরত;
	}

	print_rec(tdb, চাবি, dbuf, ফাঁকা);

	মুক্ত( dbuf.dptr );

	ফেরত;
}

স্থির শূন্য delete_tdb(অক্ষর *keyname, size_t keylen)
{
	TDB_তথ্য চাবি;

	যদি ((keyname == ফাঁকা) || (keylen == 0)) {
		terror("need key");
		ফেরত;
	}

	চাবি.dptr = (unsigned অক্ষর *)keyname;
	চাবি.dsize = keylen;

	যদি (tdb_delete(tdb, চাবি) != 0) {
		terror("delete failed");
	}
}

স্থির শূন্য move_rec(অক্ষর *keyname, size_t keylen, অক্ষর* tdbname)
{
	TDB_তথ্য চাবি, dbuf;
	TDB_CONTEXT *dst_tdb;

	যদি ((keyname == ফাঁকা) || (keylen == 0)) {
		terror("need key");
		ফেরত;
	}

	যদি ( !tdbname ) {
		terror("need destination tdb name");
		ফেরত;
	}

	চাবি.dptr = (unsigned অক্ষর *)keyname;
	চাবি.dsize = keylen;

	dbuf = tdb_fetch(tdb, চাবি);
	যদি (!dbuf.dptr) {
		terror("fetch failed");
		ফেরত;
	}

	print_rec(tdb, চাবি, dbuf, ফাঁকা);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	যদি ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ফেরত;
	}

	যদি ( tdb_store( dst_tdb, চাবি, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	অন্যথা
		printf("record moved\n");

	tdb_close( dst_tdb );

	ফেরত;
}

স্থির পূর্ণসংখ্যা print_rec(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state)
{
	printf("\nkey %d bytes\n", (পূর্ণসংখ্যা)চাবি.dsize);
	print_asc((ধ্রুবক অক্ষর *)চাবি.dptr, চাবি.dsize);
	printf("\ndata %d bytes\n", (পূর্ণসংখ্যা)dbuf.dsize);
	print_তথ্য((ধ্রুবক অক্ষর *)dbuf.dptr, dbuf.dsize);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা print_চাবি(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state)
{
	printf("key %d bytes: ", (পূর্ণসংখ্যা)চাবি.dsize);
	print_asc((ধ্রুবক অক্ষর *)চাবি.dptr, চাবি.dsize);
	printf("\n");
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা print_hexkey(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state)
{
	printf("key %d bytes\n", (পূর্ণসংখ্যা)চাবি.dsize);
	print_তথ্য((ধ্রুবক অক্ষর *)চাবি.dptr, চাবি.dsize);
	printf("\n");
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা যোগ_bytes;

স্থির পূর্ণসংখ্যা traverse_fn(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf, শূন্য *state)
{
	যোগ_bytes += dbuf.dsize;
	ফেরত 0;
}

স্থির শূন্য info_tdb(শূন্য)
{
	পূর্ণসংখ্যা গুনতি;
	যোগ_bytes = 0;
	যদি ((গুনতি = tdb_traverse(tdb, traverse_fn, ফাঁকা)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	অন্যথা
		printf("%d records totalling %d bytes\n", গুনতি, যোগ_bytes);
}

স্থির অক্ষর *tdb_getline(ধ্রুবক অক্ষর *prompt)
{
	স্থির অক্ষর thisline[1024];
	অক্ষর *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, আকার(thisline)-1, stdin);
	যদি (p) p = strchr(p, '\n');
	যদি (p) *p = 0;
	ফেরত p?thisline:ফাঁকা;
}

স্থির পূর্ণসংখ্যা do_delete_fn(TDB_CONTEXT *the_tdb, TDB_তথ্য চাবি, TDB_তথ্য dbuf,
                     শূন্য *state)
{
    ফেরত tdb_delete(the_tdb, চাবি);
}

স্থির শূন্য first_record(TDB_CONTEXT *the_tdb, TDB_তথ্য *pkey)
{
	TDB_তথ্য dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	যদি (!dbuf.dptr) terror("fetch failed");
	অন্যথা {
		print_rec(the_tdb, *pkey, dbuf, ফাঁকা);
	}
}

স্থির শূন্য পরবর্তী_record(TDB_CONTEXT *the_tdb, TDB_তথ্য *pkey)
{
	TDB_তথ্য dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	যদি (!dbuf.dptr)
		terror("fetch failed");
	অন্যথা
		print_rec(the_tdb, *pkey, dbuf, ফাঁকা);
}

স্থির পূর্ণসংখ্যা do_command(শূন্য)
{
	COMMAND_TABLE *ctp = cmd_table;
	গণনা commands mycmd = CMD_HELP;
	পূর্ণসংখ্যা cmd_len;

	যদি (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_পরবর্তী;
	} অন্যথা {
	    যখন (ctp->name) {
		cmd_len = strlen(ctp->name);
		যদি (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			বিরতি;
		}
		ctp++;
	    }
	}

	নির্বাচন (mycmd) {
	ক্ষেত্র CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    ফেরত 0;
	ক্ষেত্র CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ফেরত 0;
	ক্ষেত্র CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ফেরত 0;
	ক্ষেত্র CMD_QUIT:
	    ফেরত 1;
	পূর্বনির্ধারিত:
	    /* all the rest require a open database */
	    যদি (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ফেরত 0;
	    }
	    নির্বাচন (mycmd) {
	    ক্ষেত্র CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ফাঁকা);
		ফেরত 0;
	    ক্ষেত্র CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ফাঁকা);
		ফেরত 0;
	    ক্ষেত্র CMD_ঢোকাও:
		bIterate = 0;
		ঢোকাও_tdb(arg1, arg1len,arg2,arg2len);
		ফেরত 0;
	    ক্ষেত্র CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ফেরত 0;
	    ক্ষেত্র CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ফেরত 0;
	    ক্ষেত্র CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ফেরত 0;
	    ক্ষেত্র CMD_চাবিগুলি:
		tdb_traverse(tdb, print_চাবি, ফাঁকা);
		ফেরত 0;
	    ক্ষেত্র CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ফাঁকা);
		ফেরত 0;
	    ক্ষেত্র CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ফেরত 0;
	    ক্ষেত্র CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ফেরত 0;
	    ক্ষেত্র CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ফেরত 0;
	    ক্ষেত্র CMD_INFO:
		info_tdb();
		ফেরত 0;
	    ক্ষেত্র CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		ফেরত 0;
	    ক্ষেত্র CMD_পরবর্তী:
	       যদি (bIterate)
		  পরবর্তী_record(tdb, &iterate_kbuf);
		ফেরত 0;
	    ক্ষেত্র CMD_HELP:
		help();
		ফেরত 0;
            ক্ষেত্র CMD_CREATE_TDB:
            ক্ষেত্র CMD_OPEN_TDB:
            ক্ষেত্র CMD_SYSTEM:
            ক্ষেত্র CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ফেরত 0;
	    }
	}

	ফেরত 0;
}

স্থির অক্ষর *convert_string(অক্ষর *instring, size_t *sizep)
{
    size_t দৈর্ঘ্য = 0;
    অক্ষর *outp, *inp;
    অক্ষর অস্থায়ী[3];


    outp = inp = instring;

    যখন (*inp) {
	যদি (*inp == '\\') {
	    inp++;
	    যদি (*inp && strchr("0123456789abcdefABCDEF",(পূর্ণসংখ্যা)*inp)) {
		অস্থায়ী[0] = *inp++;
		অস্থায়ী[1] = '\0';
		যদি (*inp && strchr("0123456789abcdefABCDEF",(পূর্ণসংখ্যা)*inp)) {
		    অস্থায়ী[1] = *inp++;
		    অস্থায়ী[2] = '\0';
		}
		*outp++ = (অক্ষর)strtol((ধ্রুবক অক্ষর *)অস্থায়ী,ফাঁকা,16);
	    } অন্যথা {
		*outp++ = *inp++;
	    }
	} অন্যথা {
	    *outp++ = *inp++;
	}
	দৈর্ঘ্য++;
    }
    *sizep = দৈর্ঘ্য;
    ফেরত instring;
}

পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর *argv[])
{
    cmdname = "";
    arg1 = ফাঁকা;
    arg1len = 0;
    arg2 = ফাঁকা;
    arg2len = 0;

    যদি (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ফাঁকা;
    }

    নির্বাচন (argc) {
	ক্ষেত্র 1:
	ক্ষেত্র 2:
	    /* Interactive mode */
	    যখন ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ফাঁকা;
		যদি ((arg1 = strchr((ধ্রুবক অক্ষর *)cmdname,' ')) != ফাঁকা) {
		    arg1++;
		    arg2 = arg1;
		    যখন (*arg2) {
			যদি (*arg2 == ' ') {
			    *arg2++ = '\0';
			    বিরতি;
			}
			যদি ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		যদি (arg1) arg1 = convert_string(arg1,&arg1len);
		যদি (arg2) arg2 = convert_string(arg2,&arg2len);
		যদি (do_command()) বিরতি;
	    }
	    বিরতি;
	ক্ষেত্র 5:
	    arg2 = convert_string(argv[4],&arg2len);
	ক্ষেত্র 4:
	    arg1 = convert_string(argv[3],&arg1len);
	ক্ষেত্র 3:
	    cmdname = argv[2];
	পূর্বনির্ধারিত:
	    do_command();
	    বিরতি;
    }

    যদি (tdb) tdb_close(tdb);

    ফেরত 0;
}
