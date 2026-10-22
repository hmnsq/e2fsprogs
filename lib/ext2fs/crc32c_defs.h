/*
 * There are multiple 16-bit CRC polynomials in common use, but this is
 * *the* standard CRC-32 polynomial, first popularized by Ethernet.
 * x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x^1+x^0
 */
#وصف_ڪريو CRCPOLY_LE 0xedb88320
#وصف_ڪريو CRCPOLY_BE 0x04c11db7

/*
 * This is the CRC32c polynomial, as outlined by Castagnoli.
 * x^32+x^28+x^27+x^26+x^25+x^23+x^22+x^20+x^19+x^18+x^14+x^13+x^11+x^10+x^9+
 * x^8+x^6+x^0
 */
#وصف_ڪريو چڪري_جاچ_32_C_POLY_LE 0x82F63B78
#وصف_ڪريو چڪري_جاچ_32_C_POLY_BE 0x1EDC6F41

/* How many bits at a time to use.  Valid values are 1, 2, 4, 8, 32 and 64. */
/* For less performance-sensitive, use 4 */
#جيڪڏهن_وصف_نه_ٿيل چڪري_جاچ_LE_بٽ
# define چڪري_جاچ_LE_بٽ 64
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل چڪري_جاچ_BE_بٽ
# define چڪري_جاچ_BE_بٽ 64
#جيڪڏهن_پورو

/*
 * Little-endian CRC computation.  Used with serial bit streams sent
 * lsbit-first.  Be sure to use cpu_to_le32() to append the computed CRC.
 */
#إذا چڪري_جاچ_LE_بٽ > 64 || چڪري_جاچ_LE_بٽ < 1 || چڪري_جاچ_LE_بٽ == 16 || \
	چڪري_جاچ_LE_بٽ & چڪري_جاچ_LE_بٽ-1
# error "CRC_LE_BITS must be one of {1, 2, 4, 8, 32, 64}"
#جيڪڏهن_پورو

/*
 * Big-endian CRC computation.  Used with serial bit streams sent
 * msbit-first.  Be sure to use cpu_to_be32() to append the computed CRC.
 */
#إذا چڪري_جاچ_BE_بٽ > 64 || چڪري_جاچ_BE_بٽ < 1 || چڪري_جاچ_BE_بٽ == 16 || \
	چڪري_جاچ_BE_بٽ & چڪري_جاچ_BE_بٽ-1
# error "CRC_BE_BITS must be one of {1, 2, 4, 8, 32, 64}"
#جيڪڏهن_پورو


#وصف_ڪريو ___constant_swab32(x) \
	((uint32_t)( \
		(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
		(((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
		(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
		(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))


#إذا (__GNUC__ >= 3)
#وصف_ڪريو likely(x)	__builtin_expect(!!(x), 1)
#وصف_ڪريو unlikely(x)	__builtin_expect(!!(x), 0)
#وإلا
#وصف_ڪريو likely(x)	(x)
#وصف_ڪريو unlikely(x)	(x)
#جيڪڏهن_پورو
