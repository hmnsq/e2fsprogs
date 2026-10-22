#यदि_अपरिभाषितम् _EXT2FS_COMPILER_H
#परिभाषय _EXT2FS_COMPILER_H

#अन्तर्भाव <stddef.h>

#यदि_परिभाषितम् __GNUC__

#यदि_अपरिभाषितम् __GNUC_PREREQ
#यदि defined(__GNUC__) && defined(__GNUC_MINOR__)
#परिभाषय __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#परिभाषय __GNUC_PREREQ(maj, min) 0
#यदि_अन्तः
#यदि_अन्तः

#परिभाषय container_सम्बन्धि(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (वर्ण *)__mptr - offsetof(type,member) );})
#अन्यथा
#परिभाषय container_सम्बन्धि(ptr, type, member)				\
	((type *)((वर्ण *)(ptr) - offsetof(type, member)))
#यदि_अन्तः


#यदि_अन्तः /* _EXT2FS_COMPILER_H */
