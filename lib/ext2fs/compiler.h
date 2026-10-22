#اگر_غیر_تعریف_شدہ _EXT2FS_COMPILER_H
#تعریف_کرو _EXT2FS_COMPILER_H

#شامل <stddef.h>

#اگر_تعریف_شدہ __GNUC__

#اگر_غیر_تعریف_شدہ __GNUC_PREREQ
#اگر defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کرو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ورنہ
#تعریف_کرو __GNUC_PREREQ(maj, min) 0
#اگر_ختم
#اگر_ختم

#تعریف_کرو container_کا(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (حرف *)__mptr - offsetof(type,member) );})
#ورنہ
#تعریف_کرو container_کا(ptr, type, member)				\
	((type *)((حرف *)(ptr) - offsetof(type, member)))
#اگر_ختم


#اگر_ختم /* _EXT2FS_COMPILER_H */
