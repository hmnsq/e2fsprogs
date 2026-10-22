#ضمن <stdio.h>
#ضمن "crc32c_defs.h"
#ضمن <inttypes.h>

#تعریف_کړه ENTRIES_فی_کرښه 4

#إذا دوراني_ازموینه_LE_بټونه > 8
# define LE_جدول_لیکې (دوراني_ازموینه_LE_بټونه/8)
# define LE_جدول_اندازه 256
#وإلا
# define LE_جدول_لیکې 1
# define LE_جدول_اندازه (1 << دوراني_ازموینه_LE_بټونه)
#د_که_پای

#إذا دوراني_ازموینه_BE_بټونه > 8
# define BE_جدول_لیکې (دوراني_ازموینه_BE_بټونه/8)
# define BE_جدول_اندازه 256
#وإلا
# define BE_جدول_لیکې 1
# define BE_جدول_اندازه (1 << دوراني_ازموینه_BE_بټونه)
#د_که_پای

ساكن uint32_t crc32table_be[BE_جدول_لیکې][256];
ساكن uint32_t crc32ctable_le[LE_جدول_لیکې][256];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
ساكن فراغ crc32cinit_le(فراغ)
{
	بینښه i, j;
	uint32_t دوراني_ازموینه = 1;

	crc32ctable_le[0][0] = 0;

	لكل (i = LE_جدول_اندازه >> 1; i; i >>= 1) {
		دوراني_ازموینه = (دوراني_ازموینه >> 1) ^ ((دوراني_ازموینه & 1) ? دوراني_ازموینه_32_C_POLY_LE : 0);
		لكل (j = 0; j < LE_جدول_اندازه; j += 2 * i)
			crc32ctable_le[0][i + j] = دوراني_ازموینه ^ crc32ctable_le[0][j];
	}
	لكل (i = 0; i < LE_جدول_اندازه; i++) {
		دوراني_ازموینه = crc32ctable_le[0][i];
		لكل (j = 1; j < LE_جدول_لیکې; j++) {
			دوراني_ازموینه = crc32ctable_le[0][دوراني_ازموینه & 0xff] ^ (دوراني_ازموینه >> 8);
			crc32ctable_le[j][i] = دوراني_ازموینه;
		}
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
ساكن فراغ crc32init_be(فراغ)
{
	بینښه i, j;
	uint32_t دوراني_ازموینه = 0x80000000;

	crc32table_be[0][0] = 0;

	لكل (i = 1; i < BE_جدول_اندازه; i <<= 1) {
		دوراني_ازموینه = (دوراني_ازموینه << 1) ^ ((دوراني_ازموینه & 0x80000000) ? CRCPOLY_BE : 0);
		لكل (j = 0; j < i; j++)
			crc32table_be[0][i + j] = دوراني_ازموینه ^ crc32table_be[0][j];
	}
	لكل (i = 0; i < BE_جدول_اندازه; i++) {
		دوراني_ازموینه = crc32table_be[0][i];
		لكل (j = 1; j < BE_جدول_لیکې; j++) {
			دوراني_ازموینه = crc32table_be[0][(دوراني_ازموینه >> 24) & 0xff] ^ (دوراني_ازموینه << 8);
			crc32table_be[j][i] = دوراني_ازموینه;
		}
	}
}

ساكن فراغ output_جدول(uint32_t (*جدول)[256], صحيح لیکې, صحيح len, حرف *trans)
{
	صحيح i, j;

	لكل (j = 0 ; j < لیکې; j++) {
		چاپ_کړه("{");
		لكل (i = 0; i < len - 1; i++) {
			إذا (i % ENTRIES_فی_کرښه == 0)
				چاپ_کړه("\n");
			چاپ_کړه("%s(0x%8.8xL), ", trans, جدول[j][i]);
		}
		چاپ_کړه("%s(0x%8.8xL)},\n", trans, جدول[j][len - 1]);
	}
}

صحيح اصلي(صحيح argc, حرف **argv)
{
	چاپ_کړه("/* this file is generated - do not edit */\n\n");

	إذا (دوراني_ازموینه_BE_بټونه > 1) {
		crc32init_be();
		چاپ_کړه("static const uint32_t "
		       "crc32table_be[%d][%d] = {",
		       BE_جدول_لیکې, BE_جدول_اندازه);
		output_جدول(crc32table_be, LE_جدول_لیکې,
			     BE_جدول_اندازه, "tobe");
		چاپ_کړه("};\n");
	}
	إذا (دوراني_ازموینه_LE_بټونه > 1) {
		crc32cinit_le();
		چاپ_کړه("static const uint32_t "
		       "crc32ctable_le[%d][%d] = {",
		       LE_جدول_لیکې, LE_جدول_اندازه);
		output_جدول(crc32ctable_le, LE_جدول_لیکې,
			     LE_جدول_اندازه, "tole");
		چاپ_کړه("};\n");
	}

	أرجع 0;
}
