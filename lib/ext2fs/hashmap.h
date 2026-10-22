#که_نه_تعریف_شوی HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#که_نه_تعریف_شوی __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کړه __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کړه __GNUC_PREREQ(maj, min) 0
#د_که_پای
#د_که_پای

بنية ext2fs_hashmap;

بنية ext2fs_hashmap_ثبت {
	فراغ *معلومات;
	ثابت فراغ *کلی;
	size_t کلی_len;
	بنية ext2fs_hashmap_ثبت *بل;
	بنية ext2fs_hashmap_ثبت *list_بل;
	بنية ext2fs_hashmap_ثبت *list_prev;
};

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t اندازه);
صحيح ext2fs_hashmap_جمع_کړه(بنية ext2fs_hashmap *h,
		       فراغ *معلومات, ثابت فراغ *کلی,size_t کلی_len);
فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *کلی,
			    size_t کلی_len);
فراغ *ext2fs_hashmap_iter_دننه_ترتیب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_ثبت **it);
فراغ ext2fs_hashmap_del(بنية ext2fs_hashmap *h,
			بنية ext2fs_hashmap_ثبت *e);
فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t اندازه);

#د_که_پای /* !HASHMAP_H */
