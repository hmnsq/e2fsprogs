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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/mman.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <ctype.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>

#ଅନ୍ତର୍ଭାଵ "tdb.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_command(ଶୂନ୍ୟ);
ଅଚଳ ଅକ୍ଷର *cmdname;
ଅକ୍ଷର *arg1, *arg2;
size_t arg1len, arg2len;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା bIterate = 0;
ଅକ୍ଷର *ଧାଡ଼ି;
TDB_DATA iterate_kbuf;
ଅକ୍ଷର cmdline[1024];

ଗଣନା commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_ଭର୍ତ୍ତି,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_ଚାବିଗୁଡ଼ିକ,
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

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
	ଅଚଳ ଅକ୍ଷର *name;
	ଗଣନା commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_ଭର୍ତ୍ତି},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_ଚାବିଗୁଡ଼ିକ},
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
	{ଖାଲି,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

ସ୍ଥିର TDB_CONTEXT *tdb;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_rec(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_ଚାବି(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state);

ସ୍ଥିର ଶୂନ୍ୟ print_asc(ଅଚଳ ଅକ୍ଷର *buf,ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	ଯଦି (buf[len - 1] == 0)
	        len--;

	ପାଇଁ (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

ସ୍ଥିର ଶୂନ୍ୟ print_data(ଅଚଳ ଅକ୍ଷର *buf,ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;
	ଯଦି (len<=0) ଫେରନ୍ତୁ;
	printf("[%03X] ",i);
	ପାଇଁ (i=0;i<len;) {
		printf("%02X ",(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)buf[i]);
		i++;
		ଯଦି (i%8 == 0) printf(" ");
		ଯଦି (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			ଯଦି (i<len) printf("[%03X] ",i);
		}
	}
	ଯଦି (i%16) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

		n = 16 - (i%16);
		printf(" ");
		ଯଦି (n>8) printf(" ");
		ଯେତେବେଳେ (n--) printf("   ");

		n = i%16;
		ଯଦି (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		ଯଦି (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ help(ଶୂନ୍ୟ)
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

ସ୍ଥିର ଶୂନ୍ୟ terror(ଅଚଳ ଅକ୍ଷର *why)
{
	printf("%s\n", why);
}

ସ୍ଥିର ଶୂନ୍ୟ create_tdb(ଅଚଳ ଅକ୍ଷର *tdbname)
{
	ଯଦି (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	ଯଦି (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

ସ୍ଥିର ଶୂନ୍ୟ open_tdb(ଅଚଳ ଅକ୍ଷର *tdbname)
{
	ଯଦି (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ଯଦି (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_tdb(ଅକ୍ଷର *keyname, size_t keylen, ଅକ୍ଷର* data, size_t datalen)
{
	TDB_DATA ଚାବି, dbuf;

	ଯଦି ((keyname == ଖାଲି) || (keylen == 0)) {
		terror("need key");
		ଫେରନ୍ତୁ;
	}

	ଚାବି.dptr = (unsigned ଅକ୍ଷର *)keyname;
	ଚାବି.dsize = keylen;
	dbuf.dptr = (unsigned ଅକ୍ଷର *)data;
	dbuf.dsize = datalen;

	ଯଦି (tdb_store(tdb, ଚାବି, dbuf, TDB_ଭର୍ତ୍ତି) == -1) {
		terror("insert failed");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ store_tdb(ଅକ୍ଷର *keyname, size_t keylen, ଅକ୍ଷର* data, size_t datalen)
{
	TDB_DATA ଚାବି, dbuf;

	ଯଦି ((keyname == ଖାଲି) || (keylen == 0)) {
		terror("need key");
		ଫେରନ୍ତୁ;
	}

	ଯଦି ((data == ଖାଲି) || (datalen == 0)) {
		terror("need data");
		ଫେରନ୍ତୁ;
	}

	ଚାବି.dptr = (unsigned ଅକ୍ଷର *)keyname;
	ଚାବି.dsize = keylen;
	dbuf.dptr = (unsigned ଅକ୍ଷର *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, ଚାବି, dbuf, ଖାଲି);

	ଯଦି (tdb_store(tdb, ଚାବି, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ show_tdb(ଅକ୍ଷର *keyname, size_t keylen)
{
	TDB_DATA ଚାବି, dbuf;

	ଯଦି ((keyname == ଖାଲି) || (keylen == 0)) {
		terror("need key");
		ଫେରନ୍ତୁ;
	}

	ଚାବି.dptr = (unsigned ଅକ୍ଷର *)keyname;
	ଚାବି.dsize = keylen;

	dbuf = tdb_fetch(tdb, ଚାବି);
	ଯଦି (!dbuf.dptr) {
	    terror("fetch failed");
	    ଫେରନ୍ତୁ;
	}

	print_rec(tdb, ଚାବି, dbuf, ଖାଲି);

	ମୁକ୍ତ( dbuf.dptr );

	ଫେରନ୍ତୁ;
}

ସ୍ଥିର ଶୂନ୍ୟ delete_tdb(ଅକ୍ଷର *keyname, size_t keylen)
{
	TDB_DATA ଚାବି;

	ଯଦି ((keyname == ଖାଲି) || (keylen == 0)) {
		terror("need key");
		ଫେରନ୍ତୁ;
	}

	ଚାବି.dptr = (unsigned ଅକ୍ଷର *)keyname;
	ଚାବି.dsize = keylen;

	ଯଦି (tdb_delete(tdb, ଚାବି) != 0) {
		terror("delete failed");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ move_rec(ଅକ୍ଷର *keyname, size_t keylen, ଅକ୍ଷର* tdbname)
{
	TDB_DATA ଚାବି, dbuf;
	TDB_CONTEXT *dst_tdb;

	ଯଦି ((keyname == ଖାଲି) || (keylen == 0)) {
		terror("need key");
		ଫେରନ୍ତୁ;
	}

	ଯଦି ( !tdbname ) {
		terror("need destination tdb name");
		ଫେରନ୍ତୁ;
	}

	ଚାବି.dptr = (unsigned ଅକ୍ଷର *)keyname;
	ଚାବି.dsize = keylen;

	dbuf = tdb_fetch(tdb, ଚାବି);
	ଯଦି (!dbuf.dptr) {
		terror("fetch failed");
		ଫେରନ୍ତୁ;
	}

	print_rec(tdb, ଚାବି, dbuf, ଖାଲି);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ଯଦି ( !dst_tdb ) {
		terror("unable to open destination tdb");
		ଫେରନ୍ତୁ;
	}

	ଯଦି ( tdb_store( dst_tdb, ଚାବି, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	ଅନ୍ୟଥା
		printf("record moved\n");

	tdb_close( dst_tdb );

	ଫେରନ୍ତୁ;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_rec(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state)
{
	printf("\nkey %d bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଚାବି.dsize);
	print_asc((ଅଚଳ ଅକ୍ଷର *)ଚାବି.dptr, ଚାବି.dsize);
	printf("\ndata %d bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dbuf.dsize);
	print_data((ଅଚଳ ଅକ୍ଷର *)dbuf.dptr, dbuf.dsize);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_ଚାବି(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state)
{
	printf("key %d bytes: ", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଚାବି.dsize);
	print_asc((ଅଚଳ ଅକ୍ଷର *)ଚାବି.dptr, ଚାବି.dsize);
	printf("\n");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state)
{
	printf("key %d bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଚାବି.dsize);
	print_data((ଅଚଳ ଅକ୍ଷର *)ଚାବି.dptr, ଚାବି.dsize);
	printf("\n");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_bytes;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf, ଶୂନ୍ୟ *state)
{
	total_bytes += dbuf.dsize;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ info_tdb(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;
	total_bytes = 0;
	ଯଦି ((ଗଣନା_2 = tdb_traverse(tdb, traverse_fn, ଖାଲି)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	ଅନ୍ୟଥା
		printf("%d records totalling %d bytes\n", ଗଣନା_2, total_bytes);
}

ସ୍ଥିର ଅକ୍ଷର *tdb_getline(ଅଚଳ ଅକ୍ଷର *prompt)
{
	ସ୍ଥିର ଅକ୍ଷର thisline[1024];
	ଅକ୍ଷର *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, ଆକାର(thisline)-1, stdin);
	ଯଦି (p) p = strchr(p, '\n');
	ଯଦି (p) *p = 0;
	ଫେରନ୍ତୁ p?thisline:ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA ଚାବି, TDB_DATA dbuf,
                     ଶୂନ୍ୟ *state)
{
    ଫେରନ୍ତୁ tdb_delete(the_tdb, ଚାବି);
}

ସ୍ଥିର ଶୂନ୍ୟ first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ଯଦି (!dbuf.dptr) terror("fetch failed");
	ଅନ୍ୟଥା {
		print_rec(the_tdb, *pkey, dbuf, ଖାଲି);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ଯଦି (!dbuf.dptr)
		terror("fetch failed");
	ଅନ୍ୟଥା
		print_rec(the_tdb, *pkey, dbuf, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_command(ଶୂନ୍ୟ)
{
	COMMAND_TABLE *ctp = cmd_table;
	ଗଣନା commands mycmd = CMD_HELP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_len;

	ଯଦି (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} ଅନ୍ୟଥା {
	    ଯେତେବେଳେ (ctp->name) {
		cmd_len = strlen(ctp->name);
		ଯଦି (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			ଭାଙ୍ଗ;
		}
		ctp++;
	    }
	}

	ବାଛ (mycmd) {
	ସ୍ଥିତି CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି CMD_QUIT:
	    ଫେରନ୍ତୁ 1;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	    /* all the rest require a open database */
	    ଯଦି (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		ଫେରନ୍ତୁ 0;
	    }
	    ବାଛ (mycmd) {
	    ସ୍ଥିତି CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ଖାଲି);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ଖାଲି);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_ଭର୍ତ୍ତି:
		bIterate = 0;
		ଭର୍ତ୍ତି_tdb(arg1, arg1len,arg2,arg2len);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_ଚାବିଗୁଡ଼ିକ:
		tdb_traverse(tdb, print_ଚାବି, ଖାଲି);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ଖାଲି);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_INFO:
		info_tdb();
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_NEXT:
	       ଯଦି (bIterate)
		  next_record(tdb, &iterate_kbuf);
		ଫେରନ୍ତୁ 0;
	    ସ୍ଥିତି CMD_HELP:
		help();
		ଫେରନ୍ତୁ 0;
            ସ୍ଥିତି CMD_CREATE_TDB:
            ସ୍ଥିତି CMD_OPEN_TDB:
            ସ୍ଥିତି CMD_SYSTEM:
            ସ୍ଥିତି CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                ଫେରନ୍ତୁ 0;
	    }
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅକ୍ଷର *convert_string(ଅକ୍ଷର *instring, size_t *sizep)
{
    size_t length = 0;
    ଅକ୍ଷର *outp, *inp;
    ଅକ୍ଷର temp[3];


    outp = inp = instring;

    ଯେତେବେଳେ (*inp) {
	ଯଦି (*inp == '\\') {
	    inp++;
	    ଯଦି (*inp && strchr("0123456789abcdefABCDEF",(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		ଯଦି (*inp && strchr("0123456789abcdefABCDEF",(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (ଅକ୍ଷର)strtol((ଅଚଳ ଅକ୍ଷର *)temp,ଖାଲି,16);
	    } ଅନ୍ୟଥା {
		*outp++ = *inp++;
	    }
	} ଅନ୍ୟଥା {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    ଫେରନ୍ତୁ instring;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
    cmdname = "";
    arg1 = ଖାଲି;
    arg1len = 0;
    arg2 = ଖାଲି;
    arg2len = 0;

    ଯଦି (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ଖାଲି;
    }

    ବାଛ (argc) {
	ସ୍ଥିତି 1:
	ସ୍ଥିତି 2:
	    /* Interactive mode */
	    ଯେତେବେଳେ ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ଖାଲି;
		ଯଦି ((arg1 = strchr((ଅଚଳ ଅକ୍ଷର *)cmdname,' ')) != ଖାଲି) {
		    arg1++;
		    arg2 = arg1;
		    ଯେତେବେଳେ (*arg2) {
			ଯଦି (*arg2 == ' ') {
			    *arg2++ = '\0';
			    ଭାଙ୍ଗ;
			}
			ଯଦି ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		ଯଦି (arg1) arg1 = convert_string(arg1,&arg1len);
		ଯଦି (arg2) arg2 = convert_string(arg2,&arg2len);
		ଯଦି (do_command()) ଭାଙ୍ଗ;
	    }
	    ଭାଙ୍ଗ;
	ସ୍ଥିତି 5:
	    arg2 = convert_string(argv[4],&arg2len);
	ସ୍ଥିତି 4:
	    arg1 = convert_string(argv[3],&arg1len);
	ସ୍ଥିତି 3:
	    cmdname = argv[2];
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	    do_command();
	    ଭାଙ୍ଗ;
    }

    ଯଦି (tdb) tdb_close(tdb);

    ଫେରନ୍ତୁ 0;
}
