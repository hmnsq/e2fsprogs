#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _EXT2FS_COMPILER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _EXT2FS_COMPILER_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNUC_PREREQ
#ਜੇ defined(__GNUC__) && defined(__GNUC_MINOR__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GNUC_PREREQ(maj, min) 0
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ container_ਦਾ(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (ਅੱਖਰ *)__mptr - offsetof(type,member) );})
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ container_ਦਾ(ptr, type, member)				\
	((type *)((ਅੱਖਰ *)(ptr) - offsetof(type, member)))
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ /* _EXT2FS_COMPILER_H */
