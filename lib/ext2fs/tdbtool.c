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

स्थिर पूर्णांक do_command(शून्य);
स्थिरांक अक्षर *cmdname;
अक्षर *arg1, *arg2;
माप_प्रकार arg1len, arg2len;
पूर्णांक bIterate = 0;
अक्षर *ओळ;
TDB_DATA iterate_kbuf;
अक्षर cmdline[1024];

गणना commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_घाला,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_किल्ल्या,
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

प्रकारव्याख्या संरचना {
	स्थिरांक अक्षर *name;
	गणना commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_घाला},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_किल्ल्या},
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
	{रिक्त,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

स्थिर TDB_CONTEXT *tdb;

स्थिर पूर्णांक print_rec(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state);
स्थिर पूर्णांक print_किल्ली(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state);
स्थिर पूर्णांक print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state);

स्थिर शून्य print_asc(स्थिरांक अक्षर *buf,पूर्णांक len)
{
	पूर्णांक i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	जर (buf[len - 1] == 0)
	        len--;

	साठी (i=0;i<len;i++)
		म_लिखो("%c",है_छाप(buf[i])?buf[i]:'.');
}

स्थिर शून्य print_data(स्थिरांक अक्षर *buf,पूर्णांक len)
{
	पूर्णांक i=0;
	जर (len<=0) परत;
	म_लिखो("[%03X] ",i);
	साठी (i=0;i<len;) {
		म_लिखो("%02X ",(पूर्णांक)buf[i]);
		i++;
		जर (i%8 == 0) म_लिखो(" ");
		जर (i%16 == 0) {
			print_asc(&buf[i-16],8); म_लिखो(" ");
			print_asc(&buf[i-8],8); म_लिखो("\n");
			जर (i<len) म_लिखो("[%03X] ",i);
		}
	}
	जर (i%16) {
		पूर्णांक n;

		n = 16 - (i%16);
		म_लिखो(" ");
		जर (n>8) म_लिखो(" ");
		पर्यंत (n--) म_लिखो("   ");

		n = i%16;
		जर (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); म_लिखो(" ");
		n = (i%16) - n;
		जर (n>0) print_asc(&buf[i-n],n);
		म_लिखो("\n");
	}
}

स्थिर शून्य help(शून्य)
{
	म_लिखो("\n"
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

स्थिर शून्य terror(स्थिरांक अक्षर *why)
{
	म_लिखो("%s\n", why);
}

स्थिर शून्य create_tdb(स्थिरांक अक्षर *tdbname)
{
	जर (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	जर (!tdb) {
		म_लिखो("Could not create %s: %s\n", tdbname, म_त्रुटि(त्रुटि_सं));
	}
}

स्थिर शून्य open_tdb(स्थिरांक अक्षर *tdbname)
{
	जर (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	जर (!tdb) {
		म_लिखो("Could not open %s: %s\n", tdbname, म_त्रुटि(त्रुटि_सं));
	}
}

स्थिर शून्य घाला_tdb(अक्षर *keyname, माप_प्रकार keylen, अक्षर* data, माप_प्रकार datalen)
{
	TDB_DATA किल्ली, dbuf;

	जर ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		परत;
	}

	किल्ली.dptr = (अचिन्हित अक्षर *)keyname;
	किल्ली.dsize = keylen;
	dbuf.dptr = (अचिन्हित अक्षर *)data;
	dbuf.dsize = datalen;

	जर (tdb_store(tdb, किल्ली, dbuf, TDB_घाला) == -1) {
		terror("insert failed");
	}
}

स्थिर शून्य store_tdb(अक्षर *keyname, माप_प्रकार keylen, अक्षर* data, माप_प्रकार datalen)
{
	TDB_DATA किल्ली, dbuf;

	जर ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		परत;
	}

	जर ((data == रिक्त) || (datalen == 0)) {
		terror("need data");
		परत;
	}

	किल्ली.dptr = (अचिन्हित अक्षर *)keyname;
	किल्ली.dsize = keylen;
	dbuf.dptr = (अचिन्हित अक्षर *)data;
	dbuf.dsize = datalen;

	म_लिखो("Storing key:\n");
	print_rec(tdb, किल्ली, dbuf, रिक्त);

	जर (tdb_store(tdb, किल्ली, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

स्थिर शून्य show_tdb(अक्षर *keyname, माप_प्रकार keylen)
{
	TDB_DATA किल्ली, dbuf;

	जर ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		परत;
	}

	किल्ली.dptr = (अचिन्हित अक्षर *)keyname;
	किल्ली.dsize = keylen;

	dbuf = tdb_fetch(tdb, किल्ली);
	जर (!dbuf.dptr) {
	    terror("fetch failed");
	    परत;
	}

	print_rec(tdb, किल्ली, dbuf, रिक्त);

	मुक्त( dbuf.dptr );

	परत;
}

स्थिर शून्य delete_tdb(अक्षर *keyname, माप_प्रकार keylen)
{
	TDB_DATA किल्ली;

	जर ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		परत;
	}

	किल्ली.dptr = (अचिन्हित अक्षर *)keyname;
	किल्ली.dsize = keylen;

	जर (tdb_delete(tdb, किल्ली) != 0) {
		terror("delete failed");
	}
}

स्थिर शून्य move_rec(अक्षर *keyname, माप_प्रकार keylen, अक्षर* tdbname)
{
	TDB_DATA किल्ली, dbuf;
	TDB_CONTEXT *dst_tdb;

	जर ((keyname == रिक्त) || (keylen == 0)) {
		terror("need key");
		परत;
	}

	जर ( !tdbname ) {
		terror("need destination tdb name");
		परत;
	}

	किल्ली.dptr = (अचिन्हित अक्षर *)keyname;
	किल्ली.dsize = keylen;

	dbuf = tdb_fetch(tdb, किल्ली);
	जर (!dbuf.dptr) {
		terror("fetch failed");
		परत;
	}

	print_rec(tdb, किल्ली, dbuf, रिक्त);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	जर ( !dst_tdb ) {
		terror("unable to open destination tdb");
		परत;
	}

	जर ( tdb_store( dst_tdb, किल्ली, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	अन्यथा
		म_लिखो("record moved\n");

	tdb_close( dst_tdb );

	परत;
}

स्थिर पूर्णांक print_rec(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state)
{
	म_लिखो("\nkey %d bytes\n", (पूर्णांक)किल्ली.dsize);
	print_asc((स्थिरांक अक्षर *)किल्ली.dptr, किल्ली.dsize);
	म_लिखो("\ndata %d bytes\n", (पूर्णांक)dbuf.dsize);
	print_data((स्थिरांक अक्षर *)dbuf.dptr, dbuf.dsize);
	परत 0;
}

स्थिर पूर्णांक print_किल्ली(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state)
{
	म_लिखो("key %d bytes: ", (पूर्णांक)किल्ली.dsize);
	print_asc((स्थिरांक अक्षर *)किल्ली.dptr, किल्ली.dsize);
	म_लिखो("\n");
	परत 0;
}

स्थिर पूर्णांक print_hexkey(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state)
{
	म_लिखो("key %d bytes\n", (पूर्णांक)किल्ली.dsize);
	print_data((स्थिरांक अक्षर *)किल्ली.dptr, किल्ली.dsize);
	म_लिखो("\n");
	परत 0;
}

स्थिर पूर्णांक total_bytes;

स्थिर पूर्णांक traverse_fn(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf, शून्य *state)
{
	total_bytes += dbuf.dsize;
	परत 0;
}

स्थिर शून्य info_tdb(शून्य)
{
	पूर्णांक मोजणी;
	total_bytes = 0;
	जर ((मोजणी = tdb_traverse(tdb, traverse_fn, रिक्त)) == -1)
		म_लिखो("Error = %s\n", tdb_errorstr(tdb));
	अन्यथा
		म_लिखो("%d records totalling %d bytes\n", मोजणी, total_bytes);
}

स्थिर अक्षर *tdb_getline(स्थिरांक अक्षर *prompt)
{
	स्थिर अक्षर thisline[1024];
	अक्षर *p;
	ख_माला_दो(prompt, मानक_निकास);
	thisline[0] = 0;
	p = ख_माला_लो(thisline, आकार(thisline)-1, मानक_निवेश);
	जर (p) p = म_अक्षर(p, '\n');
	जर (p) *p = 0;
	परत p?thisline:रिक्त;
}

स्थिर पूर्णांक do_delete_fn(TDB_CONTEXT *the_tdb, TDB_DATA किल्ली, TDB_DATA dbuf,
                     शून्य *state)
{
    परत tdb_delete(the_tdb, किल्ली);
}

स्थिर शून्य first_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	जर (!dbuf.dptr) terror("fetch failed");
	अन्यथा {
		print_rec(the_tdb, *pkey, dbuf, रिक्त);
	}
}

स्थिर शून्य next_record(TDB_CONTEXT *the_tdb, TDB_DATA *pkey)
{
	TDB_DATA dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	जर (!dbuf.dptr)
		terror("fetch failed");
	अन्यथा
		print_rec(the_tdb, *pkey, dbuf, रिक्त);
}

स्थिर पूर्णांक do_command(शून्य)
{
	COMMAND_TABLE *ctp = cmd_table;
	गणना commands mycmd = CMD_HELP;
	पूर्णांक cmd_len;

	जर (cmdname && म_माप(cmdname) == 0) {
	    mycmd = CMD_NEXT;
	} अन्यथा {
	    पर्यंत (ctp->name) {
		cmd_len = म_माप(ctp->name);
		जर (म_भेदन(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			थांबा;
		}
		ctp++;
	    }
	}

	निवड (mycmd) {
	स्थिती CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    परत 0;
	स्थिती CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            परत 0;
	स्थिती CMD_SYSTEM:
	    /* Shell command */
	    प्रणाली(arg1);
	    परत 0;
	स्थिती CMD_QUIT:
	    परत 1;
	पूर्वनिर्धारित:
	    /* all the rest require a open database */
	    जर (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		परत 0;
	    }
	    निवड (mycmd) {
	    स्थिती CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, रिक्त);
		परत 0;
	    स्थिती CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, रिक्त);
		परत 0;
	    स्थिती CMD_घाला:
		bIterate = 0;
		घाला_tdb(arg1, arg1len,arg2,arg2len);
		परत 0;
	    स्थिती CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		परत 0;
	    स्थिती CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		परत 0;
	    स्थिती CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		परत 0;
	    स्थिती CMD_किल्ल्या:
		tdb_traverse(tdb, print_किल्ली, रिक्त);
		परत 0;
	    स्थिती CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, रिक्त);
		परत 0;
	    स्थिती CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		परत 0;
	    स्थिती CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		परत 0;
	    स्थिती CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		परत 0;
	    स्थिती CMD_INFO:
		info_tdb();
		परत 0;
	    स्थिती CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		परत 0;
	    स्थिती CMD_NEXT:
	       जर (bIterate)
		  next_record(tdb, &iterate_kbuf);
		परत 0;
	    स्थिती CMD_HELP:
		help();
		परत 0;
            स्थिती CMD_CREATE_TDB:
            स्थिती CMD_OPEN_TDB:
            स्थिती CMD_SYSTEM:
            स्थिती CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                परत 0;
	    }
	}

	परत 0;
}

स्थिर अक्षर *convert_string(अक्षर *instring, माप_प्रकार *sizep)
{
    माप_प्रकार length = 0;
    अक्षर *outp, *inp;
    अक्षर temp[3];


    outp = inp = instring;

    पर्यंत (*inp) {
	जर (*inp == '\\') {
	    inp++;
	    जर (*inp && म_अक्षर("0123456789abcdefABCDEF",(पूर्णांक)*inp)) {
		temp[0] = *inp++;
		temp[1] = '\0';
		जर (*inp && म_अक्षर("0123456789abcdefABCDEF",(पूर्णांक)*inp)) {
		    temp[1] = *inp++;
		    temp[2] = '\0';
		}
		*outp++ = (अक्षर)म_से_दीर्घ((स्थिरांक अक्षर *)temp,रिक्त,16);
	    } अन्यथा {
		*outp++ = *inp++;
	    }
	} अन्यथा {
	    *outp++ = *inp++;
	}
	length++;
    }
    *sizep = length;
    परत instring;
}

पूर्णांक main(पूर्णांक argc, अक्षर *argv[])
{
    cmdname = "";
    arg1 = रिक्त;
    arg1len = 0;
    arg2 = रिक्त;
    arg2len = 0;

    जर (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = रिक्त;
    }

    निवड (argc) {
	स्थिती 1:
	स्थिती 2:
	    /* Interactive mode */
	    पर्यंत ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = रिक्त;
		जर ((arg1 = म_अक्षर((स्थिरांक अक्षर *)cmdname,' ')) != रिक्त) {
		    arg1++;
		    arg2 = arg1;
		    पर्यंत (*arg2) {
			जर (*arg2 == ' ') {
			    *arg2++ = '\0';
			    थांबा;
			}
			जर ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		जर (arg1) arg1 = convert_string(arg1,&arg1len);
		जर (arg2) arg2 = convert_string(arg2,&arg2len);
		जर (do_command()) थांबा;
	    }
	    थांबा;
	स्थिती 5:
	    arg2 = convert_string(argv[4],&arg2len);
	स्थिती 4:
	    arg1 = convert_string(argv[3],&arg1len);
	स्थिती 3:
	    cmdname = argv[2];
	पूर्वनिर्धारित:
	    do_command();
	    थांबा;
    }

    जर (tdb) tdb_close(tdb);

    परत 0;
}
