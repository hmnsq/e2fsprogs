#अन्तर्भाव <stdio.h>
#अन्तर्भाव "crc32c_defs.h"
#अन्तर्भाव <inttypes.h>

#define ENTRIES_PER_पङ्क्ति 4

#यदि CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_SIZE 256
#अन्यथा
# define LE_TABLE_ROWS 1
# define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#endif

#यदि CRC_BE_BITS > 8
# define BE_TABLE_ROWS (CRC_BE_BITS/8)
# define BE_TABLE_SIZE 256
#अन्यथा
# define BE_TABLE_ROWS 1
# define BE_TABLE_SIZE (1 << CRC_BE_BITS)
#endif

स्थिर uint32_t crc32table_be[BE_TABLE_ROWS][256];
स्थिर uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
स्थिर शून्य crc32cinit_le(शून्य)
{
	अचिन्हित i, j;
	uint32_t crc = 1;

	crc32ctable_le[0][0] = 0;

	लागि (i = LE_TABLE_SIZE >> 1; i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? CRC32C_POLY_LE : 0);
		लागि (j = 0; j < LE_TABLE_SIZE; j += 2 * i)
			crc32ctable_le[0][i + j] = crc ^ crc32ctable_le[0][j];
	}
	लागि (i = 0; i < LE_TABLE_SIZE; i++) {
		crc = crc32ctable_le[0][i];
		लागि (j = 1; j < LE_TABLE_ROWS; j++) {
			crc = crc32ctable_le[0][crc & 0xff] ^ (crc >> 8);
			crc32ctable_le[j][i] = crc;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
स्थिर शून्य crc32init_be(शून्य)
{
	अचिन्हित i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0][0] = 0;

	लागि (i = 1; i < BE_TABLE_SIZE; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		लागि (j = 0; j < i; j++)
			crc32table_be[0][i + j] = crc ^ crc32table_be[0][j];
	}
	लागि (i = 0; i < BE_TABLE_SIZE; i++) {
		crc = crc32table_be[0][i];
		लागि (j = 1; j < BE_TABLE_ROWS; j++) {
			crc = crc32table_be[0][(crc >> 24) & 0xff] ^ (crc << 8);
			crc32table_be[j][i] = crc;
		}
	}
}

स्थिर शून्य output_table(uint32_t (*table)[256], पूर्णाङ्क rows, पूर्णाङ्क len, वर्ण *trans)
{
	पूर्णाङ्क i, j;

	लागि (j = 0 ; j < rows; j++) {
		म_लिखो("{");
		लागि (i = 0; i < len - 1; i++) {
			यदि (i % ENTRIES_PER_पङ्क्ति == 0)
				म_लिखो("\n");
			म_लिखो("%s(0x%8.8xL), ", trans, table[j][i]);
		}
		म_लिखो("%s(0x%8.8xL)},\n", trans, table[j][len - 1]);
	}
}

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	म_लिखो("/* this file is generated - do not edit */\n\n");

	यदि (CRC_BE_BITS > 1) {
		crc32init_be();
		म_लिखो("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_TABLE_ROWS, BE_TABLE_SIZE);
		output_table(crc32table_be, LE_TABLE_ROWS,
			     BE_TABLE_SIZE, "tobe");
		म_लिखो("};\n");
	}
	यदि (CRC_LE_BITS > 1) {
		crc32cinit_le();
		म_लिखो("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_TABLE_ROWS, LE_TABLE_SIZE);
		output_table(crc32ctable_le, LE_TABLE_ROWS,
			     LE_TABLE_SIZE, "tole");
		म_लिखो("};\n");
	}

	फर्काऊ 0;
}
