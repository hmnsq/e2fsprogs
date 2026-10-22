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


#شامل "config.h"
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#شامل "ext2fs.h"

ساکن ثابت __u32 K[64] = {
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
#تعریف_کرو Ch(x,y,z)       (z ^ (x & (y ^ z)))
#تعریف_کرو Maj(x,y,z)      (((x | y) & z) | (x & y)) 
#تعریف_کرو S(x, n)         RORc((x),(n))
#تعریف_کرو R(x, n)         (((x)&0xFFFFFFFFUL)>>(n))
#تعریف_کرو Sigma0(x)       (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#تعریف_کرو Sigma1(x)       (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#تعریف_کرو Gamma0(x)       (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#تعریف_کرو Gamma1(x)       (S(x, 17) ^ S(x, 19) ^ R(x, 10))
#تعریف_کرو RORc(x, y) ( ((((__u32)(x)&0xFFFFFFFFUL)>>(__u32)((y)&31)) | ((__u32)(x)<<(__u32)(32-((y)&31)))) & 0xFFFFFFFFUL)

#تعریف_کرو RND(a,b,c,d,e,f,g,h,i)                         \
     t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];   \
     t1 = Sigma0(a) + Maj(a, b, c);                    \
     d += t0;                                          \
     h  = t0 + t1;

#تعریف_کرو STORE64H(x, y) \
	کرو { \
		(y)[0] = (غیرعلامتی حرف)(((x)>>56)&255);\
		(y)[1] = (غیرعلامتی حرف)(((x)>>48)&255);\
		(y)[2] = (غیرعلامتی حرف)(((x)>>40)&255);\
		(y)[3] = (غیرعلامتی حرف)(((x)>>32)&255);\
		(y)[4] = (غیرعلامتی حرف)(((x)>>24)&255);\
		(y)[5] = (غیرعلامتی حرف)(((x)>>16)&255);\
		(y)[6] = (غیرعلامتی حرف)(((x)>>8)&255);\
		(y)[7] = (غیرعلامتی حرف)((x)&255); } جب(0)

#تعریف_کرو STORE32H(x, y)                                                                     \
  کرو { (y)[0] = (غیرعلامتی حرف)(((x)>>24)&255); (y)[1] = (غیرعلامتی حرف)(((x)>>16)&255);   \
       (y)[2] = (غیرعلامتی حرف)(((x)>>8)&255); (y)[3] = (غیرعلامتی حرف)((x)&255); } جب(0)

#تعریف_کرو LOAD32H(x, y)                            \
  کرو { x = ((__u32)((y)[0] & 255)<<24) | \
           ((__u32)((y)[1] & 255)<<16) | \
           ((__u32)((y)[2] & 255)<<8)  | \
           ((__u32)((y)[3] & 255)); } جب(0)

ساخت sha256_state {
    __u64 لمبائی;
    __u32 state[8], curlen;
    غیرعلامتی حرف buf[64];
};

/* This is a highly simplified version from libtomcrypt */
ساخت hash_state {
	ساخت sha256_state sha256;
};

ساکن خالی sha256_compress(ساخت hash_state * md, ثابت غیرعلامتی حرف *buf)
{
    __u32 S[8], W[64], t0, t1;
    __u32 t;
    صحیح i;

    /* copy state into S */
    برائے (i = 0; i < 8; i++) {
        S[i] = md->sha256.state[i];
    }

    /* copy the state into 512-bits into W[0..15] */
    برائے (i = 0; i < 16; i++) {
        LOAD32H(W[i], buf + (4*i));
    }

    /* fill W[16..63] */
    برائے (i = 16; i < 64; i++) {
        W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
    }        

    /* Compress */
     برائے (i = 0; i < 64; ++i) {
         RND(S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7],i);
         t = S[7]; S[7] = S[6]; S[6] = S[5]; S[5] = S[4]; 
         S[4] = S[3]; S[3] = S[2]; S[2] = S[1]; S[1] = S[0]; S[0] = t;
     }  

    /* feedback */
    برائے (i = 0; i < 8; i++) {
        md->sha256.state[i] = md->sha256.state[i] + S[i];
    }
}

ساکن خالی sha256_init(ساخت hash_state * md)
{
    md->sha256.curlen = 0;
    md->sha256.لمبائی = 0;
    md->sha256.state[0] = 0x6A09E667UL;
    md->sha256.state[1] = 0xBB67AE85UL;
    md->sha256.state[2] = 0x3C6EF372UL;
    md->sha256.state[3] = 0xA54FF53AUL;
    md->sha256.state[4] = 0x510E527FUL;
    md->sha256.state[5] = 0x9B05688CUL;
    md->sha256.state[6] = 0x1F83D9ABUL;
    md->sha256.state[7] = 0x5BE0CD19UL;
}

#تعریف_کرو MIN(x, y) ( ((x)<(y))?(x):(y) )
#تعریف_کرو SHA256_BLOCKSIZE 64
ساکن خالی sha256_process(ساخت hash_state * md, ثابت غیرعلامتی حرف *اندر, غیرعلامتی طویل inlen)
{
    غیرعلامتی طویل n;

    جب (inlen > 0) {
	    اگر (md->sha256.curlen == 0 && inlen >= SHA256_BLOCKSIZE) {
		    sha256_compress(md, اندر);
		    md->sha256.لمبائی += SHA256_BLOCKSIZE * 8;
		    اندر += SHA256_BLOCKSIZE;
		    inlen -= SHA256_BLOCKSIZE;
	    } ورنہ {
		    n = MIN(inlen, (SHA256_BLOCKSIZE - md->sha256.curlen));
		    حافظہ_نقل(md->sha256.buf + md->sha256.curlen, اندر, (size_t)n);
		    md->sha256.curlen += n;
		    اندر += n;
		    inlen -= n;
		    اگر (md->sha256.curlen == SHA256_BLOCKSIZE) {
			    sha256_compress(md, md->sha256.buf);
			    md->sha256.لمبائی += 8*SHA256_BLOCKSIZE;
			    md->sha256.curlen = 0;
		    }
	    }
    }
}


ساکن خالی sha256_مکمل(ساخت hash_state * md, غیرعلامتی حرف *برآمد)
{
    صحیح i;

    /* increase the length of the message */
    md->sha256.لمبائی += md->sha256.curlen * 8;

    /* append the '1' bit */
    md->sha256.buf[md->sha256.curlen++] = (غیرعلامتی حرف)0x80;

    /* if the length is currently above 56 bytes we append zeros
     * then compress.  Then we can fall back to padding zeros and length
     * encoding like normal.
     */
    اگر (md->sha256.curlen > 56) {
        جب (md->sha256.curlen < 64) {
            md->sha256.buf[md->sha256.curlen++] = (غیرعلامتی حرف)0;
        }
        sha256_compress(md, md->sha256.buf);
        md->sha256.curlen = 0;
    }

    /* pad up to 56 bytes of zeroes */
    جب (md->sha256.curlen < 56) {
        md->sha256.buf[md->sha256.curlen++] = (غیرعلامتی حرف)0;
    }

    /* store length */
    STORE64H(md->sha256.لمبائی, md->sha256.buf+56);
    sha256_compress(md, md->sha256.buf);

    /* copy output */
    برائے (i = 0; i < 8; i++) {
        STORE32H(md->sha256.state[i], برآمد+(4*i));
    }
}

خالی ext2fs_sha256(ثابت غیرعلامتی حرف *اندر, غیرعلامتی طویل اندر_سائز,
		   غیرعلامتی حرف برآمد[EXT2_FS_SHA256_لمبائی])
{
	ساخت hash_state md;

	sha256_init(&md);
	sha256_process(&md, اندر, اندر_سائز);
	sha256_مکمل(&md, برآمد);
}

#اگر_تعریف_شدہ UNITTEST
ساکن ثابت ساخت {
	حرف *پیغام;
	غیرعلامتی حرف hash[32];
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

صحیح مین(صحیح argc, حرف **argv)
{
	صحیح i;
	صحیح errors = 0;
	غیرعلامتی حرف tmp[32];

	برائے (i = 0; i < (صحیح)(حجم(tests) / حجم(tests[0])); i++) {
		غیرعلامتی حرف *پیغام = (غیرعلامتی حرف *) tests[i].پیغام;
		صحیح len = لڑی_لمبائی(tests[i].پیغام);

		ext2fs_sha256(پیغام, len, tmp);
		لکھو("SHA256 test message %d: ", i);
		اگر (memcmp(tmp, tests[i].hash, 32) != 0) {
			لکھو("FAILED\n");
			errors++;
		} ورنہ
			لکھو("OK\n");
	}
	واپس errors;
}

#اگر_ختم /* UNITTEST */
