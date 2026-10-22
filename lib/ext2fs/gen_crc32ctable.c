#ضمن <stdio.h>
#ضمن "crc32c_defs.h"
#ضمن <inttypes.h>

#وصف_ڪريو ENTRIES_في_سٽ 4

#إذا چڪري_جاچ_LE_بٽ > 8
# define LE_جدول_صفون (چڪري_جاچ_LE_بٽ/8)
# define LE_جدول_سائيز 256
#وإلا
# define LE_جدول_صفون 1
# define LE_جدول_سائيز (1 << چڪري_جاچ_LE_بٽ)
#جيڪڏهن_پورو

#إذا چڪري_جاچ_BE_بٽ > 8
# define BE_جدول_صفون (چڪري_جاچ_BE_بٽ/8)
# define BE_جدول_سائيز 256
#وإلا
# define BE_جدول_صفون 1
# define BE_جدول_سائيز (1 << چڪري_جاچ_BE_بٽ)
#جيڪڏهن_پورو

ساكن uint32_t crc32table_be[BE_جدول_صفون][256];
ساكن uint32_t crc32ctable_le[LE_جدول_صفون][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ساكن فراغ crc32cinit_le(فراغ)
{
	بنانشان i, j;
	uint32_t چڪري_جاچ = 1;

	crc32ctable_le[0][0] = 0;

	لكل (i = LE_جدول_سائيز >> 1; i; i >>= 1) {
		چڪري_جاچ = (چڪري_جاچ >> 1) ^ ((چڪري_جاچ & 1) ? چڪري_جاچ_32_C_POLY_LE : 0);
		لكل (j = 0; j < LE_جدول_سائيز; j += 2 * i)
			crc32ctable_le[0][i + j] = چڪري_جاچ ^ crc32ctable_le[0][j];
	}
	لكل (i = 0; i < LE_جدول_سائيز; i++) {
		چڪري_جاچ = crc32ctable_le[0][i];
		لكل (j = 1; j < LE_جدول_صفون; j++) {
			چڪري_جاچ = crc32ctable_le[0][چڪري_جاچ & 0xff] ^ (چڪري_جاچ >> 8);
			crc32ctable_le[j][i] = چڪري_جاچ;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ساكن فراغ crc32init_be(فراغ)
{
	بنانشان i, j;
	uint32_t چڪري_جاچ = 0x80000000;

	crc32table_be[0][0] = 0;

	لكل (i = 1; i < BE_جدول_سائيز; i <<= 1) {
		چڪري_جاچ = (چڪري_جاچ << 1) ^ ((چڪري_جاچ & 0x80000000) ? CRCPOLY_BE : 0);
		لكل (j = 0; j < i; j++)
			crc32table_be[0][i + j] = چڪري_جاچ ^ crc32table_be[0][j];
	}
	لكل (i = 0; i < BE_جدول_سائيز; i++) {
		چڪري_جاچ = crc32table_be[0][i];
		لكل (j = 1; j < BE_جدول_صفون; j++) {
			چڪري_جاچ = crc32table_be[0][(چڪري_جاچ >> 24) & 0xff] ^ (چڪري_جاچ << 8);
			crc32table_be[j][i] = چڪري_جاچ;
		}
	}
}

ساكن فراغ output_جدول(uint32_t (*جدول)[256], صحيح صفون, صحيح len, حرف *trans)
{
	صحيح i, j;

	لكل (j = 0 ; j < صفون; j++) {
		لکو("{");
		لكل (i = 0; i < len - 1; i++) {
			إذا (i % ENTRIES_في_سٽ == 0)
				لکو("\n");
			لکو("%s(0x%8.8xL), ", trans, جدول[j][i]);
		}
		لکو("%s(0x%8.8xL)},\n", trans, جدول[j][len - 1]);
	}
}

صحيح مکيه(صحيح argc, حرف **argv)
{
	لکو("/* this file is generated - do not edit */\n\n");

	إذا (چڪري_جاچ_BE_بٽ > 1) {
		crc32init_be();
		لکو("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_جدول_صفون, BE_جدول_سائيز);
		output_جدول(crc32table_be, LE_جدول_صفون,
			     BE_جدول_سائيز, "tobe");
		لکو("};\n");
	}
	إذا (چڪري_جاچ_LE_بٽ > 1) {
		crc32cinit_le();
		لکو("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_جدول_صفون, LE_جدول_سائيز);
		output_جدول(crc32ctable_le, LE_جدول_صفون,
			     LE_جدول_سائيز, "tole");
		لکو("};\n");
	}

	أرجع 0;
}
