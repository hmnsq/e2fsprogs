#अगर_अघोषित HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#अगर_अघोषित __GNUC_PREREQ
#अगर defined(__GNUC__) && defined(__GNUC_MINOR__)
#घोषणा __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#घोषणा __GNUC_PREREQ(maj, min) 0
#पूर्ण_अगर
#पूर्ण_अगर

काष्ठा ext2fs_hashmap;

काष्ठा ext2fs_hashmap_entry {
	व्योम *तथ्य;
	स्थिर व्योम *कुंजी;
	माप_प्रकार कुंजी_len;
	काष्ठा ext2fs_hashmap_entry *अग्र;
	काष्ठा ext2fs_hashmap_entry *list_अग्र;
	काष्ठा ext2fs_hashmap_entry *list_पूर्व;
};

काष्ठा ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(स्थिर व्योम*, माप_प्रकार),
				व्योम(*free_fct)(व्योम*), माप_प्रकार आकार);
पूर्णांक ext2fs_hashmap_add(काष्ठा ext2fs_hashmap *h,
		       व्योम *तथ्य, स्थिर व्योम *कुंजी,माप_प्रकार कुंजी_len);
व्योम *ext2fs_hashmap_lookup(काष्ठा ext2fs_hashmap *h, स्थिर व्योम *कुंजी,
			    माप_प्रकार कुंजी_len);
व्योम *ext2fs_hashmap_iter_in_order(काष्ठा ext2fs_hashmap *h,
				   काष्ठा ext2fs_hashmap_entry **it);
व्योम ext2fs_hashmap_del(काष्ठा ext2fs_hashmap *h,
			काष्ठा ext2fs_hashmap_entry *e);
व्योम ext2fs_hashmap_free(काष्ठा ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(स्थिर व्योम *str, माप_प्रकार आकार);

#पूर्ण_अगर /* !HASHMAP_H */
