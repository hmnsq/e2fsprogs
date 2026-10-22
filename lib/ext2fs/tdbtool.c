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

#अन्तर्भाव "config.h"
#अन्तर्भाव <errno.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/mman.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <ctype.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <stdarg.h>

#अन्तर्भाव "tdb.h"

स्थिर पूर्णाङ्क do_command(शून्य);
ध्रुव वर्ण *cmdname;
वर्ण *arg1, *arg2;
size_t arg1len, arg2len;
पूर्णाङ्क bIterate = 0;
वर्ण *पङ्क्तिः;
TDB_दत्त iterate_kbuf;
वर्ण cmdline[1024];

गणना commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_निवेशय,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_कुञ्चिकाः,
	CMD_HEXKEYS,
	CMD_DELETE,
	CMD_LIST_HASH_FREE,
	CMD_LIST_FREE,
	CMD_INFO,
	CMD_FIRST,
	CMD_अग्र,
	CMD_SYSTEM,
	CMD_QUIT,
	CMD_HELP
};

प्रकारनिर्देश संरचना {
	ध्रुव वर्ण *name;
	गणना commands cmd;
} COMMAND_सारणी;

COMMAND_सारणी cmd_सारणी[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_निवेशय},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_कुञ्चिकाः},
	{"hexkeys",	CMD_HEXKEYS},
	{"delete",	CMD_DELETE},
	{"list",	CMD_LIST_HASH_FREE},
	{"free",	CMD_LIST_FREE},
	{"info",	CMD_INFO},
	{"first",	CMD_FIRST},
	{"1",		CMD_FIRST},
	{"next",	CMD_अग्र},
	{"n",		CMD_अग्र},
	{"quit",	CMD_QUIT},
	{"q",		CMD_QUIT},
	{"!",		CMD_SYSTEM},
	{रिक्त,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

स्थिर TDB_CONTEXT *tdb;

स्थिर पूर्णाङ्क print_rec(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state);
स्थिर पूर्णाङ्क print_कुञ्चिका(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state);
स्थिर पूर्णाङ्क print_hexkey(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state);

स्थिर शून्य print_asc(ध्रुव वर्ण *buf,पूर्णाङ्क len)
{
	पूर्णाङ्क i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	यदि (buf[len - 1] == 0)
	        len--;

	प्रति (i=0;i<len;i++)
		प्रारूप_लिख("%c",isprint(buf[i])?buf[i]:'.');
}

स्थिर शून्य print_दत्त(ध्रुव वर्ण *buf,पूर्णाङ्क len)
{
	पूर्णाङ्क i=0;
	यदि (len<=0) प्रत्यागम;
	प्रारूप_लिख("[%03X] ",i);
	प्रति (i=0;i<len;) {
		प्रारूप_लिख("%02X ",(पूर्णाङ्क)buf[i]);
		i++;
		यदि (i%8 == 0) प्रारूप_लिख(" ");
		यदि (i%16 == 0) {
			print_asc(&buf[i-16],8); प्रारूप_लिख(" ");
			print_asc(&buf[i-8],8); प्रारूप_लिख("\n");
			यदि (i<len) प्रारूप_लिख("[%03X] ",i);
		}
	}
	यदि (i%16) {
		पूर्णाङ्क n;

		n = 16 - (i%16);
		प्रारूप_लिख(" ");
		यदि (n>8) प्रारूप_लिख(" ");
		यावत् (n--) प्रारूप_लिख("   ");

		n = i%16;
		यदि (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); प्रारूप_लिख(" ");
		n = (i%16) - n;
		यदि (n>0) print_asc(&buf[i-n],n);
		प्रारूप_लिख("\n");
	}
}

स्थिर शून्य help(शून्य)
{
	प्रारूप_लिख("\n"
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

स्थिर शून्य terror(ध्रुव वर्ण *why)
{
	प्रारूप_लिख("%s\n", why);
}

स्थिर शून्य create_tdb(ध्रुव वर्ण *tdbname)
{
	यदि (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	यदि (!tdb) {
		प्रारूप_लिख("Could not create %s: %s\n", tdbname, strerror(errno));
	}
}

स्थिर शून्य open_tdb(ध्रुव वर्ण *tdbname)
{
	यदि (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	यदि (!tdb) {
		प्रारूप_लिख("Could not open %s: %s\n", tdbname, strerror(errno));
	}
}

स्थिर शून्य निवेशय_tdb(वर्ण *keyname, size_t keylen, वर्ण* दत्त, size_t datalen)
{
	TDB_दत्त कुञ्चिका, dbuf;

	यदि ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		प्रत्यागम;
	}

	कुञ्चिका.dptr = (अचिह्नित वर्ण *)keyname;
	कुञ्चिका.dsize = keylen;
	dbuf.dptr = (अचिह्नित वर्ण *)दत्त;
	dbuf.dsize = datalen;

	यदि (tdb_store(tdb, कुञ्चिका, dbuf, TDB_निवेशय) == -1) {
		terror("insert failed");
	}
}

स्थिर शून्य store_tdb(वर्ण *keyname, size_t keylen, वर्ण* दत्त, size_t datalen)
{
	TDB_दत्त कुञ्चिका, dbuf;

	यदि ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		प्रत्यागम;
	}

	यदि ((दत्त == रिक्त) || (datalen == 0)) {
		terror("need data");
		प्रत्यागम;
	}

	कुञ्चिका.dptr = (अचिह्नित वर्ण *)keyname;
	कुञ्चिका.dsize = keylen;
	dbuf.dptr = (अचिह्नित वर्ण *)दत्त;
	dbuf.dsize = datalen;

	प्रारूप_लिख("Storing key:\n");
	print_rec(tdb, कुञ्चिका, dbuf, रिक्त);

	यदि (tdb_store(tdb, कुञ्चिका, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

स्थिर शून्य show_tdb(वर्ण *keyname, size_t keylen)
{
	TDB_दत्त कुञ्चिका, dbuf;

	यदि ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		प्रत्यागम;
	}

	कुञ्चिका.dptr = (अचिह्नित वर्ण *)keyname;
	कुञ्चिका.dsize = keylen;

	dbuf = tdb_fetch(tdb, कुञ्चिका);
	यदि (!dbuf.dptr) {
	    terror("fetch failed");
	    प्रत्यागम;
	}

	print_rec(tdb, कुञ्चिका, dbuf, रिक्त);

	मुक्त( dbuf.dptr );

	प्रत्यागम;
}

स्थिर शून्य delete_tdb(वर्ण *keyname, size_t keylen)
{
	TDB_दत्त कुञ्चिका;

	यदि ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		प्रत्यागम;
	}

	कुञ्चिका.dptr = (अचिह्नित वर्ण *)keyname;
	कुञ्चिका.dsize = keylen;

	यदि (tdb_delete(tdb, कुञ्चिका) != 0) {
		terror("delete failed");
	}
}

स्थिर शून्य move_rec(वर्ण *keyname, size_t keylen, वर्ण* tdbname)
{
	TDB_दत्त कुञ्चिका, dbuf;
	TDB_CONTEXT *dst_tdb;

	यदि ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		प्रत्यागम;
	}

	यदि ( !tdbname ) {
		terror("need destination tdb name");
		प्रत्यागम;
	}

	कुञ्चिका.dptr = (अचिह्नित वर्ण *)keyname;
	कुञ्चिका.dsize = keylen;

	dbuf = tdb_fetch(tdb, कुञ्चिका);
	यदि (!dbuf.dptr) {
		terror("fetch failed");
		प्रत्यागम;
	}

	print_rec(tdb, कुञ्चिका, dbuf, रिक्त);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	यदि ( !dst_tdb ) {
		terror("unable to open destination tdb");
		प्रत्यागम;
	}

	यदि ( tdb_store( dst_tdb, कुञ्चिका, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	अन्यथा
		प्रारूप_लिख("record moved\n");

	tdb_close( dst_tdb );

	प्रत्यागम;
}

स्थिर पूर्णाङ्क print_rec(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state)
{
	प्रारूप_लिख("\nkey %d bytes\n", (पूर्णाङ्क)कुञ्चिका.dsize);
	print_asc((ध्रुव वर्ण *)कुञ्चिका.dptr, कुञ्चिका.dsize);
	प्रारूप_लिख("\ndata %d bytes\n", (पूर्णाङ्क)dbuf.dsize);
	print_दत्त((ध्रुव वर्ण *)dbuf.dptr, dbuf.dsize);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क print_कुञ्चिका(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state)
{
	प्रारूप_लिख("key %d bytes: ", (पूर्णाङ्क)कुञ्चिका.dsize);
	print_asc((ध्रुव वर्ण *)कुञ्चिका.dptr, कुञ्चिका.dsize);
	प्रारूप_लिख("\n");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क print_hexkey(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state)
{
	प्रारूप_लिख("key %d bytes\n", (पूर्णाङ्क)कुञ्चिका.dsize);
	print_दत्त((ध्रुव वर्ण *)कुञ्चिका.dptr, कुञ्चिका.dsize);
	प्रारूप_लिख("\n");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क योग_अष्टकानि;

स्थिर पूर्णाङ्क traverse_fn(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf, शून्य *state)
{
	योग_अष्टकानि += dbuf.dsize;
	प्रत्यागम 0;
}

स्थिर शून्य info_tdb(शून्य)
{
	पूर्णाङ्क गणनम्;
	योग_अष्टकानि = 0;
	यदि ((गणनम् = tdb_traverse(tdb, traverse_fn, रिक्त)) == -1)
		प्रारूप_लिख("Error = %s\n", tdb_errorstr(tdb));
	अन्यथा
		प्रारूप_लिख("%d records totalling %d bytes\n", गणनम्, योग_अष्टकानि);
}

स्थिर वर्ण *tdb_getline(ध्रुव वर्ण *prompt)
{
	स्थिर वर्ण thisline[1024];
	वर्ण *p;
	fputs(prompt, stdout);
	thisline[0] = 0;
	p = fgets(thisline, परिमाण(thisline)-1, stdin);
	यदि (p) p = strchr(p, '\n');
	यदि (p) *p = 0;
	प्रत्यागम p?thisline:रिक्त;
}

स्थिर पूर्णाङ्क do_delete_fn(TDB_CONTEXT *the_tdb, TDB_दत्त कुञ्चिका, TDB_दत्त dbuf,
                     शून्य *state)
{
    प्रत्यागम tdb_delete(the_tdb, कुञ्चिका);
}

स्थिर शून्य first_लेखः(TDB_CONTEXT *the_tdb, TDB_दत्त *pkey)
{
	TDB_दत्त dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	यदि (!dbuf.dptr) terror("fetch failed");
	अन्यथा {
		print_rec(the_tdb, *pkey, dbuf, रिक्त);
	}
}

स्थिर शून्य अग्र_लेखः(TDB_CONTEXT *the_tdb, TDB_दत्त *pkey)
{
	TDB_दत्त dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	यदि (!dbuf.dptr)
		terror("fetch failed");
	अन्यथा
		print_rec(the_tdb, *pkey, dbuf, रिक्त);
}

स्थिर पूर्णाङ्क do_command(शून्य)
{
	COMMAND_सारणी *ctp = cmd_सारणी;
	गणना commands mycmd = CMD_HELP;
	पूर्णाङ्क cmd_len;

	यदि (cmdname && वाक्यदैर्घ्यम्(cmdname) == 0) {
	    mycmd = CMD_अग्र;
	} अन्यथा {
	    यावत् (ctp->name) {
		cmd_len = वाक्यदैर्घ्यम्(ctp->name);
		यदि (वाक्ये_तुलय(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			भङ्ग;
		}
		ctp++;
	    }
	}

	विकल्प (mycmd) {
	पक्ष CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    प्रत्यागम 0;
	पक्ष CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            प्रत्यागम 0;
	पक्ष CMD_SYSTEM:
	    /* Shell command */
	    system(arg1);
	    प्रत्यागम 0;
	पक्ष CMD_QUIT:
	    प्रत्यागम 1;
	स्वभाव:
	    /* all the rest require a open database */
	    यदि (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		प्रत्यागम 0;
	    }
	    विकल्प (mycmd) {
	    पक्ष CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, रिक्त);
		प्रत्यागम 0;
	    पक्ष CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, रिक्त);
		प्रत्यागम 0;
	    पक्ष CMD_निवेशय:
		bIterate = 0;
		निवेशय_tdb(arg1, arg1len,arg2,arg2len);
		प्रत्यागम 0;
	    पक्ष CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		प्रत्यागम 0;
	    पक्ष CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		प्रत्यागम 0;
	    पक्ष CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		प्रत्यागम 0;
	    पक्ष CMD_कुञ्चिकाः:
		tdb_traverse(tdb, print_कुञ्चिका, रिक्त);
		प्रत्यागम 0;
	    पक्ष CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, रिक्त);
		प्रत्यागम 0;
	    पक्ष CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		प्रत्यागम 0;
	    पक्ष CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		प्रत्यागम 0;
	    पक्ष CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		प्रत्यागम 0;
	    पक्ष CMD_INFO:
		info_tdb();
		प्रत्यागम 0;
	    पक्ष CMD_FIRST:
		bIterate = 1;
		first_लेखः(tdb, &iterate_kbuf);
		प्रत्यागम 0;
	    पक्ष CMD_अग्र:
	       यदि (bIterate)
		  अग्र_लेखः(tdb, &iterate_kbuf);
		प्रत्यागम 0;
	    पक्ष CMD_HELP:
		help();
		प्रत्यागम 0;
            पक्ष CMD_CREATE_TDB:
            पक्ष CMD_OPEN_TDB:
            पक्ष CMD_SYSTEM:
            पक्ष CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                प्रत्यागम 0;
	    }
	}

	प्रत्यागम 0;
}

स्थिर वर्ण *convert_string(वर्ण *instring, size_t *sizep)
{
    size_t दीर्घता = 0;
    वर्ण *outp, *inp;
    वर्ण अस्थायी[3];


    outp = inp = instring;

    यावत् (*inp) {
	यदि (*inp == '\\') {
	    inp++;
	    यदि (*inp && strchr("0123456789abcdefABCDEF",(पूर्णाङ्क)*inp)) {
		अस्थायी[0] = *inp++;
		अस्थायी[1] = '\0';
		यदि (*inp && strchr("0123456789abcdefABCDEF",(पूर्णाङ्क)*inp)) {
		    अस्थायी[1] = *inp++;
		    अस्थायी[2] = '\0';
		}
		*outp++ = (वर्ण)strtol((ध्रुव वर्ण *)अस्थायी,रिक्त,16);
	    } अन्यथा {
		*outp++ = *inp++;
	    }
	} अन्यथा {
	    *outp++ = *inp++;
	}
	दीर्घता++;
    }
    *sizep = दीर्घता;
    प्रत्यागम instring;
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण *argv[])
{
    cmdname = "";
    arg1 = रिक्त;
    arg1len = 0;
    arg2 = रिक्त;
    arg2len = 0;

    यदि (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = रिक्त;
    }

    विकल्प (argc) {
	पक्ष 1:
	पक्ष 2:
	    /* Interactive mode */
	    यावत् ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = रिक्त;
		यदि ((arg1 = strchr((ध्रुव वर्ण *)cmdname,' ')) != रिक्त) {
		    arg1++;
		    arg2 = arg1;
		    यावत् (*arg2) {
			यदि (*arg2 == ' ') {
			    *arg2++ = '\0';
			    भङ्ग;
			}
			यदि ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		यदि (arg1) arg1 = convert_string(arg1,&arg1len);
		यदि (arg2) arg2 = convert_string(arg2,&arg2len);
		यदि (do_command()) भङ्ग;
	    }
	    भङ्ग;
	पक्ष 5:
	    arg2 = convert_string(argv[4],&arg2len);
	पक्ष 4:
	    arg1 = convert_string(argv[3],&arg1len);
	पक्ष 3:
	    cmdname = argv[2];
	स्वभाव:
	    do_command();
	    भङ्ग;
    }

    यदि (tdb) tdb_close(tdb);

    प्रत्यागम 0;
}
