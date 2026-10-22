#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "crc32c_defs.h"
#அந்தர்பாவ <inttypes.h>

#define ENTRIES_PER_வரி 4

#எனில் CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_SIZE 256
#இல்லையெனில்
# define LE_TABLE_ROWS 1
# define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#endif

#எனில் CRC_BE_BITS > 8
# define BE_TABLE_ROWS (CRC_BE_BITS/8)
# define BE_TABLE_SIZE 256
#இல்லையெனில்
# define BE_TABLE_ROWS 1
# define BE_TABLE_SIZE (1 << CRC_BE_BITS)
#endif

நிலையான uint32_t crc32table_be[BE_TABLE_ROWS][256];
நிலையான uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
நிலையான வெறுமை crc32cinit_le(வெறுமை)
{
	unsigned i, j;
	uint32_t crc = 1;

	crc32ctable_le[0][0] = 0;

	க்கு (i = LE_TABLE_SIZE >> 1; i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? CRC32C_POLY_LE : 0);
		க்கு (j = 0; j < LE_TABLE_SIZE; j += 2 * i)
			crc32ctable_le[0][i + j] = crc ^ crc32ctable_le[0][j];
	}
	க்கு (i = 0; i < LE_TABLE_SIZE; i++) {
		crc = crc32ctable_le[0][i];
		க்கு (j = 1; j < LE_TABLE_ROWS; j++) {
			crc = crc32ctable_le[0][crc & 0xff] ^ (crc >> 8);
			crc32ctable_le[j][i] = crc;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
நிலையான வெறுமை crc32init_be(வெறுமை)
{
	unsigned i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0][0] = 0;

	க்கு (i = 1; i < BE_TABLE_SIZE; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		க்கு (j = 0; j < i; j++)
			crc32table_be[0][i + j] = crc ^ crc32table_be[0][j];
	}
	க்கு (i = 0; i < BE_TABLE_SIZE; i++) {
		crc = crc32table_be[0][i];
		க்கு (j = 1; j < BE_TABLE_ROWS; j++) {
			crc = crc32table_be[0][(crc >> 24) & 0xff] ^ (crc << 8);
			crc32table_be[j][i] = crc;
		}
	}
}

நிலையான வெறுமை output_table(uint32_t (*table)[256], முழு rows, முழு len, எழுத்து *trans)
{
	முழு i, j;

	க்கு (j = 0 ; j < rows; j++) {
		printf("{");
		க்கு (i = 0; i < len - 1; i++) {
			எனில் (i % ENTRIES_PER_வரி == 0)
				printf("\n");
			printf("%s(0x%8.8xL), ", trans, table[j][i]);
		}
		printf("%s(0x%8.8xL)},\n", trans, table[j][len - 1]);
	}
}

முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	printf("/* this file is generated - do not edit */\n\n");

	எனில் (CRC_BE_BITS > 1) {
		crc32init_be();
		printf("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_TABLE_ROWS, BE_TABLE_SIZE);
		output_table(crc32table_be, LE_TABLE_ROWS,
			     BE_TABLE_SIZE, "tobe");
		printf("};\n");
	}
	எனில் (CRC_LE_BITS > 1) {
		crc32cinit_le();
		printf("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_TABLE_ROWS, LE_TABLE_SIZE);
		output_table(crc32ctable_le, LE_TABLE_ROWS,
			     LE_TABLE_SIZE, "tole");
		printf("};\n");
	}

	திரும்பு 0;
}
