# makefile for tcm tool  on Linux

TCM_LIBS= -L./ -ltddl -ltcs
SYSTEM_LIBS= -lpthread -lm -lc -lcurses -lnsl -lrt 


CC=g++
cc=gcc

CHEAD= -I./ -I../alg -I../alg/openssl
BIN_DIR=bin
DIR_OBJS = obj
DEF =  -DLINUX
COMPILE_PARAM= -mt
CFLAGS=  
LIB =  -Wl,-soname,$(PROGRAM),--no-undefined

 
DIR_SOURCE_LTcmTool=./
SOURCE_LTcmTool=\
	bsg.cpp \
	dev.cpp \
	itcm.cpp \
	LTcmTool.cpp \
	TCMTOOLDlg.cpp \
	TString.cpp
	
OBJS_SOURCE_LTcmTool=\
	bsg.o \
	dev.o \
	itcm.o \
	LTcmTool.o \
	TCMTOOLDlg.o \
	TString.o


DIR_SOURCE_alg=./alg
SOURCE_alg=\
	mem_clr.c \
	mem.c \
	bnEx.c \
	bn_add.c \
	bn_asm.c \
	bn_ctx.c \
	bn_div.c \
	bn_exp.c \
	bn_gcd.c \
	bn_kron.c \
	bn_lib.c \
	bn_mod.c \
	bn_mont.c \
	bn_mul.c \
	bn_print.c \
	bn_recp.c \
	bn_shift.c \
	bn_sqr.c \
	bn_sqrt.c \
	bn_word.c \
	calculate_hash.c \
	debug.c \
	ec_decrypt.c \
	ec_encrypt.c \
	ec_genkey.c \
	ec_init.c \
	ec_operations.c \
	ec_signature.c \
	ec_verify.c \
	sch.c \
	seal.c \
	SMS4.c \
	tcm_bn.c \
	tcm_ecc.c \
	tcm_ecc_exchange.c \
	tcm_hash.c \
	tcm_rand.c \
	tcm_sms4.c


OBJS_SOURCE_alg=\
	mem_clr.o \
	mem.o \
	bnEx.o \
	bn_add.o \
	bn_asm.o \
	bn_ctx.o \
	bn_div.o \
	bn_exp.o \
	bn_gcd.o \
	bn_kron.o \
	bn_lib.o \
	bn_mod.o \
	bn_mont.o \
	bn_mul.o \
	bn_print.o \
	bn_recp.o \
	bn_shift.o \
	bn_sqr.o \
	bn_sqrt.o \
	bn_word.o \
	calculate_hash.o \
	debug.o \
	ec_decrypt.o \
	ec_encrypt.o \
	ec_genkey.o \
	ec_init.o \
	ec_operations.o \
	ec_signature.o \
	ec_verify.o \
	sch.o \
	seal.o \
	SMS4.o \
	tcm_bn.o \
	tcm_ecc.o \
	tcm_ecc_exchange.o \
	tcm_hash.o \
	tcm_rand.o \
	tcm_sms4.o

OBJS=\
	$(OBJS_SOURCE_LTcmTool) \
	$(OBJS_SOURCE_alg)

all::bin/tcmtool
${OBJS_SOURCE_LTcmTool}:%.o:./%.cpp
	$(CC) $(CFLAGS) $(CHEAD) -o $@ -c $<
	
${OBJS_SOURCE_alg}:%.o:../alg/%.c
	$(CC) $(CFLAGS) $(CHEAD) -o $@ -c $<

bin/tcmtool : $(OBJS)
	$(CC) $(LIB) -o tcmtool $(OBJS) $(SYSTEM_LIBS) $(TCM_LIBS)

clean:
	rm -f $(OBJS)
	rm -f tcmtool core
