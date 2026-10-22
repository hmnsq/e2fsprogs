#جے_بنا_تعریف HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#جے_بنا_تعریف __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کرو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کرو __GNUC_PREREQ(maj, min) 0
#جے_انت
#جے_انت

بنية ext2fs_hashmap;

بنية ext2fs_hashmap_اندراج {
	فراغ *انکڑے;
	ثابت فراغ *کلید;
	size_t کلید_len;
	بنية ext2fs_hashmap_اندراج *اگلا;
	بنية ext2fs_hashmap_اندراج *list_اگلا;
	بنية ext2fs_hashmap_اندراج *list_prev;
};

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t ناپ);
صحيح ext2fs_hashmap_جوڑو(بنية ext2fs_hashmap *h,
		       فراغ *انکڑے, ثابت فراغ *کلید,size_t کلید_len);
فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *کلید,
			    size_t کلید_len);
فراغ *ext2fs_hashmap_iter_وچ_ترتیب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_اندراج **it);
فراغ ext2fs_hashmap_del(بنية ext2fs_hashmap *h,
			بنية ext2fs_hashmap_اندراج *e);
فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t ناپ);

#جے_انت /* !HASHMAP_H */
