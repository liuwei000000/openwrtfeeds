#Copyright (c) 2014 All rights reserved.

CC := $(CROSS)gcc

CFLAGS := -Wall -g
CFLAGS := -I$(STAGING_DIR)/usr/include
LDFLAGS := -L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/usr/lib/libnet-1.0.x/lib -lnids -lpcap -lnet

all: ww_engine

ww_engine: main.o
	$(CC) $(LDFLAGS) main.o -o ww_engine

ww_engine.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *~ *.o

