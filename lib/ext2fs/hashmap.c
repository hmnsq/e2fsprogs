#ضمن "hashmap.h"
#ضمن <string.h>

بنية ext2fs_hashmap {
	uint32_t اندازه;
	uint32_t(*hash)(ثابت فراغ *کلید, size_t len);
	فراغ(*حرر)(فراغ*);
	بنية ext2fs_hashmap_مدخل *اولی;
	بنية ext2fs_hashmap_مدخل *آخرین;
#إذا __GNUC_PREREQ (4, 8)
#رهنمود GCC diagnostic افزودن
#رهنمود GCC diagnostic ignored "-Wpedantic"
#پایان_اگر
	بنية ext2fs_hashmap_مدخل *entries[0];
#إذا __GNUC_PREREQ (4, 8)
#رهنمود GCC diagnostic برداشت
#پایان_اگر
};

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t اندازه)
{
	صحيح c;
	ثابت حرف *s = str;
	uint32_t hash = 5381;

	طالما (اندازه-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	أرجع hash;
}

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t اندازه)
{
	بنية ext2fs_hashmap *h = حافظه_صفر_بده(1, حجم(بنية ext2fs_hashmap) +
				حجم(بنية ext2fs_hashmap_مدخل) * اندازه);
	إذا (!h)
		أرجع عدم;

	h->اندازه = اندازه;
	h->حرر = free_fct;
	h->hash = hash_fct;
	h->اولی = h->آخرین = عدم;
	أرجع h;
}

صحيح ext2fs_hashmap_جمع_کن(بنية ext2fs_hashmap *h,
		       فراغ *داده, ثابت فراغ *کلید, size_t کلید_len)
{
	uint32_t hash = h->hash(کلید, کلید_len) % h->اندازه;
	بنية ext2fs_hashmap_مدخل *e = حافظه_بده(حجم(*e));

	إذا (!e)
		أرجع -1;

	e->داده = داده;
	e->کلید = کلید;
	e->کلید_len = کلید_len;
	e->بعدی = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = عدم;
	e->list_بعدی = h->اولی;
	إذا (h->اولی)
		h->اولی->list_prev = e;
	h->اولی = e;
	إذا (!h->آخرین)
		h->آخرین = e;

	أرجع 0;
}

فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *کلید,
			    size_t کلید_len)
{
	بنية ext2fs_hashmap_مدخل *iter;
	uint32_t hash = h->hash(کلید, کلید_len) % h->اندازه;

	لكل (iter = h->entries[hash]; iter; iter = iter->بعدی)
		إذا (iter->کلید_len == کلید_len && !memcmp(iter->کلید, کلید, کلید_len))
			أرجع iter->داده;
	أرجع عدم;
}

فراغ *ext2fs_hashmap_iter_درون_ترتیب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_مدخل **it)
{
	*it = *it ? (*it)->list_بعدی : h->اولی;
	أرجع *it ? (*it)->داده : عدم;
}

فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h)
{
	size_t	i;

	لكل (i = 0; i < h->اندازه; ++i) {
		بنية ext2fs_hashmap_مدخل *it = h->entries[i];
		طالما (it) {
			بنية ext2fs_hashmap_مدخل *tmp = it->بعدی;
			إذا (h->حرر)
				h->حرر(it->داده);
			حرر(it);
			it = tmp;
		}
	}
	حرر(h);
}
