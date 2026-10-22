#अन्तर्भाव <stdio.h>
#अन्तर्भाव "crc32c_defs.h"
#अन्तर्भाव <inttypes.h>

#परिभाषय ENTRIES_प्रत्येकम्_पङ्क्तिः 4

#यदि CRC_LE_द्वयङ्काः > 8
# define LE_सारणी_पङ्क्तिकाः (CRC_LE_द्वयङ्काः/8)
# define LE_सारणी_आकार 256
#अन्यथा
# define LE_सारणी_पङ्क्तिकाः 1
# define LE_सारणी_आकार (1 << CRC_LE_द्वयङ्काः)
#यदि_अन्तः

#यदि CRC_BE_द्वयङ्काः > 8
# define BE_सारणी_पङ्क्तिकाः (CRC_BE_द्वयङ्काः/8)
# define BE_सारणी_आकार 256
#अन्यथा
# define BE_सारणी_पङ्क्तिकाः 1
# define BE_सारणी_आकार (1 << CRC_BE_द्वयङ्काः)
#यदि_अन्तः

स्थिर uint32_t crc32table_be[BE_सारणी_पङ्क्तिकाः][256];
स्थिर uint32_t crc32ctable_le[LE_सारणी_पङ्क्तिकाः][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
स्थिर शून्य crc32cinit_le(शून्य)
{
	अचिह्नित i, j;
	uint32_t crc = 1;

	crc32ctable_le[0][0] = 0;

	प्रति (i = LE_सारणी_आकार >> 1; i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? चक्रीयपरीक्षा_32_C_POLY_LE : 0);
		प्रति (j = 0; j < LE_सारणी_आकार; j += 2 * i)
			crc32ctable_le[0][i + j] = crc ^ crc32ctable_le[0][j];
	}
	प्रति (i = 0; i < LE_सारणी_आकार; i++) {
		crc = crc32ctable_le[0][i];
		प्रति (j = 1; j < LE_सारणी_पङ्क्तिकाः; j++) {
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
	अचिह्नित i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0][0] = 0;

	प्रति (i = 1; i < BE_सारणी_आकार; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		प्रति (j = 0; j < i; j++)
			crc32table_be[0][i + j] = crc ^ crc32table_be[0][j];
	}
	प्रति (i = 0; i < BE_सारणी_आकार; i++) {
		crc = crc32table_be[0][i];
		प्रति (j = 1; j < BE_सारणी_पङ्क्तिकाः; j++) {
			crc = crc32table_be[0][(crc >> 24) & 0xff] ^ (crc << 8);
			crc32table_be[j][i] = crc;
		}
	}
}

स्थिर शून्य output_सारणी(uint32_t (*सारणी)[256], पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क len, वर्ण *trans)
{
	पूर्णाङ्क i, j;

	प्रति (j = 0 ; j < पङ्क्तिकाः; j++) {
		प्रारूप_लिख("{");
		प्रति (i = 0; i < len - 1; i++) {
			यदि (i % ENTRIES_प्रत्येकम्_पङ्क्तिः == 0)
				प्रारूप_लिख("\n");
			प्रारूप_लिख("%s(0x%8.8xL), ", trans, सारणी[j][i]);
		}
		प्रारूप_लिख("%s(0x%8.8xL)},\n", trans, सारणी[j][len - 1]);
	}
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	प्रारूप_लिख("/* this file is generated - do not edit */\n\n");

	यदि (CRC_BE_द्वयङ्काः > 1) {
		crc32init_be();
		प्रारूप_लिख("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_सारणी_पङ्क्तिकाः, BE_सारणी_आकार);
		output_सारणी(crc32table_be, LE_सारणी_पङ्क्तिकाः,
			     BE_सारणी_आकार, "tobe");
		प्रारूप_लिख("};\n");
	}
	यदि (CRC_LE_द्वयङ्काः > 1) {
		crc32cinit_le();
		प्रारूप_लिख("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_सारणी_पङ्क्तिकाः, LE_सारणी_आकार);
		output_सारणी(crc32ctable_le, LE_सारणी_पङ्क्तिकाः,
			     LE_सारणी_आकार, "tole");
		प्रारूप_लिख("};\n");
	}

	प्रत्यागम 0;
}
