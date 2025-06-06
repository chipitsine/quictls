/*
 * Copyright 2016-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_CRYPTO_DSO_CONF_H
# define OSSL_CRYPTO_DSO_CONF_H

# include <internal/paths.h>
# if defined(HAVE_FCNTL_HEADER)
#  include <fcntl.h>
# endif
# if defined(HAVE_DLFCN_HEADER)
#  include <dlfcn.h>
# endif

# define DSO_DLFCN
# define HAVE_DLFCN_H

# define DSO_EXTENSION  SHARED_LIB_EXT
#endif
