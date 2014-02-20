#Copyright (c) 2014 All rights reserved.

GCC := $(CROSS)gcc
CXX := $(CROSS)g++

CFLAGS := -Wall -g  -I$(STAGING_DIR)/usr/include
LDFLAGS := -L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/usr/lib/libnet-1.0.x/lib -lnids -lpcap -lnet

all: ww_engine

ww_engine: main.o check.o
	$(CXX) $(LDFLAGS) main.o check.o -o ww_engine

main.o: main.c
	$(GCC) $(CFLAGS) -c main.c

check.o: check.cpp
	$(CXX) $(CFLAGS) -c check.cpp

clean:
	rm -f *~ *.o

