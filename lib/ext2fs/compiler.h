#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#include <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#ถ้า defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#มิฉะนั้น
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (var_na *)__mptr - offsetof(type,member) );})
#มิฉะนั้น
#define container_of(ptr, type, member)				\
	((type *)((var_na *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */
