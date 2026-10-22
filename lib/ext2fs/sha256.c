/*
 * sha256.c --- The sh256 algorithm
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 * (copied from libtomcrypt and then relicensed under GPLv2)
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */


#समावेश "config.h"
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#समावेश "ext2fs.h"

अटल स्थिर __u32 K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
    0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
    0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
    0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
    0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
    0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
    0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
    0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
    0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Various logical functions */
#घोषणा Ch(x,y,z)       (z ^ (x & (y ^ z)))
#घोषणा Maj(x,y,z)      (((x | y) & z) | (x & y)) 
#घोषणा S(x, n)         RORc((x),(n))
#घोषणा R(x, n)         (((x)&0xFFFFFFFFUL)>>(n))
#घोषणा Sigma0(x)       (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#घोषणा Sigma1(x)       (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#घोषणा Gamma0(x)       (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#घोषणा Gamma1(x)       (S(x, 17) ^ S(x, 19) ^ R(x, 10))
#घोषणा RORc(x, y) ( ((((__u32)(x)&0xFFFFFFFFUL)>>(__u32)((y)&31)) | ((__u32)(x)<<(__u32)(32-((y)&31)))) & 0xFFFFFFFFUL)

#घोषणा RND(a,b,c,d,e,f,g,h,i)                         \
     t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];   \
     t1 = Sigma0(a) + Maj(a, b, c);                    \
     d += t0;                                          \
     h  = t0 + t1;

#घोषणा STORE64H(x, y) \
	करो { \
		(y)[0] = (अचिन्हित अक्षर)(((x)>>56)&255);\
		(y)[1] = (अचिन्हित अक्षर)(((x)>>48)&255);\
		(y)[2] = (अचिन्हित अक्षर)(((x)>>40)&255);\
		(y)[3] = (अचिन्हित अक्षर)(((x)>>32)&255);\
		(y)[4] = (अचिन्हित अक्षर)(((x)>>24)&255);\
		(y)[5] = (अचिन्हित अक्षर)(((x)>>16)&255);\
		(y)[6] = (अचिन्हित अक्षर)(((x)>>8)&255);\
		(y)[7] = (अचिन्हित अक्षर)((x)&255); } जबतक(0)

#घोषणा STORE32H(x, y)                                                                     \
  करो { (y)[0] = (अचिन्हित अक्षर)(((x)>>24)&255); (y)[1] = (अचिन्हित अक्षर)(((x)>>16)&255);   \
       (y)[2] = (अचिन्हित अक्षर)(((x)>>8)&255); (y)[3] = (अचिन्हित अक्षर)((x)&255); } जबतक(0)

#घोषणा LOAD32H(x, y)                            \
  करो { x = ((__u32)((y)[0] & 255)<<24) | \
           ((__u32)((y)[1] & 255)<<16) | \
           ((__u32)((y)[2] & 255)<<8)  | \
           ((__u32)((y)[3] & 255)); } जबतक(0)

काष्ठा sha256_state {
    __u64 लंबाई;
    __u32 state[8], curlen;
    अचिन्हित अक्षर buf[64];
};

/* This is a highly simplified version from libtomcrypt */
काष्ठा hash_state {
	काष्ठा sha256_state sha256;
};

अटल व्योम sha256_compress(काष्ठा hash_state * md, स्थिर अचिन्हित अक्षर *buf)
{
    __u32 S[8], W[64], t0, t1;
    __u32 t;
    पूर्णांक i;

    /* copy state into S */
    क्रम (i = 0; i < 8; i++) {
        S[i] = md->sha256.state[i];
    }

    /* copy the state into 512-bits into W[0..15] */
    क्रम (i = 0; i < 16; i++) {
        LOAD32H(W[i], buf + (4*i));
    }

    /* fill W[16..63] */
    क्रम (i = 16; i < 64; i++) {
        W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
    }        

    /* Compress */
     क्रम (i = 0; i < 64; ++i) {
         RND(S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7],i);
         t = S[7]; S[7] = S[6]; S[6] = S[5]; S[5] = S[4]; 
         S[4] = S[3]; S[3] = S[2]; S[2] = S[1]; S[1] = S[0]; S[0] = t;
     }  

    /* feedback */
    क्रम (i = 0; i < 8; i++) {
        md->sha256.state[i] = md->sha256.state[i] + S[i];
    }
}

अटल व्योम sha256_init(काष्ठा hash_state * md)
{
    md->sha256.curlen = 0;
    md->sha256.लंबाई = 0;
    md->sha256.state[0] = 0x6A09E667UL;
    md->sha256.state[1] = 0xBB67AE85UL;
    md->sha256.state[2] = 0x3C6EF372UL;
    md->sha256.state[3] = 0xA54FF53AUL;
    md->sha256.state[4] = 0x510E527FUL;
    md->sha256.state[5] = 0x9B05688CUL;
    md->sha256.state[6] = 0x1F83D9ABUL;
    md->sha256.state[7] = 0x5BE0CD19UL;
}

#घोषणा MIN(x, y) ( ((x)<(y))?(x):(y) )
#घोषणा SHA256_BLOCKSIZE 64
अटल व्योम sha256_process(काष्ठा hash_state * md, स्थिर अचिन्हित अक्षर *in, अचिन्हित दीर्घ inlen)
{
    अचिन्हित दीर्घ n;

    जबतक (inlen > 0) {
	    अगर (md->sha256.curlen == 0 && inlen >= SHA256_BLOCKSIZE) {
		    sha256_compress(md, in);
		    md->sha256.लंबाई += SHA256_BLOCKSIZE * 8;
		    in += SHA256_BLOCKSIZE;
		    inlen -= SHA256_BLOCKSIZE;
	    } अन्यथा {
		    n = MIN(inlen, (SHA256_BLOCKSIZE - md->sha256.curlen));
		    स_नकल(md->sha256.buf + md->sha256.curlen, in, (माप_प्रकार)n);
		    md->sha256.curlen += n;
		    in += n;
		    inlen -= n;
		    अगर (md->sha256.curlen == SHA256_BLOCKSIZE) {
			    sha256_compress(md, md->sha256.buf);
			    md->sha256.लंबाई += 8*SHA256_BLOCKSIZE;
			    md->sha256.curlen = 0;
		    }
	    }
    }
}


अटल व्योम sha256_done(काष्ठा hash_state * md, अचिन्हित अक्षर *out)
{
    पूर्णांक i;

    /* increase the length of the message */
    md->sha256.लंबाई += md->sha256.curlen * 8;

    /* append the '1' bit */
    md->sha256.buf[md->sha256.curlen++] = (अचिन्हित अक्षर)0x80;

    /* if the length is currently above 56 bytes we append zeros
     * then compress.  Then we can fall back to padding zeros and length
     * encoding like normal.
     */
    अगर (md->sha256.curlen > 56) {
        जबतक (md->sha256.curlen < 64) {
            md->sha256.buf[md->sha256.curlen++] = (अचिन्हित अक्षर)0;
        }
        sha256_compress(md, md->sha256.buf);
        md->sha256.curlen = 0;
    }

    /* pad up to 56 bytes of zeroes */
    जबतक (md->sha256.curlen < 56) {
        md->sha256.buf[md->sha256.curlen++] = (अचिन्हित अक्षर)0;
    }

    /* store length */
    STORE64H(md->sha256.लंबाई, md->sha256.buf+56);
    sha256_compress(md, md->sha256.buf);

    /* copy output */
    क्रम (i = 0; i < 8; i++) {
        STORE32H(md->sha256.state[i], out+(4*i));
    }
}

व्योम ext2fs_sha256(स्थिर अचिन्हित अक्षर *in, अचिन्हित दीर्घ in_आकार,
		   अचिन्हित अक्षर out[EXT2_FS_SHA256_लंबाई])
{
	काष्ठा hash_state md;

	sha256_init(&md);
	sha256_process(&md, in, in_आकार);
	sha256_done(&md, out);
}

#अगर_घोषित UNITTEST
अटल स्थिर काष्ठा {
	अक्षर *संदेश;
	अचिन्हित अक्षर hash[32];
} tests[] = {
	{ "",
	  { 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
	    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
	    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
	    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 }
	},
	{ "abc",
	  { 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
	    0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
	    0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
	    0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad }
	},
	{ "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	  { 0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8,
	    0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39,
	    0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67,
	    0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1 }
	},
};

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक i;
	पूर्णांक errors = 0;
	अचिन्हित अक्षर tmp[32];

	क्रम (i = 0; i < (पूर्णांक)(माप(tests) / माप(tests[0])); i++) {
		अचिन्हित अक्षर *संदेश = (अचिन्हित अक्षर *) tests[i].संदेश;
		पूर्णांक len = म_माप(tests[i].संदेश);

		ext2fs_sha256(संदेश, len, tmp);
		म_लिखो("SHA256 test message %d: ", i);
		अगर (स_भेद(tmp, tests[i].hash, 32) != 0) {
			म_लिखो("FAILED\n");
			errors++;
		} अन्यथा
			म_लिखो("OK\n");
	}
	वापस errors;
}

#पूर्ण_अगर /* UNITTEST */
