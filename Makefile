#
# %Id: Makefile,v 1.1 
#
#
#
CC = gcc

CCFLAGS = 
LIBS = 
LDFLAGS = -lm 

%: %.c
	$(CC) $(CCFLAGS) -c -o $*.o $*.c
	$(CC) $*.o $(LIBS) $(LDFLAGS) -o $*
