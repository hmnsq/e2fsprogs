#ضمن "hashmap.h"
#ضمن <string.h>

بنية ext2fs_hashmap {
	uint32_t اندازه;
	uint32_t(*hash)(ثابت فراغ *کلی, size_t len);
	فراغ(*حرر)(فراغ*);
	بنية ext2fs_hashmap_ثبت *لومړی;
	بنية ext2fs_hashmap_ثبت *وروستی;
#إذا __GNUC_PREREQ (4, 8)
#لارښوونه GCC diagnostic ټېل
#لارښوونه GCC diagnostic ignored "-Wpedantic"
#د_که_پای
	بنية ext2fs_hashmap_ثبت *entries[0];
#إذا __GNUC_PREREQ (4, 8)
#لارښوونه GCC diagnostic راوباسه
#د_که_پای
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
	بنية ext2fs_hashmap *h = حافظه_صفر_ورکړه(1, حجم(بنية ext2fs_hashmap) +
				حجم(بنية ext2fs_hashmap_ثبت) * اندازه);
	إذا (!h)
		أرجع عدم;

	h->اندازه = اندازه;
	h->حرر = free_fct;
	h->hash = hash_fct;
	h->لومړی = h->وروستی = عدم;
	أرجع h;
}

صحيح ext2fs_hashmap_جمع_کړه(بنية ext2fs_hashmap *h,
		       فراغ *معلومات, ثابت فراغ *کلی, size_t کلی_len)
{
	uint32_t hash = h->hash(کلی, کلی_len) % h->اندازه;
	بنية ext2fs_hashmap_ثبت *e = حافظه_ورکړه(حجم(*e));

	إذا (!e)
		أرجع -1;

	e->معلومات = معلومات;
	e->کلی = کلی;
	e->کلی_len = کلی_len;
	e->بل = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = عدم;
	e->list_بل = h->لومړی;
	إذا (h->لومړی)
		h->لومړی->list_prev = e;
	h->لومړی = e;
	إذا (!h->وروستی)
		h->وروستی = e;

	أرجع 0;
}

فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *کلی,
			    size_t کلی_len)
{
	بنية ext2fs_hashmap_ثبت *iter;
	uint32_t hash = h->hash(کلی, کلی_len) % h->اندازه;

	لكل (iter = h->entries[hash]; iter; iter = iter->بل)
		إذا (iter->کلی_len == کلی_len && !memcmp(iter->کلی, کلی, کلی_len))
			أرجع iter->معلومات;
	أرجع عدم;
}

فراغ *ext2fs_hashmap_iter_دننه_ترتیب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_ثبت **it)
{
	*it = *it ? (*it)->list_بل : h->لومړی;
	أرجع *it ? (*it)->معلومات : عدم;
}

فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h)
{
	size_t	i;

	لكل (i = 0; i < h->اندازه; ++i) {
		بنية ext2fs_hashmap_ثبت *it = h->entries[i];
		طالما (it) {
			بنية ext2fs_hashmap_ثبت *tmp = it->بل;
			إذا (h->حرر)
				h->حرر(it->معلومات);
			حرر(it);
			it = tmp;
		}
	}
	حرر(h);
}
