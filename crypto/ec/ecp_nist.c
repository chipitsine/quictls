/*
 * Copyright 2001-2021 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * ECDSA low level APIs are deprecated for public use, but still ok for
 * internal use.
 */
#include <internal/deprecated.h>

#include <limits.h>

#include <openssl/err.h>
#include <openssl/obj_mac.h>
#include "ec_local.h"

const EC_METHOD *EC_GFp_nist_method(void)
{
    static const EC_METHOD ret = {
        .field_type = NID_X9_62_prime_field,
        .group_init = ossl_ec_GFp_simple_group_init,
        .group_finish = ossl_ec_GFp_simple_group_finish,
        .group_clear_finish = ossl_ec_GFp_simple_group_clear_finish,
        .group_copy = ossl_ec_GFp_nist_group_copy,
        .group_set_curve = ossl_ec_GFp_nist_group_set_curve,
        .group_get_curve = ossl_ec_GFp_simple_group_get_curve,
        .group_get_degree = ossl_ec_GFp_simple_group_get_degree,
        .group_order_bits = ossl_ec_group_simple_order_bits,
        .group_check_discriminant = ossl_ec_GFp_simple_group_check_discriminant,
        .point_init = ossl_ec_GFp_simple_point_init,
        .point_finish = ossl_ec_GFp_simple_point_finish,
        .point_clear_finish = ossl_ec_GFp_simple_point_clear_finish,
        .point_copy = ossl_ec_GFp_simple_point_copy,
        .point_set_to_infinity = ossl_ec_GFp_simple_point_set_to_infinity,
        .point_set_affine_coordinates =
            ossl_ec_GFp_simple_point_set_affine_coordinates,
        .point_get_affine_coordinates =
            ossl_ec_GFp_simple_point_get_affine_coordinates,
        .add = ossl_ec_GFp_simple_add,
        .dbl = ossl_ec_GFp_simple_dbl,
        .invert = ossl_ec_GFp_simple_invert,
        .is_at_infinity = ossl_ec_GFp_simple_is_at_infinity,
        .is_on_curve = ossl_ec_GFp_simple_is_on_curve,
        .point_cmp = ossl_ec_GFp_simple_cmp,
        .make_affine = ossl_ec_GFp_simple_make_affine,
        .points_make_affine = ossl_ec_GFp_simple_points_make_affine,
        .mul = NULL,
        .precompute_mult = NULL,
        .have_precompute_mult = NULL,
        .field_mul = ossl_ec_GFp_nist_field_mul,
        .field_sqr = ossl_ec_GFp_nist_field_sqr,
        .field_inv = ossl_ec_GFp_simple_field_inv,
        .field_encode = NULL,
        .field_decode = NULL,
        .field_set_to_one = NULL,
        .priv2oct = ossl_ec_key_simple_priv2oct,
        .oct2priv = ossl_ec_key_simple_oct2priv,
        .keygen = ossl_ec_key_simple_generate_key,
        .keycheck = ossl_ec_key_simple_check_key,
        .keygenpub = ossl_ec_key_simple_generate_public_key,
        .ecdh_compute_key = ossl_ecdh_simple_compute_key,
        .ecdsa_sign_setup = ossl_ecdsa_simple_sign_setup,
        .ecdsa_sign_sig = ossl_ecdsa_simple_sign_sig,
        .ecdsa_verify_sig = ossl_ecdsa_simple_verify_sig,
        .field_inverse_mod_ord = NULL,
        .blind_coordinates = ossl_ec_GFp_simple_blind_coordinates,
        .ladder_pre = ossl_ec_GFp_simple_ladder_pre,
        .ladder_step = ossl_ec_GFp_simple_ladder_step,
        .ladder_post = ossl_ec_GFp_simple_ladder_post,
    };

    return &ret;
}

int ossl_ec_GFp_nist_group_copy(EC_GROUP *dest, const EC_GROUP *src)
{
    dest->field_mod_func = src->field_mod_func;

    return ossl_ec_GFp_simple_group_copy(dest, src);
}

int ossl_ec_GFp_nist_group_set_curve(EC_GROUP *group, const BIGNUM *p,
                                     const BIGNUM *a, const BIGNUM *b,
                                     BN_CTX *ctx)
{
    int ret = 0;
    BN_CTX *new_ctx = NULL;

    if (ctx == NULL)
        if ((ctx = new_ctx = BN_CTX_new_ex(group->libctx)) == NULL)
            return 0;

    BN_CTX_start(ctx);

    if (BN_ucmp(BN_get0_nist_prime_192(), p) == 0)
        group->field_mod_func = BN_nist_mod_192;
    else if (BN_ucmp(BN_get0_nist_prime_224(), p) == 0)
        group->field_mod_func = BN_nist_mod_224;
    else if (BN_ucmp(BN_get0_nist_prime_256(), p) == 0)
        group->field_mod_func = BN_nist_mod_256;
    else if (BN_ucmp(BN_get0_nist_prime_384(), p) == 0)
        group->field_mod_func = BN_nist_mod_384;
    else if (BN_ucmp(BN_get0_nist_prime_521(), p) == 0)
        group->field_mod_func = BN_nist_mod_521;
    else {
        ERR_raise(ERR_LIB_EC, EC_R_NOT_A_NIST_PRIME);
        goto err;
    }

    ret = ossl_ec_GFp_simple_group_set_curve(group, p, a, b, ctx);

 err:
    BN_CTX_end(ctx);
    BN_CTX_free(new_ctx);
    return ret;
}

int ossl_ec_GFp_nist_field_mul(const EC_GROUP *group, BIGNUM *r, const BIGNUM *a,
                               const BIGNUM *b, BN_CTX *ctx)
{
    int ret = 0;
    BN_CTX *ctx_new = NULL;

    if (!group || !r || !a || !b) {
        ERR_raise(ERR_LIB_EC, ERR_R_PASSED_NULL_PARAMETER);
        goto err;
    }
    if (!ctx)
        if ((ctx_new = ctx = BN_CTX_new_ex(group->libctx)) == NULL)
            goto err;

    if (!BN_mul(r, a, b, ctx))
        goto err;
    if (!group->field_mod_func(r, r, group->field, ctx))
        goto err;

    ret = 1;
 err:
    BN_CTX_free(ctx_new);
    return ret;
}

int ossl_ec_GFp_nist_field_sqr(const EC_GROUP *group, BIGNUM *r, const BIGNUM *a,
                               BN_CTX *ctx)
{
    int ret = 0;
    BN_CTX *ctx_new = NULL;

    if (!group || !r || !a) {
        ERR_raise(ERR_LIB_EC, EC_R_PASSED_NULL_PARAMETER);
        goto err;
    }
    if (!ctx)
        if ((ctx_new = ctx = BN_CTX_new_ex(group->libctx)) == NULL)
            goto err;

    if (!BN_sqr(r, a, ctx))
        goto err;
    if (!group->field_mod_func(r, r, group->field, ctx))
        goto err;

    ret = 1;
 err:
    BN_CTX_free(ctx_new);
    return ret;
}
