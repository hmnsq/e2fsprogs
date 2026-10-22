#شامل <stdio.h>
#شامل "crc32c_defs.h"
#شامل <inttypes.h>

#تعریف_کرو ENTRIES_فی_سطر 4

#اگر چکری_جانچ_LE_بٹ > 8
# define LE_جدول_صفیں (چکری_جانچ_LE_بٹ/8)
# define LE_جدول_سائز 256
#ورنہ
# define LE_جدول_صفیں 1
# define LE_جدول_سائز (1 << چکری_جانچ_LE_بٹ)
#اگر_ختم

#اگر چکری_جانچ_BE_بٹ > 8
# define BE_جدول_صفیں (چکری_جانچ_BE_بٹ/8)
# define BE_جدول_سائز 256
#ورنہ
# define BE_جدول_صفیں 1
# define BE_جدول_سائز (1 << چکری_جانچ_BE_بٹ)
#اگر_ختم

ساکن uint32_t crc32table_be[BE_جدول_صفیں][256];
ساکن uint32_t crc32ctable_le[LE_جدول_صفیں][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ساکن خالی crc32cinit_le(خالی)
{
	غیرعلامتی i, j;
	uint32_t چکری_جانچ = 1;

	crc32ctable_le[0][0] = 0;

	برائے (i = LE_جدول_سائز >> 1; i; i >>= 1) {
		چکری_جانچ = (چکری_جانچ >> 1) ^ ((چکری_جانچ & 1) ? چکری_جانچ_32_C_POLY_LE : 0);
		برائے (j = 0; j < LE_جدول_سائز; j += 2 * i)
			crc32ctable_le[0][i + j] = چکری_جانچ ^ crc32ctable_le[0][j];
	}
	برائے (i = 0; i < LE_جدول_سائز; i++) {
		چکری_جانچ = crc32ctable_le[0][i];
		برائے (j = 1; j < LE_جدول_صفیں; j++) {
			چکری_جانچ = crc32ctable_le[0][چکری_جانچ & 0xff] ^ (چکری_جانچ >> 8);
			crc32ctable_le[j][i] = چکری_جانچ;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ساکن خالی crc32init_be(خالی)
{
	غیرعلامتی i, j;
	uint32_t چکری_جانچ = 0x80000000;

	crc32table_be[0][0] = 0;

	برائے (i = 1; i < BE_جدول_سائز; i <<= 1) {
		چکری_جانچ = (چکری_جانچ << 1) ^ ((چکری_جانچ & 0x80000000) ? CRCPOLY_BE : 0);
		برائے (j = 0; j < i; j++)
			crc32table_be[0][i + j] = چکری_جانچ ^ crc32table_be[0][j];
	}
	برائے (i = 0; i < BE_جدول_سائز; i++) {
		چکری_جانچ = crc32table_be[0][i];
		برائے (j = 1; j < BE_جدول_صفیں; j++) {
			چکری_جانچ = crc32table_be[0][(چکری_جانچ >> 24) & 0xff] ^ (چکری_جانچ << 8);
			crc32table_be[j][i] = چکری_جانچ;
		}
	}
}

ساکن خالی output_جدول(uint32_t (*جدول)[256], صحیح صفیں, صحیح len, حرف *trans)
{
	صحیح i, j;

	برائے (j = 0 ; j < صفیں; j++) {
		لکھو("{");
		برائے (i = 0; i < len - 1; i++) {
			اگر (i % ENTRIES_فی_سطر == 0)
				لکھو("\n");
			لکھو("%s(0x%8.8xL), ", trans, جدول[j][i]);
		}
		لکھو("%s(0x%8.8xL)},\n", trans, جدول[j][len - 1]);
	}
}

صحیح مین(صحیح argc, حرف **argv)
{
	لکھو("/* this file is generated - do not edit */\n\n");

	اگر (چکری_جانچ_BE_بٹ > 1) {
		crc32init_be();
		لکھو("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_جدول_صفیں, BE_جدول_سائز);
		output_جدول(crc32table_be, LE_جدول_صفیں,
			     BE_جدول_سائز, "tobe");
		لکھو("};\n");
	}
	اگر (چکری_جانچ_LE_بٹ > 1) {
		crc32cinit_le();
		لکھو("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_جدول_صفیں, LE_جدول_سائز);
		output_جدول(crc32ctable_le, LE_جدول_صفیں,
			     LE_جدول_سائز, "tole");
		لکھو("};\n");
	}

	واپس 0;
}
