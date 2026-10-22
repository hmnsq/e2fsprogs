#ضمن <stdio.h>
#ضمن "crc32c_defs.h"
#ضمن <inttypes.h>

#تعریف_کرو ENTRIES_فی_سطر 4

#إذا چکری_جانچ_LE_بٹ > 8
# define LE_جدول_صفاں (چکری_جانچ_LE_بٹ/8)
# define LE_جدول_ناپ 256
#وإلا
# define LE_جدول_صفاں 1
# define LE_جدول_ناپ (1 << چکری_جانچ_LE_بٹ)
#جے_انت

#إذا چکری_جانچ_BE_بٹ > 8
# define BE_جدول_صفاں (چکری_جانچ_BE_بٹ/8)
# define BE_جدول_ناپ 256
#وإلا
# define BE_جدول_صفاں 1
# define BE_جدول_ناپ (1 << چکری_جانچ_BE_بٹ)
#جے_انت

ساكن uint32_t crc32table_be[BE_جدول_صفاں][256];
ساكن uint32_t crc32ctable_le[LE_جدول_صفاں][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ساكن فراغ crc32cinit_le(فراغ)
{
	بنانشانی i, j;
	uint32_t چکری_جانچ = 1;

	crc32ctable_le[0][0] = 0;

	لكل (i = LE_جدول_ناپ >> 1; i; i >>= 1) {
		چکری_جانچ = (چکری_جانچ >> 1) ^ ((چکری_جانچ & 1) ? چکری_جانچ_32_C_POLY_LE : 0);
		لكل (j = 0; j < LE_جدول_ناپ; j += 2 * i)
			crc32ctable_le[0][i + j] = چکری_جانچ ^ crc32ctable_le[0][j];
	}
	لكل (i = 0; i < LE_جدول_ناپ; i++) {
		چکری_جانچ = crc32ctable_le[0][i];
		لكل (j = 1; j < LE_جدول_صفاں; j++) {
			چکری_جانچ = crc32ctable_le[0][چکری_جانچ & 0xff] ^ (چکری_جانچ >> 8);
			crc32ctable_le[j][i] = چکری_جانچ;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ساكن فراغ crc32init_be(فراغ)
{
	بنانشانی i, j;
	uint32_t چکری_جانچ = 0x80000000;

	crc32table_be[0][0] = 0;

	لكل (i = 1; i < BE_جدول_ناپ; i <<= 1) {
		چکری_جانچ = (چکری_جانچ << 1) ^ ((چکری_جانچ & 0x80000000) ? CRCPOLY_BE : 0);
		لكل (j = 0; j < i; j++)
			crc32table_be[0][i + j] = چکری_جانچ ^ crc32table_be[0][j];
	}
	لكل (i = 0; i < BE_جدول_ناپ; i++) {
		چکری_جانچ = crc32table_be[0][i];
		لكل (j = 1; j < BE_جدول_صفاں; j++) {
			چکری_جانچ = crc32table_be[0][(چکری_جانچ >> 24) & 0xff] ^ (چکری_جانچ << 8);
			crc32table_be[j][i] = چکری_جانچ;
		}
	}
}

ساكن فراغ output_جدول(uint32_t (*جدول)[256], صحيح صفاں, صحيح len, حرف *trans)
{
	صحيح i, j;

	لكل (j = 0 ; j < صفاں; j++) {
		لکھو("{");
		لكل (i = 0; i < len - 1; i++) {
			إذا (i % ENTRIES_فی_سطر == 0)
				لکھو("\n");
			لکھو("%s(0x%8.8xL), ", trans, جدول[j][i]);
		}
		لکھو("%s(0x%8.8xL)},\n", trans, جدول[j][len - 1]);
	}
}

صحيح مکھ(صحيح argc, حرف **argv)
{
	لکھو("/* this file is generated - do not edit */\n\n");

	إذا (چکری_جانچ_BE_بٹ > 1) {
		crc32init_be();
		لکھو("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_جدول_صفاں, BE_جدول_ناپ);
		output_جدول(crc32table_be, LE_جدول_صفاں,
			     BE_جدول_ناپ, "tobe");
		لکھو("};\n");
	}
	إذا (چکری_جانچ_LE_بٹ > 1) {
		crc32cinit_le();
		لکھو("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_جدول_صفاں, LE_جدول_ناپ);
		output_جدول(crc32ctable_le, LE_جدول_صفاں,
			     LE_جدول_ناپ, "tole");
		لکھو("};\n");
	}

	أرجع 0;
}
