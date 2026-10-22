#अगर_अघोषित _EXT2FS_COMPILER_H
#घोषणा _EXT2FS_COMPILER_H

#समावेश <stddef.h>

#अगर_घोषित __GNUC__

#अगर_अघोषित __GNUC_PREREQ
#अगर defined(__GNUC__) && defined(__GNUC_MINOR__)
#घोषणा __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#घोषणा __GNUC_PREREQ(maj, min) 0
#पूर्ण_अगर
#पूर्ण_अगर

#घोषणा container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (अक्षर *)__mptr - दुरत्व(type,member) );})
#अन्यथा
#घोषणा container_of(ptr, type, member)				\
	((type *)((अक्षर *)(ptr) - दुरत्व(type, member)))
#पूर्ण_अगर


#पूर्ण_अगर /* _EXT2FS_COMPILER_H */
