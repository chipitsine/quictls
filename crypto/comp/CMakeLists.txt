set(SOURCE
    comp_lib.c comp_err.c c_brotli.c c_zstd.c c_zlib.c
    )
target_sources(crypto PRIVATE ${SOURCE})
