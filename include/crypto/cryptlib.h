/*
 * Copyright 2016-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_CRYPTO_CRYPTLIB_H
# define OSSL_CRYPTO_CRYPTLIB_H

# include <openssl/core.h>
# include <internal/cryptlib.h>

int ossl_init_thread_start(const void *index, void *arg,
                           OSSL_thread_stop_handler_fn handfn);
int ossl_init_thread_deregister(void *index);
int ossl_init_thread(void);
void ossl_cleanup_thread(void);
void ossl_ctx_thread_stop(OSSL_LIB_CTX *ctx);

/*
 * OPENSSL_INIT flags. The primary list of these is in crypto.h. Flags below
 * are those omitted from crypto.h because they are "reserved for internal
 * use".
 */
# define OPENSSL_INIT_BASE_ONLY              0x00040000L

int ossl_crypto_alloc_ex_data_intern(int class_index, void *obj,
                                     CRYPTO_EX_DATA *ad, int idx);

#endif  /* OSSL_CRYPTO_CRYPTLIB_H */
