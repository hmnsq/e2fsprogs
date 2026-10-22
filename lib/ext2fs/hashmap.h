#اگر_غیر_تعریف_شدہ HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#اگر_غیر_تعریف_شدہ __GNUC_PREREQ
#اگر defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کرو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ورنہ
#تعریف_کرو __GNUC_PREREQ(maj, min) 0
#اگر_ختم
#اگر_ختم

ساخت ext2fs_hashmap;

ساخت ext2fs_hashmap_اندراج {
	خالی *ڈیٹا;
	ثابت خالی *کلید;
	size_t کلید_len;
	ساخت ext2fs_hashmap_اندراج *اگلا;
	ساخت ext2fs_hashmap_اندراج *list_اگلا;
	ساخت ext2fs_hashmap_اندراج *list_پچھلا;
};

ساخت ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت خالی*, size_t),
				خالی(*free_fct)(خالی*), size_t سائز);
صحیح ext2fs_hashmap_جوڑو(ساخت ext2fs_hashmap *h,
		       خالی *ڈیٹا, ثابت خالی *کلید,size_t کلید_len);
خالی *ext2fs_hashmap_lookup(ساخت ext2fs_hashmap *h, ثابت خالی *کلید,
			    size_t کلید_len);
خالی *ext2fs_hashmap_iter_اندر_ترتیب(ساخت ext2fs_hashmap *h,
				   ساخت ext2fs_hashmap_اندراج **it);
خالی ext2fs_hashmap_del(ساخت ext2fs_hashmap *h,
			ساخت ext2fs_hashmap_اندراج *e);
خالی ext2fs_hashmap_free(ساخت ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت خالی *str, size_t سائز);

#اگر_ختم /* !HASHMAP_H */
