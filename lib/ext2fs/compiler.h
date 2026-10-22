#اگر_تعریف_نشده _EXT2FS_COMPILER_H
#تعریف_کن _EXT2FS_COMPILER_H

#ضمن <stddef.h>

#اگر_تعریف_شده __GNUC__

#اگر_تعریف_نشده __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کن __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کن __GNUC_PREREQ(maj, min) 0
#پایان_اگر
#پایان_اگر

#تعریف_کن container_از(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (حرف *)__mptr - offsetof(type,member) );})
#وإلا
#تعریف_کن container_از(ptr, type, member)				\
	((type *)((حرف *)(ptr) - offsetof(type, member)))
#پایان_اگر


#پایان_اگر /* _EXT2FS_COMPILER_H */
