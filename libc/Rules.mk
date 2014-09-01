LIBC_DIR := $(CURDIR)

include $(LIBC_DIR)/Files.mk

LIBC_LIB := $(LIB)
LIBC_SRC := $(SRC)
LIBC_INC := $(patsubst %, -I $(LIBC_DIR)/%, $(INC))
LIBC_OBJ := $(OBJ)
LIBC_DEP := $(DEPENDS)
LIBC_DESTDIR := $(DST)
LIBC_ABSDESTDIR := $(abspath $(LIBC_DESTDIR))

build-$(TARGET): destdir-$(TARGET) $(BUILD_DEP) $(LIBC_DESTDIR)/$(LIBC_LIB)

$(LIBC_DESTDIR)/$(LIBC_LIB): $(LIBC_OBJ)
	$(AR)  rcs $@ $^

$(LIBC_DESTDIR)/%.o: $(LIBC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) -MMD $(LIBC_INC)

$(LIBC_ABSDESTDIR):
	mkdir -p $(LIBC_ABSDESTDIR)

destdir-$(TARGET): $(LIBC_ABSDESTDIR)

.PHONY: clean-$(LIBC_LIB) mrproper-$(LIBC_LIB)

clean-$(TARGET):
	rm -f $(LIBC_ABSDESTDIR)/*.o
	rm -f $(LIBC_ABSDESTDIR)/*.d

mrproper-$(TARGET): clean
ifneq ($(LIBC_ABSDESTDIR),$(realpath $(LIBC_DIR)))
	rm -rf $(LIBC_ABSDESTDIR)
else
	rm -f $(LIBC_ABSDESTDIR)/$(LIBC_LIB)
endif

-include $(RULESMK_DEP)
-include $(LIBC_DEP)

