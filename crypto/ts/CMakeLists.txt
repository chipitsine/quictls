set(SOURCE
    ts_err.c ts_req_utils.c ts_req_print.c ts_rsp_utils.c ts_rsp_print.c
    ts_rsp_sign.c ts_rsp_verify.c ts_verify_ctx.c ts_lib.c ts_conf.c
    ts_asn1.c
    )
target_sources(crypto PRIVATE ${SOURCE})
