set(SOURCE
    err_blocks.c err_mark.c err.c err_all.c
    err_all_legacy.c err_prn.c err_save.c
    )
target_sources(crypto PRIVATE ${SOURCE})
