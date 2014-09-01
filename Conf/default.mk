CC := gcc
LD := gcc
MAKE := make
AR := ar

DESTDIR := ./build

CFLAGS := -W -Wall -m32 -Os
LDFLAGS := -m32 -L$(DESTDIR)/libc -lc

