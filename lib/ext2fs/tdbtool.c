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

#समावेश "config.h"
#समावेश <errno.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <fcntl.h>
#समावेश <unistd.h>
#समावेश <string.h>
#समावेश <fcntl.h>
#समावेश <time.h>
#समावेश <sys/mman.h>
#समावेश <sys/stat.h>
#समावेश <sys/time.h>
#समावेश <ctype.h>
#समावेश <signal.h>
#समावेश <stdarg.h>

#समावेश "tdb.h"

अटल पूर्णांक do_command(व्योम);
स्थिर अक्षर *cmdname;
अक्षर *arg1, *arg2;
माप_प्रकार arg1len, arg2len;
पूर्णांक bIterate = 0;
अक्षर *पंक्ति;
TDB_तथ्य iterate_kbuf;
अक्षर cmdline[1024];

क्रमागत commands {
	CMD_CREATE_TDB,
	CMD_OPEN_TDB,
	CMD_ERASE,
	CMD_DUMP,
	CMD_जोड़ो,
	CMD_MOVE,
	CMD_STORE,
	CMD_SHOW,
	CMD_कुंजियाँ,
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

प्रकार काष्ठा {
	स्थिर अक्षर *name;
	क्रमागत commands cmd;
} COMMAND_TABLE;

COMMAND_TABLE cmd_table[] = {
	{"create",	CMD_CREATE_TDB},
	{"open",	CMD_OPEN_TDB},
	{"erase",	CMD_ERASE},
	{"dump",	CMD_DUMP},
	{"insert",	CMD_जोड़ो},
	{"move",	CMD_MOVE},
	{"store",	CMD_STORE},
	{"show",	CMD_SHOW},
	{"keys",	CMD_कुंजियाँ},
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
	{शून्य,		CMD_HELP}
};

/* a tdb tool for manipulating a tdb database */

अटल TDB_CONTEXT *tdb;

अटल पूर्णांक print_rec(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state);
अटल पूर्णांक print_कुंजी(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state);
अटल पूर्णांक print_hexkey(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state);

अटल व्योम print_asc(स्थिर अक्षर *buf,पूर्णांक len)
{
	पूर्णांक i;

	/* We're probably printing ASCII strings so don't try to display
	   the trailing NULL character. */

	अगर (buf[len - 1] == 0)
	        len--;

	क्रम (i=0;i<len;i++)
		म_लिखो("%c",है_छाप(buf[i])?buf[i]:'.');
}

अटल व्योम print_तथ्य(स्थिर अक्षर *buf,पूर्णांक len)
{
	पूर्णांक i=0;
	अगर (len<=0) वापस;
	म_लिखो("[%03X] ",i);
	क्रम (i=0;i<len;) {
		म_लिखो("%02X ",(पूर्णांक)buf[i]);
		i++;
		अगर (i%8 == 0) म_लिखो(" ");
		अगर (i%16 == 0) {
			print_asc(&buf[i-16],8); म_लिखो(" ");
			print_asc(&buf[i-8],8); म_लिखो("\n");
			अगर (i<len) म_लिखो("[%03X] ",i);
		}
	}
	अगर (i%16) {
		पूर्णांक n;

		n = 16 - (i%16);
		म_लिखो(" ");
		अगर (n>8) म_लिखो(" ");
		जबतक (n--) म_लिखो("   ");

		n = i%16;
		अगर (n > 8) n = 8;
		print_asc(&buf[i-(i%16)],n); म_लिखो(" ");
		n = (i%16) - n;
		अगर (n>0) print_asc(&buf[i-n],n);
		म_लिखो("\n");
	}
}

अटल व्योम help(व्योम)
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

अटल व्योम terror(स्थिर अक्षर *why)
{
	म_लिखो("%s\n", why);
}

अटल व्योम create_tdb(स्थिर अक्षर *tdbname)
{
	अगर (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, TDB_CLEAR_IF_FIRST,
		       O_RDWR | O_CREAT | O_TRUNC, 0600);
	अगर (!tdb) {
		म_लिखो("Could not create %s: %s\n", tdbname, म_त्रुटि(त्रुटि_सं));
	}
}

अटल व्योम open_tdb(स्थिर अक्षर *tdbname)
{
	अगर (tdb) tdb_close(tdb);
	tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	अगर (!tdb) {
		म_लिखो("Could not open %s: %s\n", tdbname, म_त्रुटि(त्रुटि_सं));
	}
}

अटल व्योम जोड़ो_tdb(अक्षर *keyname, माप_प्रकार keylen, अक्षर* तथ्य, माप_प्रकार datalen)
{
	TDB_तथ्य कुंजी, dbuf;

	अगर ((keyname == शून्य) || (keylen == 0)) {
		terror("need key");
		वापस;
	}

	कुंजी.dptr = (अचिन्हित अक्षर *)keyname;
	कुंजी.dsize = keylen;
	dbuf.dptr = (अचिन्हित अक्षर *)तथ्य;
	dbuf.dsize = datalen;

	अगर (tdb_store(tdb, कुंजी, dbuf, TDB_जोड़ो) == -1) {
		terror("insert failed");
	}
}

अटल व्योम store_tdb(अक्षर *keyname, माप_प्रकार keylen, अक्षर* तथ्य, माप_प्रकार datalen)
{
	TDB_तथ्य कुंजी, dbuf;

	अगर ((keyname == शून्य) || (keylen == 0)) {
		terror("need key");
		वापस;
	}

	अगर ((तथ्य == शून्य) || (datalen == 0)) {
		terror("need data");
		वापस;
	}

	कुंजी.dptr = (अचिन्हित अक्षर *)keyname;
	कुंजी.dsize = keylen;
	dbuf.dptr = (अचिन्हित अक्षर *)तथ्य;
	dbuf.dsize = datalen;

	म_लिखो("Storing key:\n");
	print_rec(tdb, कुंजी, dbuf, शून्य);

	अगर (tdb_store(tdb, कुंजी, dbuf, TDB_REPLACE) == -1) {
		terror("store failed");
	}
}

अटल व्योम show_tdb(अक्षर *keyname, माप_प्रकार keylen)
{
	TDB_तथ्य कुंजी, dbuf;

	अगर ((keyname == शून्य) || (keylen == 0)) {
		terror("need key");
		वापस;
	}

	कुंजी.dptr = (अचिन्हित अक्षर *)keyname;
	कुंजी.dsize = keylen;

	dbuf = tdb_fetch(tdb, कुंजी);
	अगर (!dbuf.dptr) {
	    terror("fetch failed");
	    वापस;
	}

	print_rec(tdb, कुंजी, dbuf, शून्य);

	मुक्त( dbuf.dptr );

	वापस;
}

अटल व्योम delete_tdb(अक्षर *keyname, माप_प्रकार keylen)
{
	TDB_तथ्य कुंजी;

	अगर ((keyname == शून्य) || (keylen == 0)) {
		terror("need key");
		वापस;
	}

	कुंजी.dptr = (अचिन्हित अक्षर *)keyname;
	कुंजी.dsize = keylen;

	अगर (tdb_delete(tdb, कुंजी) != 0) {
		terror("delete failed");
	}
}

अटल व्योम move_rec(अक्षर *keyname, माप_प्रकार keylen, अक्षर* tdbname)
{
	TDB_तथ्य कुंजी, dbuf;
	TDB_CONTEXT *dst_tdb;

	अगर ((keyname == शून्य) || (keylen == 0)) {
		terror("need key");
		वापस;
	}

	अगर ( !tdbname ) {
		terror("need destination tdb name");
		वापस;
	}

	कुंजी.dptr = (अचिन्हित अक्षर *)keyname;
	कुंजी.dsize = keylen;

	dbuf = tdb_fetch(tdb, कुंजी);
	अगर (!dbuf.dptr) {
		terror("fetch failed");
		वापस;
	}

	print_rec(tdb, कुंजी, dbuf, शून्य);

	dst_tdb = tdb_open(tdbname, 0, 0, O_RDWR, 0600);
	अगर ( !dst_tdb ) {
		terror("unable to open destination tdb");
		वापस;
	}

	अगर ( tdb_store( dst_tdb, कुंजी, dbuf, TDB_REPLACE ) == -1 ) {
		terror("failed to move record");
	}
	अन्यथा
		म_लिखो("record moved\n");

	tdb_close( dst_tdb );

	वापस;
}

अटल पूर्णांक print_rec(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state)
{
	म_लिखो("\nkey %d bytes\n", (पूर्णांक)कुंजी.dsize);
	print_asc((स्थिर अक्षर *)कुंजी.dptr, कुंजी.dsize);
	म_लिखो("\ndata %d bytes\n", (पूर्णांक)dbuf.dsize);
	print_तथ्य((स्थिर अक्षर *)dbuf.dptr, dbuf.dsize);
	वापस 0;
}

अटल पूर्णांक print_कुंजी(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state)
{
	म_लिखो("key %d bytes: ", (पूर्णांक)कुंजी.dsize);
	print_asc((स्थिर अक्षर *)कुंजी.dptr, कुंजी.dsize);
	म_लिखो("\n");
	वापस 0;
}

अटल पूर्णांक print_hexkey(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state)
{
	म_लिखो("key %d bytes\n", (पूर्णांक)कुंजी.dsize);
	print_तथ्य((स्थिर अक्षर *)कुंजी.dptr, कुंजी.dsize);
	म_लिखो("\n");
	वापस 0;
}

अटल पूर्णांक योग_bytes;

अटल पूर्णांक traverse_fn(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf, व्योम *state)
{
	योग_bytes += dbuf.dsize;
	वापस 0;
}

अटल व्योम info_tdb(व्योम)
{
	पूर्णांक गिनती;
	योग_bytes = 0;
	अगर ((गिनती = tdb_traverse(tdb, traverse_fn, शून्य)) == -1)
		म_लिखो("Error = %s\n", tdb_errorstr(tdb));
	अन्यथा
		म_लिखो("%d records totalling %d bytes\n", गिनती, योग_bytes);
}

अटल अक्षर *tdb_getline(स्थिर अक्षर *prompt)
{
	अटल अक्षर thisline[1024];
	अक्षर *p;
	ख_माला_दो(prompt, मानक_निकास);
	thisline[0] = 0;
	p = ख_माला_लो(thisline, माप(thisline)-1, मानक_निवेश);
	अगर (p) p = म_अक्षर(p, '\n');
	अगर (p) *p = 0;
	वापस p?thisline:शून्य;
}

अटल पूर्णांक do_delete_fn(TDB_CONTEXT *the_tdb, TDB_तथ्य कुंजी, TDB_तथ्य dbuf,
                     व्योम *state)
{
    वापस tdb_delete(the_tdb, कुंजी);
}

अटल व्योम first_record(TDB_CONTEXT *the_tdb, TDB_तथ्य *pkey)
{
	TDB_तथ्य dbuf;
	*pkey = tdb_firstkey(the_tdb);

	dbuf = tdb_fetch(the_tdb, *pkey);
	अगर (!dbuf.dptr) terror("fetch failed");
	अन्यथा {
		print_rec(the_tdb, *pkey, dbuf, शून्य);
	}
}

अटल व्योम अग्र_record(TDB_CONTEXT *the_tdb, TDB_तथ्य *pkey)
{
	TDB_तथ्य dbuf;
	*pkey = tdb_nextkey(the_tdb, *pkey);

	dbuf = tdb_fetch(the_tdb, *pkey);
	अगर (!dbuf.dptr)
		terror("fetch failed");
	अन्यथा
		print_rec(the_tdb, *pkey, dbuf, शून्य);
}

अटल पूर्णांक do_command(व्योम)
{
	COMMAND_TABLE *ctp = cmd_table;
	क्रमागत commands mycmd = CMD_HELP;
	पूर्णांक cmd_len;

	अगर (cmdname && म_माप(cmdname) == 0) {
	    mycmd = CMD_अग्र;
	} अन्यथा {
	    जबतक (ctp->name) {
		cmd_len = म_माप(ctp->name);
		अगर (म_भेदन(ctp->name,cmdname,cmd_len) == 0) {
			mycmd = ctp->cmd;
			अवरोध;
		}
		ctp++;
	    }
	}

	चयन (mycmd) {
	हाल CMD_CREATE_TDB:
            bIterate = 0;
            create_tdb(arg1);
	    वापस 0;
	हाल CMD_OPEN_TDB:
            bIterate = 0;
            open_tdb(arg1);
            वापस 0;
	हाल CMD_SYSTEM:
	    /* Shell command */
	    प्रणाली(arg1);
	    वापस 0;
	हाल CMD_QUIT:
	    वापस 1;
	शेष:
	    /* all the rest require a open database */
	    अगर (!tdb) {
		bIterate = 0;
		terror("database not open");
		help();
		वापस 0;
	    }
	    चयन (mycmd) {
	    हाल CMD_ERASE:
		bIterate = 0;
		tdb_traverse(tdb, do_delete_fn, शून्य);
		वापस 0;
	    हाल CMD_DUMP:
		bIterate = 0;
		tdb_traverse(tdb, print_rec, शून्य);
		वापस 0;
	    हाल CMD_जोड़ो:
		bIterate = 0;
		जोड़ो_tdb(arg1, arg1len,arg2,arg2len);
		वापस 0;
	    हाल CMD_MOVE:
		bIterate = 0;
		move_rec(arg1,arg1len,arg2);
		वापस 0;
	    हाल CMD_STORE:
		bIterate = 0;
		store_tdb(arg1,arg1len,arg2,arg2len);
		वापस 0;
	    हाल CMD_SHOW:
		bIterate = 0;
		show_tdb(arg1, arg1len);
		वापस 0;
	    हाल CMD_कुंजियाँ:
		tdb_traverse(tdb, print_कुंजी, शून्य);
		वापस 0;
	    हाल CMD_HEXKEYS:
		tdb_traverse(tdb, print_hexkey, शून्य);
		वापस 0;
	    हाल CMD_DELETE:
		bIterate = 0;
		delete_tdb(arg1,arg1len);
		वापस 0;
	    हाल CMD_LIST_HASH_FREE:
		tdb_dump_all(tdb);
		वापस 0;
	    हाल CMD_LIST_FREE:
		tdb_printfreelist(tdb);
		वापस 0;
	    हाल CMD_INFO:
		info_tdb();
		वापस 0;
	    हाल CMD_FIRST:
		bIterate = 1;
		first_record(tdb, &iterate_kbuf);
		वापस 0;
	    हाल CMD_अग्र:
	       अगर (bIterate)
		  अग्र_record(tdb, &iterate_kbuf);
		वापस 0;
	    हाल CMD_HELP:
		help();
		वापस 0;
            हाल CMD_CREATE_TDB:
            हाल CMD_OPEN_TDB:
            हाल CMD_SYSTEM:
            हाल CMD_QUIT:
                /*
                 * unhandled commands.  cases included here to avoid compiler
                 * warnings.
                 */
                वापस 0;
	    }
	}

	वापस 0;
}

अटल अक्षर *convert_string(अक्षर *instring, माप_प्रकार *sizep)
{
    माप_प्रकार लंबाई = 0;
    अक्षर *outp, *inp;
    अक्षर अस्थायी[3];


    outp = inp = instring;

    जबतक (*inp) {
	अगर (*inp == '\\') {
	    inp++;
	    अगर (*inp && म_अक्षर("0123456789abcdefABCDEF",(पूर्णांक)*inp)) {
		अस्थायी[0] = *inp++;
		अस्थायी[1] = '\0';
		अगर (*inp && म_अक्षर("0123456789abcdefABCDEF",(पूर्णांक)*inp)) {
		    अस्थायी[1] = *inp++;
		    अस्थायी[2] = '\0';
		}
		*outp++ = (अक्षर)म_से_दीर्घ((स्थिर अक्षर *)अस्थायी,शून्य,16);
	    } अन्यथा {
		*outp++ = *inp++;
	    }
	} अन्यथा {
	    *outp++ = *inp++;
	}
	लंबाई++;
    }
    *sizep = लंबाई;
    वापस instring;
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर *argv[])
{
    cmdname = "";
    arg1 = शून्य;
    arg1len = 0;
    arg2 = शून्य;
    arg2len = 0;

    अगर (argv[1]) {
	cmdname = "open";
	arg1 = argv[1];
        do_command();
	cmdname =  "";
	arg1 = शून्य;
    }

    चयन (argc) {
	हाल 1:
	हाल 2:
	    /* Interactive mode */
	    जबतक ((cmdname = tdb_getline("tdb> "))) {
		arg2 = arg1 = शून्य;
		अगर ((arg1 = म_अक्षर((स्थिर अक्षर *)cmdname,' ')) != शून्य) {
		    arg1++;
		    arg2 = arg1;
		    जबतक (*arg2) {
			अगर (*arg2 == ' ') {
			    *arg2++ = '\0';
			    अवरोध;
			}
			अगर ((*arg2++ == '\\') && (*arg2 == ' ')) {
			    arg2++;
			}
		    }
		}
		अगर (arg1) arg1 = convert_string(arg1,&arg1len);
		अगर (arg2) arg2 = convert_string(arg2,&arg2len);
		अगर (do_command()) अवरोध;
	    }
	    अवरोध;
	हाल 5:
	    arg2 = convert_string(argv[4],&arg2len);
	हाल 4:
	    arg1 = convert_string(argv[3],&arg1len);
	हाल 3:
	    cmdname = argv[2];
	शेष:
	    do_command();
	    अवरोध;
    }

    अगर (tdb) tdb_close(tdb);

    वापस 0;
}
