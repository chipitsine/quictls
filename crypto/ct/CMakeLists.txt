set(SOURCE
    ct_b64.c ct_err.c ct_log.c ct_oct.c ct_policy.c
    ct_prn.c ct_sct.c ct_sct_ctx.c ct_vfy.c ct_x509v3.c
    )
target_sources(crypto PRIVATE ${SOURCE})
