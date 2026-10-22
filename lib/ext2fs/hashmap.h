#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#જો defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#નહીંતર
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

સંરચના ext2fs_hashmap;

સંરચના ext2fs_hashmap_entry {
	શૂન્ય *data;
	અચલ શૂન્ય *ચાવી;
	size_t ચાવી_len;
	સંરચના ext2fs_hashmap_entry *next;
	સંરચના ext2fs_hashmap_entry *list_next;
	સંરચના ext2fs_hashmap_entry *list_prev;
};

સંરચના ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(અચલ શૂન્ય*, size_t),
				શૂન્ય(*free_fct)(શૂન્ય*), size_t size);
પૂર્ણાંક ext2fs_hashmap_add(સંરચના ext2fs_hashmap *h,
		       શૂન્ય *data, અચલ શૂન્ય *ચાવી,size_t ચાવી_len);
શૂન્ય *ext2fs_hashmap_lookup(સંરચના ext2fs_hashmap *h, અચલ શૂન્ય *ચાવી,
			    size_t ચાવી_len);
શૂન્ય *ext2fs_hashmap_iter_in_order(સંરચના ext2fs_hashmap *h,
				   સંરચના ext2fs_hashmap_entry **it);
શૂન્ય ext2fs_hashmap_del(સંરચના ext2fs_hashmap *h,
			સંરચના ext2fs_hashmap_entry *e);
શૂન્ય ext2fs_hashmap_free(સંરચના ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(અચલ શૂન્ય *str, size_t size);

#endif /* !HASHMAP_H */
