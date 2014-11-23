# directorys
BUILD_DIR  = ./build
IMAGE_DIR  = $(BUILD_DIR)/img
FS_ROOT_DIR= $(BUILD_DIR)/fs_root
BOOT_DIR   = $(BUILD_DIR)/boot
SOURCE_DIR = ./src
BIN_DIR    = ./bin
TOOLS_DIR  = ./tools/install
CONFIG_DIR = ./config
DOC_DIR    = ./doc
LIB_DIR    = ./lib

# files
BOCHS_CONFIG           = $(CONFIG_DIR)/bochs/bochsrc
BOCHS_VB_CONFIG        = $(CONFIG_DIR)/bochs/verbose_bochsrc
GDB_CONFIG             = $(CONFIG_DIR)/gdb/gdb.cmd
LINKER_SCRIPT_BIN      = $(CONFIG_DIR)/linker/link_bin.ld
LINKER_SCRIPT_ELF      = $(CONFIG_DIR)/linker/link_elf.ld
DOXYGEN_CONFIG         = $(CONFIG_DIR)/doxygen/doxygen.config
DOXYGEN_MINIMAL_CONFIG = $(CONFIG_DIR)/doxygen/doxygen_minimal.config
LOG_FILE               = $(BUILD_DIR)/qemu.log
CTAGS_FILE	       = ./tags

#main target
.PHONY: main
main: build

#sourcen der configurationen
-include $(CONFIG_DIR)/make/kernel_sources.make
-include $(CONFIG_DIR)/make/user_sources.make
-include $(CONFIG_DIR)/make/general.make
-include $(CONFIG_DIR)/make/image.make

###############
# build targets
###############

# the targets which should be called from outside

.PHONY: all
all: build doc docmin toolchain

.PHONY: build
build: image ctags

.PHONY: run
run: run_image

.PHONY: debug
debug: run_image_debug

.PHONY: run_image
run_image: image
	@echo "[SIM] $(KERNEL)"
	@$(QEMU) $(QEMU_OPTIONS) -hda $(IMAGE_FILE)

.PHONY: run_image_bochs
run_image_bochs: image
	@echo "[SIM] $(KERNEL)"
	@$(BOCHS) -f $(BOCHS_CONFIG)

.PHONY: run_image_bochs_verbose
run_image_bochs_verbose: image
	@echo "[SIM] $(KERNEL)"
	@$(BOCHS) -f $(BOCHS_VB_CONFIG)

.PHONY: run_image_debug
run_image_debug: image
	@echo "[SIM] $(KERNEL)"
	@$(QEMU) $(QEMU_OPTIONS) -hda $(IMAGE_FILE) -s -S &
	@sleep 1
	@$(GDB) -s $(KERNEL) -x $(GDB_CONFIG)

.PHONY: run_image_debug_insight
run_image_debug_insight: image
	@echo "[SIM] $(KERNEL)"
	@$(QEMU) $(QEMU_OPTIONS) -hda $(IMAGE_FILE) -s -S &
	@sleep 1
	@insight -se $(KERNEL) -x $(GDB_CONFIG)

.PHONY: run_image_log
run_image_log: image
	@echo "[SIM] $(KERNEL) with Log in $(LOG_FILE)"
	@$(QEMU) $(QEMU_OPTIONS) -hda $(IMAGE_FILE) -d $(QEMU_LOG_ITEMS)

.PHONY: clean
clean:
	@echo -n "[CLN] "
	@echo -n "objects, "
	@cd $(BUILD_DIR) && $(FIND) . -name *.o -exec $(RM) {} \;
	@cd $(BUILD_DIR) && $(FIND) . -name *.O -exec $(RM) {} \;
	@echo -n "generated includes, "
	@cd $(BUILD_DIR) && $(FIND) . -name *.inc -exec $(RM) {} \;
	@echo -n "dependency files, "
	@cd $(BUILD_DIR) && $(FIND) . -name *.dep -exec $(RM) {} \;
	@echo -n "images, "
	@cd $(BUILD_DIR) && $(FIND) . -name *.img -exec $(RM) {} \;
	@echo -n "kernels, "
	@cd $(BUILD_DIR) && $(FIND) . -name *.bin -exec $(RM) {} \;
	@cd $(BUILD_DIR) && $(FIND) . -name *.elf -exec $(RM) {} \;
	@echo -n "ctags file, "
	@$(RM) -f $(CTAGS_FILE)
	@echo "documentation "
	@rm -rf $(DOC_DIR)/doxygen
	@rm -rf $(DOC_DIR)/doxygen_minimal

.PHONY: LoC
LoC:
	@$(FIND) $(SOURCE_DIR) -type f | xargs wc | grep -v $(SOURCE_DIR)

.PHONY: rm_endspace
rm_endspace:
	@echo "[FIND] removing spaces and tabs at end of line"
	@$(FIND) $(SOURCE_DIR) -type f -exec $(SED) --in-place "s/[ \t]\+$$//g" {} \;

.PHONY: todo
todo:
	@echo "[GRP] searching for todos"
	@$(GREP) -R -e "todo\|TODO" $(SOURCE_DIR)/*

.PHONY: help
help: 
	@echo "targets: usage make <target>"
	@echo "all						-	generates everything"
	@echo "clean                    -	the common clean target; it cleans up all stuff"
	@echo "ctags                    -	erstellt eine ctags datei"
	@echo "debug                    -	see run_image_debug"
	@echo "doc                      -	generates documantation"
	@echo "image                    -	creates a disk image with the kernel copied on"
	@echo "kernel                   -	builds the kernel"
	@echo "userland                 -	generates the userland programs"
	@echo "run                      -	runs the kernel after building"
	@echo "run_image                -	runs the kernel after building with a disk image"
	@echo "run_image_debug          -	runs the kernel after building with a disk imagein debug mode"
	@echo "run_image_debug_insight  -	runs the kernel after building with a disk imagein debug mode (with insight)"
	@echo "run_image_log            -	runs the kernel after building with a disk imagein log mode"
	@echo "run_image_bochs          -	runs the kernel after building with bochs"
	@echo "LoC                      -	prints Lines of Code"
	@echo "rm_endspace              -	remove trailing spaces in the source code"

###############
# file target #
###############

# what files are we able to generate

image: $(IMAGE_FILE)
.PHONY: image

kernel: $(KERNEL)
.PHONY: kernel

mini_kernel: $(BOOT_DIR)/$(GRUB_MINI_KERNEL)
.PHONY: mini_kernel

ctags: $(CTAGS_FILE)
.PHONY: ctags

doc: doxygen
.PHONY: doc

docmin: doxygen_minimal
.PHONY: docmin

userland: $(USERLAND)
.PHONY:userland

.PHONY: build_toolchain
build_toolchain:
	@echo "Generating toolchain"
	@cd tools;./toolchain.sh

################
# ctags target #
################

# how to generate a ctags file

$(CTAGS_FILE): $(shell find $(SOURCE_DIR) -name "*.cpp" -or -name "*.h" -or -name "*.hpp")
	@echo "[CTG] $@"
	@-ctags -h .h.hpp -f $@ $^

################
# doxygen gen  #
################

doxygen: $(DOC_DIR)/doxygen/html/index.html
.PHONY: doxygen

$(DOC_DIR)/doxygen/html/index.html: $(SRC_FILES) $(DOXYGEN_CONFIG)
	@mkdir -p $(DOC_DIR)/doxygen
	@echo "[DOXYGEN] Creating Documentation"
	@$(DOXYGEN) $(DOXYGEN_CONFIG)

doxygen_minimal: $(DOC_DIR)/doxygen_minimal/html/index.html
.PHONY: doxygen_minimal

$(DOC_DIR)/doxygen_minimal/html/index.html: $(SRC_FILES) $(DOXYGEN_MINIMAL_CONFIG)
	@mkdir -p $(DOC_DIR)/doxygen_minimal
	@echo "[DOXYGEN] Creating minimal Documentation"
	@$(DOXYGEN) $(DOXYGEN_MINIMAL_CONFIG)

#############################
# includes for dependencies #
#############################

SRC_FILES=$(shell $(FIND) $(SOURCE_DIR) -name "*.c" -or -name "*.cpp" -or -name "*.asm" | $(SED) s%$(SOURCE_DIR)%$(BUILD_DIR)%)

DEP_FILES=$(subst .c,.dep,$(subst .asm,.dep,$(subst .cpp,.dep,$(SRC_FILES))))


.PHONY: test
test:
	@echo -n "DEP_FILES = "
	@echo $(DEP_FILES)
	@echo -n "KERNEL = "
	@echo $(KERNEL)
	@echo -n "KERNEL_SOURCES = "
	@echo $(KERNEL_SOURCES)
	@echo -n "USERLAND = "
	@echo $(USERLAND)
	@echo -n "KERNEL_HEADERS = "
	@echo $(KERNEL_HEADER_OBJECTS)

-include $(DEP_FILES)

% vim:ts=4:tw=0:
