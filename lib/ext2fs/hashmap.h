#నిర్వచితం_కాకపోతే HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#నిర్వచితం_కాకపోతే __GNUC_PREREQ
#ఒకవేళ defined(__GNUC__) && defined(__GNUC_MINOR__)
#నిర్వచించు __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#లేకపోతే
#నిర్వచించు __GNUC_PREREQ(maj, min) 0
#అయితే_ముగింపు
#అయితే_ముగింపు

నిర్మాణం ext2fs_hashmap;

నిర్మాణం ext2fs_hashmap_నమోదు {
	శూన్య *దత్తాంశం;
	స్థిరరాశి శూన్య *కీలకం;
	size_t కీలకం_len;
	నిర్మాణం ext2fs_hashmap_నమోదు *తదుపరి;
	నిర్మాణం ext2fs_hashmap_నమోదు *list_తదుపరి;
	నిర్మాణం ext2fs_hashmap_నమోదు *list_prev;
};

నిర్మాణం ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(స్థిరరాశి శూన్య*, size_t),
				శూన్య(*free_fct)(శూన్య*), size_t కొలత);
పూర్ణాంకం ext2fs_hashmap_కూడు(నిర్మాణం ext2fs_hashmap *h,
		       శూన్య *దత్తాంశం, స్థిరరాశి శూన్య *కీలకం,size_t కీలకం_len);
శూన్య *ext2fs_hashmap_lookup(నిర్మాణం ext2fs_hashmap *h, స్థిరరాశి శూన్య *కీలకం,
			    size_t కీలకం_len);
శూన్య *ext2fs_hashmap_iter_లోపల_క్రమం(నిర్మాణం ext2fs_hashmap *h,
				   నిర్మాణం ext2fs_hashmap_నమోదు **it);
శూన్య ext2fs_hashmap_del(నిర్మాణం ext2fs_hashmap *h,
			నిర్మాణం ext2fs_hashmap_నమోదు *e);
శూన్య ext2fs_hashmap_free(నిర్మాణం ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(స్థిరరాశి శూన్య *str, size_t కొలత);

#అయితే_ముగింపు /* !HASHMAP_H */
