#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#എങ്കിൽ defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#അല്ലെങ്കിൽ
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

ഘടന ext2fs_hashmap;

ഘടന ext2fs_hashmap_entry {
	ശൂന്യം *data;
	സ്ഥിരം ശൂന്യം *താക്കോൽ;
	size_t താക്കോൽ_len;
	ഘടന ext2fs_hashmap_entry *next;
	ഘടന ext2fs_hashmap_entry *list_next;
	ഘടന ext2fs_hashmap_entry *list_prev;
};

ഘടന ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(സ്ഥിരം ശൂന്യം*, size_t),
				ശൂന്യം(*free_fct)(ശൂന്യം*), size_t size);
പൂർണ്ണസംഖ്യ ext2fs_hashmap_add(ഘടന ext2fs_hashmap *h,
		       ശൂന്യം *data, സ്ഥിരം ശൂന്യം *താക്കോൽ,size_t താക്കോൽ_len);
ശൂന്യം *ext2fs_hashmap_lookup(ഘടന ext2fs_hashmap *h, സ്ഥിരം ശൂന്യം *താക്കോൽ,
			    size_t താക്കോൽ_len);
ശൂന്യം *ext2fs_hashmap_iter_in_order(ഘടന ext2fs_hashmap *h,
				   ഘടന ext2fs_hashmap_entry **it);
ശൂന്യം ext2fs_hashmap_del(ഘടന ext2fs_hashmap *h,
			ഘടന ext2fs_hashmap_entry *e);
ശൂന്യം ext2fs_hashmap_free(ഘടന ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(സ്ഥിരം ശൂന്യം *str, size_t size);

#endif /* !HASHMAP_H */
