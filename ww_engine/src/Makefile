#Copyright (c) 2014 All rights reserved.

CC := $(CROSS)gcc

CFLAGS := -Wall -g
CFLAGS := -I$(STAGING_DIR)/usr/include
LDFLAGS := -L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/usr/lib/libnet-1.0.x/lib -lnids -lpcap -lnet

all: ww_engine

ww_engine: printall.o
	$(CC) $(LDFLAGS) printall.o -o ww_engine

ww_engine.o: printall.c
	$(CC) $(CFLAGS) -c printall.c

clean:
	rm -f *~ *.o

