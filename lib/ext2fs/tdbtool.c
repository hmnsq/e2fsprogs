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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/mman.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <ctype.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <stdarg.h>

#અન્તર્ભાવ "tdb.h"

સ્થિર પૂર્ણાંક do_command(શૂન્ય);
અચલ અક્ષર *cmdname;
અક્ષર *arg1, *arg2;
size_t arg1len, arg2len;
પૂર્ણાંક bIterate = 0;
અક્ષર *લીટી;
TDB_DATA iterate_kbuf;
અક્ષર cmdline[1024];

ગણના commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_ઉમેરો,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_ચાવીઓ,
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

પ્રકારવ્યાખ્યા સંરચના {
	અચલ અક્ષર *name;
	ગણના commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_ઉમેરો},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_ચાવીઓ},
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
	{ખાલી,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

સ્થિર TDB_CONTEXT *tdb;

સ્થિર પૂર્ણાંક print_rec(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state);
સ્થિર પૂર્ણાંક print_ચાવી(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state);
સ્થિર પૂર્ણાંક print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state);

સ્થિર શૂન્ય print_asc(અચલ અક્ષર *buf,પૂર્ણાંક len)
{
	પૂર્ણાંક i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	જો (buf[len - 1] == 0)
	        len--;

	માટે (i=0;i<len;i++)
		printf("%c",isprint(buf[i])?buf[i]:'.');
}

સ્થિર શૂન્ય print_data(અચલ અક્ષર *buf,પૂર્ણાંક len)
{
	પૂર્ણાંક i=0;
	જો (len<=0) પાછા;
	printf("[%03X] ",i);
	માટે (i=0;i<len;) {
		printf("%02X ",(પૂર્ણાંક)buf[i]);
		i++;
		જો (i%8 == 0) printf(" ");
		જો (i%16 == 0) {
			print_asc(&buf[i-16],8); printf(" ");
			print_asc(&buf[i-8],8); printf("\n");
			જો (i<len) printf("[%03X] ",i);
		}
	}
	જો (i%16) {
		પૂર્ણાંક n;

		n = 16 - (i%16);
		printf(" ");
		જો (n>8) printf(" ");
		જ્યારે (n--) printf("   ");

		n = i%16;
		જો (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		જો (n>0) print_asc(&buf[i-n],n);
		printf("\n");
	}
}

સ્થિર શૂન્ય help(શૂન્ય)
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

સ્થિર શૂન્ય terror(અચલ અક્ષર *why)
{
	printf("%s\n", why);
}

સ્થિર શૂન્ય create_tdb(અચલ અક્ષર *tdbname)
{
	જો (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	જો (!tdb) {
		printf("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

સ્થિર શૂન્ય open_tdb(અચલ અક્ષર *tdbname)
{
	જો (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	જો (!tdb) {
		printf("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

સ્થિર શૂન્ય ઉમેરો_tdb(અક્ષર *keyname, size_t keylen, અક્ષર* data, size_t datalen)
{
	TDB_DATA ચાવી, dbuf;

	જો ((keyname == ખાલી) || (keylen == 0)) {
		terror("need key");
		પાછા;
	}

	ચાવી.dptr = (unsigned અક્ષર *)keyname;
	ચાવી.dsize = keylen;
	dbuf.dptr = (unsigned અક્ષર *)data;
	dbuf.dsize = datalen;

	જો (tdb_store(tdb, ચાવી, dbuf, TDB_ઉમેરો) == -1) {
		terror("insert failed");
	}
}

સ્થિર શૂન્ય store_tdb(અક્ષર *keyname, size_t keylen, અક્ષર* data, size_t datalen)
{
	TDB_DATA ચાવી, dbuf;

	જો ((keyname == ખાલી) || (keylen == 0)) {
		terror("need key");
		પાછા;
	}

	જો ((data == ખાલી) || (datalen == 0)) {
		terror("need data");
		પાછા;
	}

	ચાવી.dptr = (unsigned અક્ષર *)keyname;
	ચાવી.dsize = keylen;
	dbuf.dptr = (unsigned અક્ષર *)data;
	dbuf.dsize = datalen;

	printf("Storing key:\n");
	print_rec(tdb, ચાવી, dbuf, ખાલી);

	જો (tdb_store(tdb, ચાવી, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

સ્થિર શૂન્ય show_tdb(અક્ષર *keyname, size_t keylen)
{
	TDB_DATA ચાવી, dbuf;

	જો ((keyname == ખાલી) || (keylen == 0)) {
		terror("need key");
		પાછા;
	}

	ચાવી.dptr = (unsigned અક્ષર *)keyname;
	ચાવી.dsize = keylen;

	dbuf = tdb_fetch(tdb, ચાવી);
	જો (!dbuf.dptr) {
	    terror("fetch failed");
	    પાછા;
	}

	print_rec(tdb, ચાવી, dbuf, ખાલી);

	મુક્ત( dbuf.dptr );

	પાછા;
}

સ્થિર શૂન્ય delete_tdb(અક્ષર *keyname, size_t keylen)
{
	TDB_DATA ચાવી;

	જો ((keyname == ખાલી) || (keylen == 0)) {
		terror("need key");
		પાછા;
	}

	ચાવી.dptr = (unsigned અક્ષર *)keyname;
	ચાવી.dsize = keylen;

	જો (tdb_delete(tdb, ચાવી) != 0) {
		terror("delete failed");
	}
}

સ્થિર શૂન્ય move_rec(અક્ષર *keyname, size_t keylen, અક્ષર* tdbname)
{
	TDB_DATA ચાવી, dbuf;
	TDB_CONTEXT *dst_tdb;

	જો ((keyname == ખાલી) || (keylen == 0)) {
		terror("need key");
		પાછા;
	}

	જો ( !tdbname ) {
		terror("need destination tdb name");
		પાછા;
	}

	ચાવી.dptr = (unsigned અક્ષર *)keyname;
	ચાવી.dsize = keylen;

	dbuf = tdb_fetch(tdb, ચાવી);
	જો (!dbuf.dptr) {
		terror("fetch failed");
		પાછા;
	}

	print_rec(tdb, ચાવી, dbuf, ખાલી);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	જો ( !dst_tdb ) {
		terror("unable to open destination tdb");
		પાછા;
	}

	જો ( tdb_store( dst_tdb, ચાવી, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	નહીંતર
		printf("record moved\n");

	tdb_close( dst_tdb );

	પાછા;
}

સ્થિર પૂર્ણાંક print_rec(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state)
{
	printf("\nkey %d bytes\n", (પૂર્ણાંક)ચાવી.dsize);
	print_asc((અચલ અક્ષર *)ચાવી.dptr, ચાવી.dsize);
	printf("\ndata %d bytes\n", (પૂર્ણાંક)dbuf.dsize);
	print_data((અચલ અક્ષર *)dbuf.dptr, dbuf.dsize);
	પાછા 0;
}

સ્થિર પૂર્ણાંક print_ચાવી(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state)
{
	printf("key %d bytes: ", (પૂર્ણાંક)ચાવી.dsize);
	print_asc((અચલ અક્ષર *)ચાવી.dptr, ચાવી.dsize);
	printf("\n");
	પાછા 0;
}

સ્થિર પૂર્ણાંક print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state)
{
	printf("key %d bytes\n", (પૂર્ણાંક)ચાવી.dsize);
	print_data((અચલ અક્ષર *)ચાવી.dptr, ચાવી.dsize);
	printf("\n");
	પાછા 0;
}

સ્થિર પૂર્ણાંક total_bytes;

સ્થિર પૂર્ણાંક traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf, શૂન્ય *state)
{
	total_bytes += dbuf.dsize;
	પાછા 0;
}

સ્થિર શૂન્ય info_tdb(શૂન્ય)
{
	પૂર્ણાંક ગણતરી;
	total_bytes = 0;
	જો ((ગણતરી = tdb_traverse(tdb, traverse_fn, ખાલી)) == -1)
		printf("Error = %s\n", tdb_errorstr(tdb));
	નહીંતર
		printf("%d records totalling %d bytes\n", ગણતરી, total_bytes);
}

સ્થિર અક્ષર *tdb_getline(અચલ અક્ષર *prompt)
{
	સ્થિર અક્ષર thisline[1024];
	અક્ષર *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, કદ(thisline)-1, stdin);
	જો (p) p = strchr(p, '\n');
	જો (p) *p = 0;
	પાછા p?thisline:ખાલી;
}

સ્થિર પૂર્ણાંક do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA ચાવી, TDB_DATA dbuf,
                     શૂન્ય *state)
{
    પાછા tdb_delete(the_tdb, ચાવી);
}

સ્થિર શૂન્ય first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	જો (!dbuf.dptr) terror("fetch failed");
	નહીંતર {
		print_rec(the_tdb, *pkey, dbuf, ખાલી);
	}
}

સ્થિર શૂન્ય next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	જો (!dbuf.dptr)
		terror("fetch failed");
	નહીંતર
		print_rec(the_tdb, *pkey, dbuf, ખાલી);
}

સ્થિર પૂર્ણાંક do_command(શૂન્ય)
{
	COMMAND_TABLE *ctp = cmd_table;
	ગણના commands mycmd = CMD_HELP;
	પૂર્ણાંક cmd_len;

	જો (cmdname && strlen(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} નહીંતર {
	    જ્યારે (ctp->name) {
		cmd_len = strlen(ctp->name);
		જો (strncmp(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			તોડો;
		}
		ctp++;
	    }
	}

	પસંદગી (mycmd) {
	સ્થિતિ CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    પાછા 0;
	સ્થિતિ CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            પાછા 0;
	સ્થિતિ CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    પાછા 0;
	સ્થિતિ CMD_QUIT:
	    પાછા 1;
	મૂળભૂત:
	    /* all the rest require a open database */
	    જો (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		પાછા 0;
	    }
	    પસંદગી (mycmd) {
	    સ્થિતિ CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ખાલી);
		પાછા 0;
	    સ્થિતિ CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ખાલી);
		પાછા 0;
	    સ્થિતિ CMD_ઉમેરો:
		bIterate = 0;
		ઉમેરો_tdb(arg1, arg1len,arg2,arg2len);
		પાછા 0;
	    સ્થિતિ CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		પાછા 0;
	    સ્થિતિ CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		પાછા 0;
	    સ્થિતિ CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		પાછા 0;
	    સ્થિતિ CMD_ચાવીઓ:
		tdb_traverse(tdb, print_ચાવી, ખાલી);
		પાછા 0;
	    સ્થિતિ CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ખાલી);
		પાછા 0;
	    સ્થિતિ CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		પાછા 0;
	    સ્થિતિ CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		પાછા 0;
	    સ્થિતિ CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		પાછા 0;
	    સ્થિતિ CMD_INFO:
		info_tdb();
		પાછા 0;
	    સ્થિતિ CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		પાછા 0;
	    સ્થિતિ CMD_NEXT:
	       જો (bIterate)
		  next_record(tdb, &iterate_kbuf);
		પાછા 0;
	    સ્થિતિ CMD_HELP:
		help();
		પાછા 0;
            સ્થિતિ CMD_CREATE_TDB:
            સ્થિતિ CMD_OPEN_TDB:
            સ્થિતિ CMD_SYSTEM:
            સ્થિતિ CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                પાછા 0;
	    }
	}

	પાછા 0;
}

સ્થિર અક્ષર *convert_string(અક્ષર *instring, size_t *sizep)
{
    size_t length = 0;
    અક્ષર *outp, *inp;
    અક્ષર temp[3];


    outp = inp = instring;

    જ્યારે (*inp) {
	જો (*inp == '\\') {
	    inp++;
	    જો (*inp && strchr("0123456789abcdefABCDEF",(પૂર્ણાંક)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		જો (*inp && strchr("0123456789abcdefABCDEF",(પૂર્ણાંક)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (અક્ષર)strtol((અચલ અક્ષર *)temp,ખાલી,16);
	    } નહીંતર {
		*outp++ = *inp++;
	    }
	} નહીંતર {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    પાછા instring;
}

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર *argv[])
{
    cmdname = "";
    arg1 = ખાલી;
    arg1len = 0;
    arg2 = ખાલી;
    arg2len = 0;

    જો (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ખાલી;
    }

    પસંદગી (argc) {
	સ્થિતિ 1:
	સ્થિતિ 2:
	    /* Interactive mode */
	    જ્યારે ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ખાલી;
		જો ((arg1 = strchr((અચલ અક્ષર *)cmdname,' ')) != ખાલી) {
		    arg1++;
		    arg2 = arg1;
		    જ્યારે (*arg2) {
			જો (*arg2 == ' ') {
			    *arg2++ = '\0';
			    તોડો;
			}
			જો ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		જો (arg1) arg1 = convert_string(arg1,&arg1len);
		જો (arg2) arg2 = convert_string(arg2,&arg2len);
		જો (do_command()) તોડો;
	    }
	    તોડો;
	સ્થિતિ 5:
	    arg2 = convert_string(argv[4],&arg2len);
	સ્થિતિ 4:
	    arg1 = convert_string(argv[3],&arg1len);
	સ્થિતિ 3:
	    cmdname = argv[2];
	મૂળભૂત:
	    do_command();
	    તોડો;
    }

    જો (tdb) tdb_close(tdb);

    પાછા 0;
}
