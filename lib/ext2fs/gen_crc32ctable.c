#ضمن <stdio.h>
#ضمن "crc32c_defs.h"
#ضمن <inttypes.h>

#تعریف_کن ENTRIES_در_هر_سطر 4

#إذا بررسی_چرخهای_LE_بیتها > 8
# define LE_جدول_ردیفها (بررسی_چرخهای_LE_بیتها/8)
# define LE_جدول_اندازه 256
#وإلا
# define LE_جدول_ردیفها 1
# define LE_جدول_اندازه (1 << بررسی_چرخهای_LE_بیتها)
#پایان_اگر

#إذا بررسی_چرخهای_BE_بیتها > 8
# define BE_جدول_ردیفها (بررسی_چرخهای_BE_بیتها/8)
# define BE_جدول_اندازه 256
#وإلا
# define BE_جدول_ردیفها 1
# define BE_جدول_اندازه (1 << بررسی_چرخهای_BE_بیتها)
#پایان_اگر

ساكن uint32_t crc32table_be[BE_جدول_ردیفها][256];
ساكن uint32_t crc32ctable_le[LE_جدول_ردیفها][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ساكن فراغ crc32cinit_le(فراغ)
{
	بیعلامت i, j;
	uint32_t بررسی_چرخهای = 1;

	crc32ctable_le[0][0] = 0;

	لكل (i = LE_جدول_اندازه >> 1; i; i >>= 1) {
		بررسی_چرخهای = (بررسی_چرخهای >> 1) ^ ((بررسی_چرخهای & 1) ? بررسی_چرخهای_32_C_POLY_LE : 0);
		لكل (j = 0; j < LE_جدول_اندازه; j += 2 * i)
			crc32ctable_le[0][i + j] = بررسی_چرخهای ^ crc32ctable_le[0][j];
	}
	لكل (i = 0; i < LE_جدول_اندازه; i++) {
		بررسی_چرخهای = crc32ctable_le[0][i];
		لكل (j = 1; j < LE_جدول_ردیفها; j++) {
			بررسی_چرخهای = crc32ctable_le[0][بررسی_چرخهای & 0xff] ^ (بررسی_چرخهای >> 8);
			crc32ctable_le[j][i] = بررسی_چرخهای;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ساكن فراغ crc32init_be(فراغ)
{
	بیعلامت i, j;
	uint32_t بررسی_چرخهای = 0x80000000;

	crc32table_be[0][0] = 0;

	لكل (i = 1; i < BE_جدول_اندازه; i <<= 1) {
		بررسی_چرخهای = (بررسی_چرخهای << 1) ^ ((بررسی_چرخهای & 0x80000000) ? CRCPOLY_BE : 0);
		لكل (j = 0; j < i; j++)
			crc32table_be[0][i + j] = بررسی_چرخهای ^ crc32table_be[0][j];
	}
	لكل (i = 0; i < BE_جدول_اندازه; i++) {
		بررسی_چرخهای = crc32table_be[0][i];
		لكل (j = 1; j < BE_جدول_ردیفها; j++) {
			بررسی_چرخهای = crc32table_be[0][(بررسی_چرخهای >> 24) & 0xff] ^ (بررسی_چرخهای << 8);
			crc32table_be[j][i] = بررسی_چرخهای;
		}
	}
}

ساكن فراغ output_جدول(uint32_t (*جدول)[256], صحيح ردیفها, صحيح len, حرف *trans)
{
	صحيح i, j;

	لكل (j = 0 ; j < ردیفها; j++) {
		چاپ_کن("{");
		لكل (i = 0; i < len - 1; i++) {
			إذا (i % ENTRIES_در_هر_سطر == 0)
				چاپ_کن("\n");
			چاپ_کن("%s(0x%8.8xL), ", trans, جدول[j][i]);
		}
		چاپ_کن("%s(0x%8.8xL)},\n", trans, جدول[j][len - 1]);
	}
}

صحيح اصلی(صحيح argc, حرف **argv)
{
	چاپ_کن("/* this file is generated - do not edit */\n\n");

	إذا (بررسی_چرخهای_BE_بیتها > 1) {
		crc32init_be();
		چاپ_کن("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_جدول_ردیفها, BE_جدول_اندازه);
		output_جدول(crc32table_be, LE_جدول_ردیفها,
			     BE_جدول_اندازه, "tobe");
		چاپ_کن("};\n");
	}
	إذا (بررسی_چرخهای_LE_بیتها > 1) {
		crc32cinit_le();
		چاپ_کن("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_جدول_ردیفها, LE_جدول_اندازه);
		output_جدول(crc32ctable_le, LE_جدول_ردیفها,
			     LE_جدول_اندازه, "tole");
		چاپ_کن("};\n");
	}

	أرجع 0;
}
