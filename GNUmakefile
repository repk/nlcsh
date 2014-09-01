CONFDIR ?= ./Conf
CONF ?= default

include $(CONFDIR)/$(CONF).mk
include Files.mk

.SUFFIXES:
	MAKEFLAGS += --no-builtin-rules --no-builtin-variables \
		--warn-undefined-variables


CURFILE = $(dir $(lastword $(MAKEFILE_LIST)))
CURDIR = $(CURFILE:%/=%)

DEPSDIR = $(dir $(DEPS))
DEPSEXEC = $(notdir $(DEPS))

DST = $(patsubst %/.,%, $(DESTDIR)/$(CURDIR))
TARGET = $(patsubst ./%,%, $(CURDIR)/$(EXEC))

_SRC = $(SRC:.c=.o)
OBJ = $(patsubst %, $(DST)/%, $(_SRC:.S=.So))
DEPENDS = $(filter-out %.So, $(OBJ:%.o=%.d))


RULESMK_DEP = $(DEPSDIR:%=%Rules.mk)
BUILD_DEP = $(DEPS:%=build-%)
CLEAN_DEP = $(DEPS:%=clean-%)
MRPROPER_DEP = $(DEPS:%=mrproper-%)
DESTDIR_DEP = $(DEPS:%=destdir-%)

default: build-$(TARGET)

.PHONY: clean mrproper

clean: clean-$(TARGET)

mrproper: mrproper-$(TARGET)

include Rules.mk
