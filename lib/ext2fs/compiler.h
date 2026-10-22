#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#அந்தர்பாவ <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#எனில் defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#இல்லையெனில்
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (எழுத்து *)__mptr - offsetof(type,member) );})
#இல்லையெனில்
#define container_of(ptr, type, member)				\
	((type *)((எழுத்து *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */
