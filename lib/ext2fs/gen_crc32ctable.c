#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "crc32c_defs.h"
#അന്തര്ഭാവ <inttypes.h>

#define ENTRIES_PER_വരി 4

#എങ്കിൽ CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_SIZE 256
#അല്ലെങ്കിൽ
# define LE_TABLE_ROWS 1
# define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#endif

#എങ്കിൽ CRC_BE_BITS > 8
# define BE_TABLE_ROWS (CRC_BE_BITS/8)
# define BE_TABLE_SIZE 256
#അല്ലെങ്കിൽ
# define BE_TABLE_ROWS 1
# define BE_TABLE_SIZE (1 << CRC_BE_BITS)
#endif

സ്ഥിതം uint32_t crc32table_be[BE_TABLE_ROWS][256];
സ്ഥിതം uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
സ്ഥിതം ശൂന്യം crc32cinit_le(ശൂന്യം)
{
	unsigned i, j;
	uint32_t crc = 1;

	crc32ctable_le[0][0] = 0;

	വേണ്ടി (i = LE_TABLE_SIZE >> 1; i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? CRC32C_POLY_LE : 0);
		വേണ്ടി (j = 0; j < LE_TABLE_SIZE; j += 2 * i)
			crc32ctable_le[0][i + j] = crc ^ crc32ctable_le[0][j];
	}
	വേണ്ടി (i = 0; i < LE_TABLE_SIZE; i++) {
		crc = crc32ctable_le[0][i];
		വേണ്ടി (j = 1; j < LE_TABLE_ROWS; j++) {
			crc = crc32ctable_le[0][crc & 0xff] ^ (crc >> 8);
			crc32ctable_le[j][i] = crc;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
സ്ഥിതം ശൂന്യം crc32init_be(ശൂന്യം)
{
	unsigned i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0][0] = 0;

	വേണ്ടി (i = 1; i < BE_TABLE_SIZE; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		വേണ്ടി (j = 0; j < i; j++)
			crc32table_be[0][i + j] = crc ^ crc32table_be[0][j];
	}
	വേണ്ടി (i = 0; i < BE_TABLE_SIZE; i++) {
		crc = crc32table_be[0][i];
		വേണ്ടി (j = 1; j < BE_TABLE_ROWS; j++) {
			crc = crc32table_be[0][(crc >> 24) & 0xff] ^ (crc << 8);
			crc32table_be[j][i] = crc;
		}
	}
}

സ്ഥിതം ശൂന്യം output_table(uint32_t (*table)[256], പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ len, അക്ഷരം *trans)
{
	പൂർണ്ണസംഖ്യ i, j;

	വേണ്ടി (j = 0 ; j < rows; j++) {
		printf("{");
		വേണ്ടി (i = 0; i < len - 1; i++) {
			എങ്കിൽ (i % ENTRIES_PER_വരി == 0)
				printf("\n");
			printf("%s(0x%8.8xL), ", trans, table[j][i]);
		}
		printf("%s(0x%8.8xL)},\n", trans, table[j][len - 1]);
	}
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	printf("/* this file is generated - do not edit */\n\n");

	എങ്കിൽ (CRC_BE_BITS > 1) {
		crc32init_be();
		printf("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_TABLE_ROWS, BE_TABLE_SIZE);
		output_table(crc32table_be, LE_TABLE_ROWS,
			     BE_TABLE_SIZE, "tobe");
		printf("};\n");
	}
	എങ്കിൽ (CRC_LE_BITS > 1) {
		crc32cinit_le();
		printf("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_TABLE_ROWS, LE_TABLE_SIZE);
		output_table(crc32ctable_le, LE_TABLE_ROWS,
			     LE_TABLE_SIZE, "tole");
		printf("};\n");
	}

	തിരികെ 0;
}
