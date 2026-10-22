#اگر_تعریف_نشده HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#اگر_تعریف_نشده __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کن __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کن __GNUC_PREREQ(maj, min) 0
#پایان_اگر
#پایان_اگر

بنية ext2fs_hashmap;

بنية ext2fs_hashmap_مدخل {
	فراغ *داده;
	ثابت فراغ *کلید;
	size_t کلید_len;
	بنية ext2fs_hashmap_مدخل *بعدی;
	بنية ext2fs_hashmap_مدخل *list_بعدی;
	بنية ext2fs_hashmap_مدخل *list_prev;
};

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t اندازه);
صحيح ext2fs_hashmap_جمع_کن(بنية ext2fs_hashmap *h,
		       فراغ *داده, ثابت فراغ *کلید,size_t کلید_len);
فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *کلید,
			    size_t کلید_len);
فراغ *ext2fs_hashmap_iter_درون_ترتیب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_مدخل **it);
فراغ ext2fs_hashmap_del(بنية ext2fs_hashmap *h,
			بنية ext2fs_hashmap_مدخل *e);
فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t اندازه);

#پایان_اگر /* !HASHMAP_H */
