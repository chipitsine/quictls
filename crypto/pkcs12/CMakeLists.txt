set(SOURCE
    p12_add.c p12_asn.c p12_attr.c p12_crpt.c p12_crt.c p12_decr.c
    p12_init.c p12_key.c p12_kiss.c p12_mutl.c p12_sbag.c p12_utl.c
    p12_npas.c pk12err.c p12_p8d.c p12_p8e.c
    )
target_sources(crypto PRIVATE ${SOURCE})
