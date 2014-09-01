SHELL_DIR := $(CURDIR)

include $(SHELL_DIR)/Files.mk

SHELL_EXEC := $(EXEC)
SHELL_SRC := $(SRC)
SHELL_INC := $(patsubst %, -I $(SHELL_DIR)/%, $(INC))
SHELL_OBJ := $(OBJ)
SHELL_DEP := $(DEPENDS)
SHELL_DESTDIR := $(DST)
SHELL_ABSDESTDIR := $(abspath $(SHELL_DESTDIR))

build-$(TARGET): destdir-$(TARGET) $(BUILD_DEP) $(SHELL_DESTDIR)/$(SHELL_EXEC)

$(SHELL_DESTDIR)/$(SHELL_EXEC): $(SHELL_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) -nostdlib

$(SHELL_DESTDIR)/%.So: $(SHELL_DIR)/%.S
	$(CC) -o $@ -c $< $(CFLAGS)

$(SHELL_DESTDIR)/%.o: $(SHELL_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) -MMD $(SHELL_INC)

$(SHELL_ABSDESTDIR):
	mkdir -p $(SHELL_ABSDESTDIR)

destdir-$(TARGET): $(SHELL_ABSDESTDIR)

.PHONY: clean-$(SHELL_EXEC) mrproper-$(SHELL_EXEC)

clean-$(TARGET):
	rm -f $(SHELL_ABSDESTDIR)/*.o
	rm -f $(SHELL_ABSDESTDIR)/*.d

mrproper-$(TARGET): clean
ifneq ($(SHELL_ABSDESTDIR),$(realpath $(SHELL_DIR)))
	rm -rf $(SHELL_ABSDESTDIR)
else
	rm -f $(SHELL_ABSDESTDIR)/$(SHELL_EXEC)
endif

-include $(RULESMK_DEP)
-include $(SHELL_DEP)

