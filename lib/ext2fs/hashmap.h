#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNUC_PREREQ
#ಆದರೆ defined(__GNUC__) && defined(__GNUC_MINOR__)
#ವ್ಯಾಖ್ಯಾನಿಸು __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __GNUC_PREREQ(maj, min) 0
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ ext2fs_hashmap;

ರಚನೆ ext2fs_hashmap_ನಮೂದು {
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ;
	size_t ಕೀಲಿ_len;
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *ಮುಂದಿನ;
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *list_ಮುಂದಿನ;
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *list_prev;
};

ರಚನೆ ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t),
				ಶೂನ್ಯ(*free_fct)(ಶೂನ್ಯ*), size_t ಗಾತ್ರ_2);
ಪೂರ್ಣಾಂಕ ext2fs_hashmap_ಕೂಡಿಸು(ರಚನೆ ext2fs_hashmap *h,
		       ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ,size_t ಕೀಲಿ_len);
ಶೂನ್ಯ *ext2fs_hashmap_lookup(ರಚನೆ ext2fs_hashmap *h, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ,
			    size_t ಕೀಲಿ_len);
ಶೂನ್ಯ *ext2fs_hashmap_iter_ಒಳ_ಕ್ರಮ(ರಚನೆ ext2fs_hashmap *h,
				   ರಚನೆ ext2fs_hashmap_ನಮೂದು **it);
ಶೂನ್ಯ ext2fs_hashmap_del(ರಚನೆ ext2fs_hashmap *h,
			ರಚನೆ ext2fs_hashmap_ನಮೂದು *e);
ಶೂನ್ಯ ext2fs_hashmap_free(ರಚನೆ ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *str, size_t ಗಾತ್ರ_2);

#ಆದರೆ_ಅಂತ್ಯ /* !HASHMAP_H */
