set(SOURCE
    conf_err.c conf_lib.c conf_api.c conf_def.c
    conf_mod.c conf_mall.c conf_sap.c conf_ssl.c
    )
target_sources(crypto PRIVATE ${SOURCE})
