set(SOURCE
    ui_err.c ui_lib.c ui_openssl.c ui_null.c ui_util.c
    )
target_sources(crypto PRIVATE ${SOURCE})
