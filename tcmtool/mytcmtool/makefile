# makefile for tcm tool  on Linux

TCM_LIBS= 
SYSTEM_LIBS= -lpthread -lm -lc -lcurses -lnsl -lrt 


CC=g++
cc=gcc

CHEAD= -I./ 
BIN_DIR=bin
DIR_OBJS = obj
DEF =  -DLINUX
COMPILE_PARAM= -mt
CFLAGS= -g 
LIB =  -Wl,-soname,$(PROGRAM),--no-undefined

 
DIR_SOURCE=./
SOURCE=\
	test.c 

OBJS_SOURCE=\
	test.o

OBJS=\
	$(OBJS_SOURCE)

all::bin/test
${OBJS_SOURCE}:%.o:./%.c
	$(CC) $(CFLAGS) $(CHEAD) -o $@ -c $<
	
bin/test : $(OBJS)
	$(CC) $(LIB) -o test $(OBJS) $(SYSTEM_LIBS) $(TCM_LIBS)

clean:
	rm -f $(OBJS)
	rm -f test 
