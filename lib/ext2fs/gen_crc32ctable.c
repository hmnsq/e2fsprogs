#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "crc32c_defs.h"
#ଅନ୍ତର୍ଭାଵ <inttypes.h>

#define ENTRIES_PER_ଧାଡ଼ି 4

#ଯଦି CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_SIZE 256
#ଅନ୍ୟଥା
# define LE_TABLE_ROWS 1
# define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#endif

#ଯଦି CRC_BE_BITS > 8
# define BE_TABLE_ROWS (CRC_BE_BITS/8)
# define BE_TABLE_SIZE 256
#ଅନ୍ୟଥା
# define BE_TABLE_ROWS 1
# define BE_TABLE_SIZE (1 << CRC_BE_BITS)
#endif

ସ୍ଥିର uint32_t crc32table_be[BE_TABLE_ROWS][256];
ସ୍ଥିର uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ସ୍ଥିର ଶୂନ୍ୟ crc32cinit_le(ଶୂନ୍ୟ)
{
	unsigned i, j;
	uint32_t crc = 1;

	crc32ctable_le[0][0] = 0;

	ପାଇଁ (i = LE_TABLE_SIZE >> 1; i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? CRC32C_POLY_LE : 0);
		ପାଇଁ (j = 0; j < LE_TABLE_SIZE; j += 2 * i)
			crc32ctable_le[0][i + j] = crc ^ crc32ctable_le[0][j];
	}
	ପାଇଁ (i = 0; i < LE_TABLE_SIZE; i++) {
		crc = crc32ctable_le[0][i];
		ପାଇଁ (j = 1; j < LE_TABLE_ROWS; j++) {
			crc = crc32ctable_le[0][crc & 0xff] ^ (crc >> 8);
			crc32ctable_le[j][i] = crc;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ସ୍ଥିର ଶୂନ୍ୟ crc32init_be(ଶୂନ୍ୟ)
{
	unsigned i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0][0] = 0;

	ପାଇଁ (i = 1; i < BE_TABLE_SIZE; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		ପାଇଁ (j = 0; j < i; j++)
			crc32table_be[0][i + j] = crc ^ crc32table_be[0][j];
	}
	ପାଇଁ (i = 0; i < BE_TABLE_SIZE; i++) {
		crc = crc32table_be[0][i];
		ପାଇଁ (j = 1; j < BE_TABLE_ROWS; j++) {
			crc = crc32table_be[0][(crc >> 24) & 0xff] ^ (crc << 8);
			crc32table_be[j][i] = crc;
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ output_table(uint32_t (*table)[256], ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅକ୍ଷର *trans)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

	ପାଇଁ (j = 0 ; j < rows; j++) {
		printf("{");
		ପାଇଁ (i = 0; i < len - 1; i++) {
			ଯଦି (i % ENTRIES_PER_ଧାଡ଼ି == 0)
				printf("\n");
			printf("%s(0x%8.8xL), ", trans, table[j][i]);
		}
		printf("%s(0x%8.8xL)},\n", trans, table[j][len - 1]);
	}
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	printf("/* this file is generated - do not edit */\n\n");

	ଯଦି (CRC_BE_BITS > 1) {
		crc32init_be();
		printf("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_TABLE_ROWS, BE_TABLE_SIZE);
		output_table(crc32table_be, LE_TABLE_ROWS,
			     BE_TABLE_SIZE, "tobe");
		printf("};\n");
	}
	ଯଦି (CRC_LE_BITS > 1) {
		crc32cinit_le();
		printf("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_TABLE_ROWS, LE_TABLE_SIZE);
		output_table(crc32ctable_le, LE_TABLE_ROWS,
			     LE_TABLE_SIZE, "tole");
		printf("};\n");
	}

	ଫେରନ୍ତୁ 0;
}
