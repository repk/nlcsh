CC := gcc
LD := gcc
MAKE := make
AR := ar

DESTDIR := ./build

CFLAGS := -W -Wall -m32 -O0 -g
LDFLAGS := -m32 -L$(DESTDIR)/libc -lc
