set(SOURCE
    pem_sign.c pem_info.c pem_lib.c pem_all.c pem_err.c pem_x509.c
    pem_xaux.c pem_oth.c pem_pk8.c pem_pkey.c pvkfmt.c
    )
target_sources(crypto PRIVATE ${SOURCE})
