/*
 * Copyright 1995-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_INTERNAL_COMMON_H
# define OSSL_INTERNAL_COMMON_H

# include <stdlib.h>
# include <string.h>
# include <openssl/configuration.h>
# include <internal/e_os.h>
# include <internal/nelem.h>

# if defined(__GNUC__) || defined(__clang__)
#  define ossl_likely(x)     __builtin_expect(!!(x), 1)
#  define ossl_unlikely(x)   __builtin_expect(!!(x), 0)
# else
#  define ossl_likely(x)     x
#  define ossl_unlikely(x)   x
# endif

# if defined(__GNUC__) || defined(__clang__)
#  define ALIGN32       __attribute((aligned(32)))
#  define ALIGN64       __attribute((aligned(64)))
# elif defined(_MSC_VER)
#  define ALIGN32       __declspec(align(32))
#  define ALIGN64       __declspec(align(64))
# else
#  define ALIGN32
#  define ALIGN64
# endif

# ifdef NDEBUG
#  define ossl_assert(x) ossl_likely((x) != 0)
# else
__owur static inline int ossl_assert_int(int expr, const char *exprstr,
                                              const char *file, int line)
{
    if (!expr)
        OPENSSL_die(exprstr, file, line);

    return expr;
}

#  define ossl_assert(x) ossl_assert_int((x) != 0, "Assertion failed: "#x, \
                                         __FILE__, __LINE__)

# endif

/* Check if |pre|, which must be a string literal, is a prefix of |str| */
#define HAS_PREFIX(str, pre) (strncmp(str, pre "", sizeof(pre) - 1) == 0)
/* As before, and if check succeeds, advance |str| past the prefix |pre| */
#define CHECK_AND_SKIP_PREFIX(str, pre) \
    (HAS_PREFIX(str, pre) ? ((str) += sizeof(pre) - 1, 1) : 0)
/* Check if the string literal |p| is a case-insensitive prefix of |s| */
#define HAS_CASE_PREFIX(s, p) (OPENSSL_strncasecmp(s, p "", sizeof(p) - 1) == 0)
/* As before, and if check succeeds, advance |str| past the prefix |pre| */
#define CHECK_AND_SKIP_CASE_PREFIX(str, pre) \
    (HAS_CASE_PREFIX(str, pre) ? ((str) += sizeof(pre) - 1, 1) : 0)
/* Check if the string literal |suffix| is a case-insensitive suffix of |str| */
#define HAS_CASE_SUFFIX(str, suffix) (strlen(str) < sizeof(suffix) - 1 ? 0 : \
    OPENSSL_strcasecmp(str + strlen(str) - sizeof(suffix) + 1, suffix "") == 0)

/*
 * Use this inside a union with the field that needs to be aligned to a
 * reasonable boundary for the platform.  The most pessimistic alignment
 * of the listed types will be used by the compiler.
 */
# define OSSL_UNION_ALIGN       \
    double align;               \
    uintmax_t align_int;        \
    void *align_ptr

# define OPENSSL_CONF             "openssl.cnf"

# define X509_CERT_AREA          OPENSSLDIR
# define X509_CERT_DIR           OPENSSLDIR "/certs"
# define X509_CERT_FILE          OPENSSLDIR "/cert.pem"
# define X509_PRIVATE_DIR        OPENSSLDIR "/private"
# define CTLOG_FILE              OPENSSLDIR "/ct_log_list.cnf"

# define X509_CERT_DIR_EVP        "SSL_CERT_DIR"
# define X509_CERT_FILE_EVP       "SSL_CERT_FILE"
# define CTLOG_FILE_EVP           "CTLOG_FILE"

/* size of string representations */
# define DECIMAL_SIZE(type)      ((sizeof(type)*8+2)/3+1)
# define HEX_SIZE(type)          (sizeof(type)*2)

/*
 * Loop rolling/unrolling, used by some cipher implementations.
 * Note that all of these evaluate their parameters multiple
 * times. We could remove the parens around some of them,
 * like "(c)" etc., but don't. Many of the macros use the comma
 * to get a "sequence point" to ensure that the right bytes
 * are shifted with the right amounts.
 */

# define n2s(c, s) \
    ((s)  = ((unsigned int)(*((c)++))) << 8, \
     (s) |= ((unsigned int)(*((c)++)))     )

# define s2n(s, c) \
    (*((c)++) = (unsigned char)(((s) >> 8) & 0xff), \
     *((c)++) = (unsigned char)(((s)     ) & 0xff))

# define n2l3(c, l) \
    ((l)  = ((unsigned long)(*((c)++)) << 16), \
     (l) |= ((unsigned long)(*((c)++)) <<  8), \
     (l) |= ((unsigned long)(*((c)++))      ))

# define l2n3(l, c) \
    (*((c)++) = (unsigned char)(((l) >> 16) & 0xff), \
     *((c)++) = (unsigned char)(((l) >>  8) & 0xff), \
     *((c)++) = (unsigned char)(((l)      ) & 0xff))

# define c2lXXX(c, l, T) \
    ((l)  =  ((T)(*((c)++))       ), \
     (l) |= (((T)(*((c)++))) <<  8), \
     (l) |= (((T)(*((c)++))) << 16), \
     (l) |= (((T)(*((c)++))) << 24))

# define c2l(c, l) c2lXXX(c, l, unsigned long)

# define l2c(l, c) \
    (*((c)++) = (unsigned char)(((l)      ) & 0xff), \
     *((c)++) = (unsigned char)(((l) >>  8) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 16) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 24) & 0xff))

# define n2l(c, l) \
    ((l)  = ((unsigned long)(*((c)++))) << 24, \
     (l) |= ((unsigned long)(*((c)++))) << 16, \
     (l) |= ((unsigned long)(*((c)++))) <<  8, \
     (l) |= ((unsigned long)(*((c)++)))      )

# define l2n(l, c) \
    (*((c)++) = (unsigned char)(((l) >> 24) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 16) & 0xff), \
     *((c)++) = (unsigned char)(((l) >>  8) & 0xff), \
     *((c)++) = (unsigned char)(((l)      ) & 0xff))

# define n2l8(c, l) \
    ((l)  = ((uint64_t)(*((c)++))) << 56, \
     (l) |= ((uint64_t)(*((c)++))) << 48, \
     (l) |= ((uint64_t)(*((c)++))) << 40, \
     (l) |= ((uint64_t)(*((c)++))) << 32, \
     (l) |= ((uint64_t)(*((c)++))) << 24, \
     (l) |= ((uint64_t)(*((c)++))) << 16, \
     (l) |= ((uint64_t)(*((c)++))) <<  8, \
     (l) |= ((uint64_t)(*((c)++)))      )

# define l2n8(l, c) \
    (*((c)++) = (unsigned char)(((l) >> 56) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 48) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 40) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 32) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 24) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 16) & 0xff), \
     *((c)++) = (unsigned char)(((l) >>  8) & 0xff), \
     *((c)++) = (unsigned char)(((l)      ) & 0xff))

# define c2lnXXX(c, l1, l2, n, T) \
    { \
        (c) += (n); \
        (l1) = (l2) = 0; \
        switch ((n)) { \
        case 8: (l2)  = ((T)(*(--(c)))) << 24; \
                /* fallthrough */ \
        case 7: (l2) |= ((T)(*(--(c)))) << 16; \
                /* fallthrough */ \
        case 6: (l2) |= ((T)(*(--(c)))) <<  8; \
                /* fallthrough */ \
        case 5: (l2) |= ((T)(*(--(c))))      ; \
                /* fallthrough */ \
        case 4: (l1)  = ((T)(*(--(c)))) << 24; \
                /* fallthrough */ \
        case 3: (l1) |= ((T)(*(--(c)))) << 16; \
                /* fallthrough */ \
        case 2: (l1) |= ((T)(*(--(c)))) <<  8; \
                /* fallthrough */ \
        case 1: (l1) |= ((T)(*(--(c))))      ; \
        } \
    }
# define c2ln(c, l1, l2, n) c2lnXXX(c, l1, l2, n, unsigned long)

# define l2cn(l1, l2, c, n) \
    { \
        (c) += (n); \
        switch ((n)) { \
        case 8: *(--(c)) = (unsigned char)(((l2) >> 24) & 0xff); \
                /* fallthrough */ \
        case 7: *(--(c)) = (unsigned char)(((l2) >> 16) & 0xff); \
                /* fallthrough */ \
        case 6: *(--(c)) = (unsigned char)(((l2) >>  8) & 0xff); \
                /* fallthrough */ \
        case 5: *(--(c)) = (unsigned char)(((l2)      ) & 0xff); \
                /* fallthrough */ \
        case 4: *(--(c)) = (unsigned char)(((l1) >> 24) & 0xff); \
                /* fallthrough */ \
        case 3: *(--(c)) = (unsigned char)(((l1) >> 16) & 0xff); \
                /* fallthrough */ \
        case 2: *(--(c)) = (unsigned char)(((l1) >>  8) & 0xff); \
                /* fallthrough */ \
        case 1: *(--(c)) = (unsigned char)(((l1)      ) & 0xff); \
        } \
    }

# define n2ln(c, l1, l2, n) \
    { \
        (c) += (n); \
        (l1) = (l2) = 0; \
        switch ((n)) { \
        case 8: (l2)  = ((unsigned long)(*(--(c))))      ; \
                /* fallthrough */ \
        case 7: (l2) |= ((unsigned long)(*(--(c)))) <<  8; \
                /* fallthrough */ \
        case 6: (l2) |= ((unsigned long)(*(--(c)))) << 16; \
                /* fallthrough */ \
        case 5: (l2) |= ((unsigned long)(*(--(c)))) << 24; \
                /* fallthrough */ \
        case 4: (l1)  = ((unsigned long)(*(--(c))))      ; \
                /* fallthrough */ \
        case 3: (l1) |= ((unsigned long)(*(--(c)))) <<  8; \
                /* fallthrough */ \
        case 2: (l1) |= ((unsigned long)(*(--(c)))) << 16; \
                /* fallthrough */ \
        case 1: (l1) |= ((unsigned long)(*(--(c)))) << 24; \
        } \
    }

# define l2nn(l1, l2, c, n) \
    { \
        (c) += (n); \
        switch ((n)) { \
        case 8: *(--(c)) = (unsigned char)(((l2)      ) & 0xff); \
                /* fallthrough */ \
        case 7: *(--(c)) = (unsigned char)(((l2) >>  8) & 0xff); \
                /* fallthrough */ \
        case 6: *(--(c)) = (unsigned char)(((l2) >> 16) & 0xff); \
                /* fallthrough */ \
        case 5: *(--(c)) = (unsigned char)(((l2) >> 24) & 0xff); \
                /* fallthrough */ \
        case 4: *(--(c)) = (unsigned char)(((l1)      ) & 0xff); \
                /* fallthrough */ \
        case 3: *(--(c)) = (unsigned char)(((l1) >>  8) & 0xff); \
                /* fallthrough */ \
        case 2: *(--(c)) = (unsigned char)(((l1) >> 16) & 0xff); \
                /* fallthrough */ \
        case 1: *(--(c)) = (unsigned char)(((l1) >> 24) & 0xff); \
        } \
    }

static inline int ossl_ends_with_dirsep(const char *path)
{
    if (*path != '\0')
        path += strlen(path) - 1;
# if   defined _WIN32
    if (*path == '\\')
        return 1;
# endif
    return *path == '/';
}

static inline int ossl_is_absolute_path(const char *path)
{
# if   defined _WIN32
    /* Not worth checking for uppercase letter before the colon */
    if (path[0] == '\\' || (path[0] != '\0' && path[1] == ':'))
        return 1;
# endif
    return path[0] == '/';
}

#endif
