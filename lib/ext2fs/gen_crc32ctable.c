#समावेश <stdio.h>
#समावेश "crc32c_defs.h"
#समावेश <inttypes.h>

#घोषणा ENTRIES_PER_पंक्ति 4

#अगर CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_आकार 256
#अन्यथा
# define LE_TABLE_ROWS 1
# define LE_TABLE_आकार (1 << CRC_LE_BITS)
#पूर्ण_अगर

#अगर CRC_BE_BITS > 8
# define BE_TABLE_ROWS (CRC_BE_BITS/8)
# define BE_TABLE_आकार 256
#अन्यथा
# define BE_TABLE_ROWS 1
# define BE_TABLE_आकार (1 << CRC_BE_BITS)
#पूर्ण_अगर

अटल uint32_t crc32table_be[BE_TABLE_ROWS][256];
अटल uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
अटल व्योम crc32cinit_le(व्योम)
{
	अचिन्हित i, j;
	uint32_t crc = 1;

	crc32ctable_le[0][0] = 0;

	क्रम (i = LE_TABLE_आकार >> 1; i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? CRC32C_POLY_LE : 0);
		क्रम (j = 0; j < LE_TABLE_आकार; j += 2 * i)
			crc32ctable_le[0][i + j] = crc ^ crc32ctable_le[0][j];
	}
	क्रम (i = 0; i < LE_TABLE_आकार; i++) {
		crc = crc32ctable_le[0][i];
		क्रम (j = 1; j < LE_TABLE_ROWS; j++) {
			crc = crc32ctable_le[0][crc & 0xff] ^ (crc >> 8);
			crc32ctable_le[j][i] = crc;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
अटल व्योम crc32init_be(व्योम)
{
	अचिन्हित i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0][0] = 0;

	क्रम (i = 1; i < BE_TABLE_आकार; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		क्रम (j = 0; j < i; j++)
			crc32table_be[0][i + j] = crc ^ crc32table_be[0][j];
	}
	क्रम (i = 0; i < BE_TABLE_आकार; i++) {
		crc = crc32table_be[0][i];
		क्रम (j = 1; j < BE_TABLE_ROWS; j++) {
			crc = crc32table_be[0][(crc >> 24) & 0xff] ^ (crc << 8);
			crc32table_be[j][i] = crc;
		}
	}
}

अटल व्योम output_table(uint32_t (*table)[256], पूर्णांक rows, पूर्णांक len, अक्षर *trans)
{
	पूर्णांक i, j;

	क्रम (j = 0 ; j < rows; j++) {
		म_लिखो("{");
		क्रम (i = 0; i < len - 1; i++) {
			अगर (i % ENTRIES_PER_पंक्ति == 0)
				म_लिखो("\n");
			म_लिखो("%s(0x%8.8xL), ", trans, table[j][i]);
		}
		म_लिखो("%s(0x%8.8xL)},\n", trans, table[j][len - 1]);
	}
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	म_लिखो("/* this file is generated - do not edit */\n\n");

	अगर (CRC_BE_BITS > 1) {
		crc32init_be();
		म_लिखो("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_TABLE_ROWS, BE_TABLE_आकार);
		output_table(crc32table_be, LE_TABLE_ROWS,
			     BE_TABLE_आकार, "tobe");
		म_लिखो("};\n");
	}
	अगर (CRC_LE_BITS > 1) {
		crc32cinit_le();
		म_लिखो("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_TABLE_ROWS, LE_TABLE_आकार);
		output_table(crc32ctable_le, LE_TABLE_ROWS,
			     LE_TABLE_आकार, "tole");
		म_लिखो("};\n");
	}

	वापस 0;
}
