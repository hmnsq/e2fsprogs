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

#అన్తర్భావ "config.h"
#అన్తర్భావ <errno.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/mman.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <ctype.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <stdarg.h>

#అన్తర్భావ "tdb.h"

స్థిర పూర్ణాంకం do_command(శూన్య);
స్థిరరాశి అక్షరం *cmdname;
అక్షరం *arg1, *arg2;
size_t arg1len, arg2len;
పూర్ణాంకం bIterate = 0;
అక్షరం *పంక్తి;
TDB_దత్తాంశం iterate_kbuf;
అక్షరం cmdline[1024];

గణన commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_చొప్పించు,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_కీలకాలు,
	CMD_HEXKEYS,
	CMD_DELETE,
	CMD_LIST_HASH_FREE,
	CMD_LIST_FREE,
	CMD_INFO,
	CMD_మొదటి,
	CMD_తదుపరి,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

రకనిర్వచనం నిర్మాణం {
	స్థిరరాశి అక్షరం *name;
	గణన commands cmd;
} COMMAND_పట్టిక;

COMMAND_పట్టిక cmd_పట్టిక[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_చొప్పించు},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_కీలకాలు},
	{"hexkeys",	CMD_HEXKEYS},
	{"delete",	CMD_DELETE},
	{"list",	CMD_LIST_HASH_FREE},
	{"free",	CMD_LIST_FREE},
	{"info",	CMD_INFO},
	{"first",	CMD_మొదటి},
	{"1",		CMD_మొదటి},
	{"next",	CMD_తదుపరి},
	{"n",		CMD_తదుపరి},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{ఖాళీ,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

స్థిర TDB_CONTEXT *tdb;

స్థిర పూర్ణాంకం print_rec(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state);
స్థిర పూర్ణాంకం print_కీలకం(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state);
స్థిర పూర్ణాంకం print_hexkey(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state);

స్థిర శూన్య print_asc(స్థిరరాశి అక్షరం *buf,పూర్ణాంకం len)
{
	పూర్ణాంకం i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	ఒకవేళ (buf[len - 1] == 0)
	        len--;

	కొరకు (i=0;i<len;i++)
		రాయి("%c",isprint(buf[i])?buf[i]:'.');
}

స్థిర శూన్య print_దత్తాంశం(స్థిరరాశి అక్షరం *buf,పూర్ణాంకం len)
{
	పూర్ణాంకం i=0;
	ఒకవేళ (len<=0) తిరిగి;
	రాయి("[%03X] ",i);
	కొరకు (i=0;i<len;) {
		రాయి("%02X ",(పూర్ణాంకం)buf[i]);
		i++;
		ఒకవేళ (i%8 == 0) రాయి(" ");
		ఒకవేళ (i%16 == 0) {
			print_asc(&buf[i-16],8); రాయి(" ");
			print_asc(&buf[i-8],8); రాయి("\n");
			ఒకవేళ (i<len) రాయి("[%03X] ",i);
		}
	}
	ఒకవేళ (i%16) {
		పూర్ణాంకం n;

		n = 16 - (i%16);
		రాయి(" ");
		ఒకవేళ (n>8) రాయి(" ");
		అయితే (n--) రాయి("   ");

		n = i%16;
		ఒకవేళ (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); రాయి(" ");
		n = (i%16) - n;
		ఒకవేళ (n>0) print_asc(&buf[i-n],n);
		రాయి("\n");
	}
}

స్థిర శూన్య help(శూన్య)
{
	రాయి("\n"
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

స్థిర శూన్య terror(స్థిరరాశి అక్షరం *why)
{
	రాయి("%s\n", why);
}

స్థిర శూన్య create_tdb(స్థిరరాశి అక్షరం *tdbname)
{
	ఒకవేళ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_మొదటి,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	ఒకవేళ (!tdb) {
		రాయి("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

స్థిర శూన్య open_tdb(స్థిరరాశి అక్షరం *tdbname)
{
	ఒకవేళ (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ఒకవేళ (!tdb) {
		రాయి("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

స్థిర శూన్య చొప్పించు_tdb(అక్షరం *keyname, size_t keylen, అక్షరం* దత్తాంశం, size_t datalen)
{
	TDB_దత్తాంశం కీలకం, dbuf;

	ఒకవేళ ((keyname == ఖాళీ) || (keylen == 0)) {
		terror("need key");
		తిరిగి;
	}

	కీలకం.dptr = (గుర్తులేని అక్షరం *)keyname;
	కీలకం.dsize = keylen;
	dbuf.dptr = (గుర్తులేని అక్షరం *)దత్తాంశం;
	dbuf.dsize = datalen;

	ఒకవేళ (tdb_store(tdb, కీలకం, dbuf, TDB_చొప్పించు) == -1) {
		terror("insert failed");
	}
}

స్థిర శూన్య store_tdb(అక్షరం *keyname, size_t keylen, అక్షరం* దత్తాంశం, size_t datalen)
{
	TDB_దత్తాంశం కీలకం, dbuf;

	ఒకవేళ ((keyname == ఖాళీ) || (keylen == 0)) {
		terror("need key");
		తిరిగి;
	}

	ఒకవేళ ((దత్తాంశం == ఖాళీ) || (datalen == 0)) {
		terror("need data");
		తిరిగి;
	}

	కీలకం.dptr = (గుర్తులేని అక్షరం *)keyname;
	కీలకం.dsize = keylen;
	dbuf.dptr = (గుర్తులేని అక్షరం *)దత్తాంశం;
	dbuf.dsize = datalen;

	రాయి("Storing key:\n");
	print_rec(tdb, కీలకం, dbuf, ఖాళీ);

	ఒకవేళ (tdb_store(tdb, కీలకం, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

స్థిర శూన్య show_tdb(అక్షరం *keyname, size_t keylen)
{
	TDB_దత్తాంశం కీలకం, dbuf;

	ఒకవేళ ((keyname == ఖాళీ) || (keylen == 0)) {
		terror("need key");
		తిరిగి;
	}

	కీలకం.dptr = (గుర్తులేని అక్షరం *)keyname;
	కీలకం.dsize = keylen;

	dbuf = tdb_fetch(tdb, కీలకం);
	ఒకవేళ (!dbuf.dptr) {
	    terror("fetch failed");
	    తిరిగి;
	}

	print_rec(tdb, కీలకం, dbuf, ఖాళీ);

	విడుదల( dbuf.dptr );

	తిరిగి;
}

స్థిర శూన్య delete_tdb(అక్షరం *keyname, size_t keylen)
{
	TDB_దత్తాంశం కీలకం;

	ఒకవేళ ((keyname == ఖాళీ) || (keylen == 0)) {
		terror("need key");
		తిరిగి;
	}

	కీలకం.dptr = (గుర్తులేని అక్షరం *)keyname;
	కీలకం.dsize = keylen;

	ఒకవేళ (tdb_delete(tdb, కీలకం) != 0) {
		terror("delete failed");
	}
}

స్థిర శూన్య move_rec(అక్షరం *keyname, size_t keylen, అక్షరం* tdbname)
{
	TDB_దత్తాంశం కీలకం, dbuf;
	TDB_CONTEXT *dst_tdb;

	ఒకవేళ ((keyname == ఖాళీ) || (keylen == 0)) {
		terror("need key");
		తిరిగి;
	}

	ఒకవేళ ( !tdbname ) {
		terror("need destination tdb name");
		తిరిగి;
	}

	కీలకం.dptr = (గుర్తులేని అక్షరం *)keyname;
	కీలకం.dsize = keylen;

	dbuf = tdb_fetch(tdb, కీలకం);
	ఒకవేళ (!dbuf.dptr) {
		terror("fetch failed");
		తిరిగి;
	}

	print_rec(tdb, కీలకం, dbuf, ఖాళీ);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	ఒకవేళ ( !dst_tdb ) {
		terror("unable to open destination tdb");
		తిరిగి;
	}

	ఒకవేళ ( tdb_store( dst_tdb, కీలకం, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	లేకపోతే
		రాయి("record moved\n");

	tdb_close( dst_tdb );

	తిరిగి;
}

స్థిర పూర్ణాంకం print_rec(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state)
{
	రాయి("\nkey %d bytes\n", (పూర్ణాంకం)కీలకం.dsize);
	print_asc((స్థిరరాశి అక్షరం *)కీలకం.dptr, కీలకం.dsize);
	రాయి("\ndata %d bytes\n", (పూర్ణాంకం)dbuf.dsize);
	print_దత్తాంశం((స్థిరరాశి అక్షరం *)dbuf.dptr, dbuf.dsize);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం print_కీలకం(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state)
{
	రాయి("key %d bytes: ", (పూర్ణాంకం)కీలకం.dsize);
	print_asc((స్థిరరాశి అక్షరం *)కీలకం.dptr, కీలకం.dsize);
	రాయి("\n");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం print_hexkey(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state)
{
	రాయి("key %d bytes\n", (పూర్ణాంకం)కీలకం.dsize);
	print_దత్తాంశం((స్థిరరాశి అక్షరం *)కీలకం.dptr, కీలకం.dsize);
	రాయి("\n");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం మొత్తం_బైట్లు;

స్థిర పూర్ణాంకం traverse_fn(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf, శూన్య *state)
{
	మొత్తం_బైట్లు += dbuf.dsize;
	తిరిగి 0;
}

స్థిర శూన్య info_tdb(శూన్య)
{
	పూర్ణాంకం లెక్క;
	మొత్తం_బైట్లు = 0;
	ఒకవేళ ((లెక్క = tdb_traverse(tdb, traverse_fn, ఖాళీ)) == -1)
		రాయి("Error = %s\n", tdb_errorstr(tdb));
	లేకపోతే
		రాయి("%d records totalling %d bytes\n", లెక్క, మొత్తం_బైట్లు);
}

స్థిర అక్షరం *tdb_getline(స్థిరరాశి అక్షరం *prompt)
{
	స్థిర అక్షరం thisline[1024];
	అక్షరం *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, పరిమాణం(thisline)-1, stdin);
	ఒకవేళ (p) p = strchr(p, '\n');
	ఒకవేళ (p) *p = 0;
	తిరిగి p?thisline:ఖాళీ;
}

స్థిర పూర్ణాంకం do_delete_fn(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం కీలకం, TDB_దత్తాంశం dbuf,
                     శూన్య *state)
{
    తిరిగి tdb_delete(the_tdb, కీలకం);
}

స్థిర శూన్య మొదటి_రికార్డు(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం *pkey)
{
	TDB_దత్తాంశం dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ఒకవేళ (!dbuf.dptr) terror("fetch failed");
	లేకపోతే {
		print_rec(the_tdb, *pkey, dbuf, ఖాళీ);
	}
}

స్థిర శూన్య తదుపరి_రికార్డు(TDB_CONTEXT *the_tdb, TDB_దత్తాంశం *pkey)
{
	TDB_దత్తాంశం dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	ఒకవేళ (!dbuf.dptr)
		terror("fetch failed");
	లేకపోతే
		print_rec(the_tdb, *pkey, dbuf, ఖాళీ);
}

స్థిర పూర్ణాంకం do_command(శూన్య)
{
	COMMAND_పట్టిక *ctp = cmd_పట్టిక;
	గణన commands mycmd = CMD_HELP;
	పూర్ణాంకం cmd_len;

	ఒకవేళ (cmdname && అక్షరశ్రేణి_పొడవు(cmdname) == 0) {
	    mycmd = CMD_తదుపరి;
	} లేకపోతే {
	    అయితే (ctp->name) {
		cmd_len = అక్షరశ్రేణి_పొడవు(ctp->name);
		ఒకవేళ (అక్షరశ్రేణి_పోలిక(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			ఆపు;
		}
		ctp++;
	    }
	}

	ఎంపిక (mycmd) {
	స్థితి CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    తిరిగి 0;
	స్థితి CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            తిరిగి 0;
	స్థితి CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    తిరిగి 0;
	స్థితి CMD_QUIT:
	    తిరిగి 1;
	అప్రమేయ:
	    /* all the rest require a open database */
	    ఒకవేళ (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		తిరిగి 0;
	    }
	    ఎంపిక (mycmd) {
	    స్థితి CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, ఖాళీ);
		తిరిగి 0;
	    స్థితి CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, ఖాళీ);
		తిరిగి 0;
	    స్థితి CMD_చొప్పించు:
		bIterate = 0;
		చొప్పించు_tdb(arg1, arg1len,arg2,arg2len);
		తిరిగి 0;
	    స్థితి CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		తిరిగి 0;
	    స్థితి CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		తిరిగి 0;
	    స్థితి CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		తిరిగి 0;
	    స్థితి CMD_కీలకాలు:
		tdb_traverse(tdb, print_కీలకం, ఖాళీ);
		తిరిగి 0;
	    స్థితి CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, ఖాళీ);
		తిరిగి 0;
	    స్థితి CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		తిరిగి 0;
	    స్థితి CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		తిరిగి 0;
	    స్థితి CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		తిరిగి 0;
	    స్థితి CMD_INFO:
		info_tdb();
		తిరిగి 0;
	    స్థితి CMD_మొదటి:
		bIterate = 1;
		మొదటి_రికార్డు(tdb, &iterate_kbuf);
		తిరిగి 0;
	    స్థితి CMD_తదుపరి:
	       ఒకవేళ (bIterate)
		  తదుపరి_రికార్డు(tdb, &iterate_kbuf);
		తిరిగి 0;
	    స్థితి CMD_HELP:
		help();
		తిరిగి 0;
            స్థితి CMD_CREATE_TDB:
            స్థితి CMD_OPEN_TDB:
            స్థితి CMD_SYSTEM:
            స్థితి CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                తిరిగి 0;
	    }
	}

	తిరిగి 0;
}

స్థిర అక్షరం *convert_string(అక్షరం *instring, size_t *sizep)
{
    size_t నిడివి = 0;
    అక్షరం *outp, *inp;
    అక్షరం తాత్కాలిక[3];


    outp = inp = instring;

    అయితే (*inp) {
	ఒకవేళ (*inp == '\\') {
	    inp++;
	    ఒకవేళ (*inp && strchr("0123456789abcdefABCDEF",(పూర్ణాంకం)*inp)) {
		తాత్కాలిక[0] = *inp++;
		తాత్కాలిక[1] = '\0';
		ఒకవేళ (*inp && strchr("0123456789abcdefABCDEF",(పూర్ణాంకం)*inp)) {
		    తాత్కాలిక[1] = *inp++;
		    తాత్కాలిక[2] = '\0';
		}
		*outp++ = (అక్షరం)strtol((స్థిరరాశి అక్షరం *)తాత్కాలిక,ఖాళీ,16);
	    } లేకపోతే {
		*outp++ = *inp++;
	    }
	} లేకపోతే {
	    *outp++ = *inp++;
	}
	నిడివి++;
    }
    *sizep = నిడివి;
    తిరిగి instring;
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం *argv[])
{
    cmdname = "";
    arg1 = ఖాళీ;
    arg1len = 0;
    arg2 = ఖాళీ;
    arg2len = 0;

    ఒకవేళ (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = ఖాళీ;
    }

    ఎంపిక (argc) {
	స్థితి 1:
	స్థితి 2:
	    /* Interactive mode */
	    అయితే ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = ఖాళీ;
		ఒకవేళ ((arg1 = strchr((స్థిరరాశి అక్షరం *)cmdname,' ')) != ఖాళీ) {
		    arg1++;
		    arg2 = arg1;
		    అయితే (*arg2) {
			ఒకవేళ (*arg2 == ' ') {
			    *arg2++ = '\0';
			    ఆపు;
			}
			ఒకవేళ ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		ఒకవేళ (arg1) arg1 = convert_string(arg1,&arg1len);
		ఒకవేళ (arg2) arg2 = convert_string(arg2,&arg2len);
		ఒకవేళ (do_command()) ఆపు;
	    }
	    ఆపు;
	స్థితి 5:
	    arg2 = convert_string(argv[4],&arg2len);
	స్థితి 4:
	    arg1 = convert_string(argv[3],&arg1len);
	స్థితి 3:
	    cmdname = argv[2];
	అప్రమేయ:
	    do_command();
	    ఆపు;
    }

    ఒకవేళ (tdb) tdb_close(tdb);

    తిరిగి 0;
}
