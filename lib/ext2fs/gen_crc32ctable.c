#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "crc32c_defs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ENTRIES_ਪ੍ਰਤੀ_ਸਤਰ 4

#ਜੇ ਚੱਕਰੀ_ਜਾਂਚ_LE_ਬਿੱਟ > 8
# define LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ (ਚੱਕਰੀ_ਜਾਂਚ_LE_ਬਿੱਟ/8)
# define LE_ਸਾਰਣੀ_ਮਾਪ 256
#ਨਹੀਂਤਾਂ
# define LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ 1
# define LE_ਸਾਰਣੀ_ਮਾਪ (1 << ਚੱਕਰੀ_ਜਾਂਚ_LE_ਬਿੱਟ)
#ਜੇ_ਅੰਤ

#ਜੇ ਚੱਕਰੀ_ਜਾਂਚ_BE_ਬਿੱਟ > 8
# define BE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ (ਚੱਕਰੀ_ਜਾਂਚ_BE_ਬਿੱਟ/8)
# define BE_ਸਾਰਣੀ_ਮਾਪ 256
#ਨਹੀਂਤਾਂ
# define BE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ 1
# define BE_ਸਾਰਣੀ_ਮਾਪ (1 << ਚੱਕਰੀ_ਜਾਂਚ_BE_ਬਿੱਟ)
#ਜੇ_ਅੰਤ

ਸਥਿਰ uint32_t crc32table_be[BE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ][256];
ਸਥਿਰ uint32_t crc32ctable_le[LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ਸਥਿਰ ਖਾਲੀ crc32cinit_le(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ i, j;
	uint32_t ਚੱਕਰੀ_ਜਾਂਚ = 1;

	crc32ctable_le[0][0] = 0;

	ਲਈ (i = LE_ਸਾਰਣੀ_ਮਾਪ >> 1; i; i >>= 1) {
		ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ >> 1) ^ ((ਚੱਕਰੀ_ਜਾਂਚ & 1) ? ਚੱਕਰੀ_ਜਾਂਚ_32_C_POLY_LE : 0);
		ਲਈ (j = 0; j < LE_ਸਾਰਣੀ_ਮਾਪ; j += 2 * i)
			crc32ctable_le[0][i + j] = ਚੱਕਰੀ_ਜਾਂਚ ^ crc32ctable_le[0][j];
	}
	ਲਈ (i = 0; i < LE_ਸਾਰਣੀ_ਮਾਪ; i++) {
		ਚੱਕਰੀ_ਜਾਂਚ = crc32ctable_le[0][i];
		ਲਈ (j = 1; j < LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ; j++) {
			ਚੱਕਰੀ_ਜਾਂਚ = crc32ctable_le[0][ਚੱਕਰੀ_ਜਾਂਚ & 0xff] ^ (ਚੱਕਰੀ_ਜਾਂਚ >> 8);
			crc32ctable_le[j][i] = ਚੱਕਰੀ_ਜਾਂਚ;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ਸਥਿਰ ਖਾਲੀ crc32init_be(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ i, j;
	uint32_t ਚੱਕਰੀ_ਜਾਂਚ = 0x80000000;

	crc32table_be[0][0] = 0;

	ਲਈ (i = 1; i < BE_ਸਾਰਣੀ_ਮਾਪ; i <<= 1) {
		ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 1) ^ ((ਚੱਕਰੀ_ਜਾਂਚ & 0x80000000) ? CRCPOLY_BE : 0);
		ਲਈ (j = 0; j < i; j++)
			crc32table_be[0][i + j] = ਚੱਕਰੀ_ਜਾਂਚ ^ crc32table_be[0][j];
	}
	ਲਈ (i = 0; i < BE_ਸਾਰਣੀ_ਮਾਪ; i++) {
		ਚੱਕਰੀ_ਜਾਂਚ = crc32table_be[0][i];
		ਲਈ (j = 1; j < BE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ; j++) {
			ਚੱਕਰੀ_ਜਾਂਚ = crc32table_be[0][(ਚੱਕਰੀ_ਜਾਂਚ >> 24) & 0xff] ^ (ਚੱਕਰੀ_ਜਾਂਚ << 8);
			crc32table_be[j][i] = ਚੱਕਰੀ_ਜਾਂਚ;
		}
	}
}

ਸਥਿਰ ਖਾਲੀ output_ਸਾਰਣੀ(uint32_t (*ਸਾਰਣੀ)[256], ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ len, ਅੱਖਰ *trans)
{
	ਪੂਰਨ i, j;

	ਲਈ (j = 0 ; j < ਪੰਕਤੀਆਂ; j++) {
		ਲਿਖੋ("{");
		ਲਈ (i = 0; i < len - 1; i++) {
			ਜੇ (i % ENTRIES_ਪ੍ਰਤੀ_ਸਤਰ == 0)
				ਲਿਖੋ("\n");
			ਲਿਖੋ("%s(0x%8.8xL), ", trans, ਸਾਰਣੀ[j][i]);
		}
		ਲਿਖੋ("%s(0x%8.8xL)},\n", trans, ਸਾਰਣੀ[j][len - 1]);
	}
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਲਿਖੋ("/* this file is generated - do not edit */\n\n");

	ਜੇ (ਚੱਕਰੀ_ਜਾਂਚ_BE_ਬਿੱਟ > 1) {
		crc32init_be();
		ਲਿਖੋ("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ, BE_ਸਾਰਣੀ_ਮਾਪ);
		output_ਸਾਰਣੀ(crc32table_be, LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ,
			     BE_ਸਾਰਣੀ_ਮਾਪ, "tobe");
		ਲਿਖੋ("};\n");
	}
	ਜੇ (ਚੱਕਰੀ_ਜਾਂਚ_LE_ਬਿੱਟ > 1) {
		crc32cinit_le();
		ਲਿਖੋ("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ, LE_ਸਾਰਣੀ_ਮਾਪ);
		output_ਸਾਰਣੀ(crc32ctable_le, LE_ਸਾਰਣੀ_ਪੰਕਤੀਆਂ,
			     LE_ਸਾਰਣੀ_ਮਾਪ, "tole");
		ਲਿਖੋ("};\n");
	}

	ਵਾਪਸ 0;
}
