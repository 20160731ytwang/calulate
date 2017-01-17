noinst_LTLIBRARIES = libtcmalg.la

libtcmalg_la_SOURCES = bn_add.c bn_asm.c bn_ctx.c bn_div.c bnEx.c bn_exp.c bn_gcd.c \
		       bn_kron.c bn_lib.c bn_mod.c bn_mont.c bn_mul.c bn_print.c bn_recp.c \
		       bn_shift.c bn_sqr.c bn_sqrt.c bn_word.c calculate_hash.c debug.c \
		       ec_decrypt.c ec_encrypt.c ec_genkey.c ec_init.c ec_operations.c ec_signature.c ec_verify.c \
		       mem.c mem_clr.c sch.c seal.c sms4.c \
		       tcm_bn.c tcm_ecc.c tcm_ecc_exchange.c tcm_hash.c tcm_rand.c tcm_sms4.c
		    
noinst_HEADERS = tcmalg.h

libtcmalg_la_CFLAGS = -I ./openssl
