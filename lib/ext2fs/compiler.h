#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#ಅನ್ತರ್ಭಾವ <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#ಯದಿ defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ಅನ್ಯಥಾ
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (ವರ್ಣ *)__mptr - offsetof(type,member) );})
#ಅನ್ಯಥಾ
#define container_of(ptr, type, member)				\
	((type *)((ವರ್ಣ *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */
