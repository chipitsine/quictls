set(SOURCE
    dsa_sign.c dsa_vrf.c dsa_lib.c dsa_ossl.c dsa_check.c dsa_key.c
    dsa_backend.c dsa_gen.c dsa_asn1.c dsa_err.c dsa_ameth.c dsa_pmeth.c
    dsa_prn.c dsa_meth.c
    )
target_sources(crypto PRIVATE ${SOURCE})
