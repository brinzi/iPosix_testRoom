#config

IMAGE_FILE		= $(IMAGE_DIR)/boot_hd.img

SEC_PER_TRACK	= "63"
CYLINDER		= "20"
HEADS			= "16"

# do not edit
GRUB_CONFIG_DIR	= $(CONFIG_DIR)/grub
CONTENT_DIR		= $(FS_ROOT_DIR)
GRUB_LIB_DIR	= $(TOOLS_DIR)/lib/grub

GRUB_CONFIG		= $(CONFIG_DIR)/grub/menu.lst
GRUB_CMD		= $(TOOLS_DIR)/sbin/grub
MTOOLS_CMD		= $(TOOLS_DIR)/bin/mtools
DD				= $(shell which dd)

SECTORS			= $(shell expr $(SEC_PER_TRACK) \* $(shell expr $(CYLINDER) \* $(HEADS)))

GZ_KERNEL		= $(KERNEL).gz
RAW_IMAGE_FILE	= $(IMAGE_DIR)/raw_image.img
BOOTABLE_IMAGE_FILE=$(IMAGE_DIR)/bootable_image.img
ROOT_IMAGE_FILE	= $(IMAGE_DIR)/root_image.img

#find all files which should be containts in the image
ifeq ($(shell [ -d "$(CONTENT_DIR)" ] && echo -n "true"),true)
CONTENT			= $(shell find $(CONTENT_DIR))
else
CONTENT			=
endif

#targets

$(GZ_KERNEL): $(KERNEL)
	@echo "[GZIP] Gzipping kernel"
	@gzip --stdout $^ > $@

$(RAW_IMAGE_FILE):
	@echo "[DD] Creating raw image"
	@mkdir -p $(shell dirname $@)
	@$(DD) if=/dev/zero of="$@.tmp" bs=512 count=$(SECTORS) >/dev/null 2>/dev/null
	@echo "[MTOOLS] Creating partitiontable"
	@#saving old mtoolsconfig
	@([ -f ~/.mtoolsrc ] && mv ~/.mtoolsrc ~/.mtoolsrc.bak) || true
	@echo "drive c: file=\"$@.tmp\" partition=1" > ~/.mtoolsrc
	@$(MTOOLS_CMD) -c mpartition -I -s $(SEC_PER_TRACK) -t $(CYLINDER) -h $(HEADS) c: >/dev/null 2>/dev/null
	@echo "[MTOOLS] Creating partition"
	@$(MTOOLS_CMD) -c mpartition -c -s $(SEC_PER_TRACK) -t $(CYLINDER) -h $(HEADS) c: >/dev/null 2>/dev/null
	@echo "[MTOOLS] Formating partition"
	@$(MTOOLS_CMD) -c mformat c:
	@#restoring mtools config
	@([ -f ~/.mtoolsrc.bak ] && mv ~/.mtoolsrc.bak ~/.mtoolsrc) || true
	@mv "$@.tmp" "$@"

$(BOOTABLE_IMAGE_FILE): $(RAW_IMAGE_FILE)
	@echo "[GRUB] Make image bootable"
	@cp "$<" "$@.tmp"
	@#saving old mtoolsconfig
	@([ -f ~/.mtoolsrc ] && mv ~/.mtoolsrc ~/.mtoolsrc.bak) || true
	@echo "drive c: file=\"$@.tmp\" partition=1" > ~/.mtoolsrc
	@$(MTOOLS_CMD) -c mmd "c:/boot"
	@$(MTOOLS_CMD) -c mmd "c:/boot/grub"
	@$(MTOOLS_CMD) -c mcopy -s $(GRUB_LIB_DIR)/*/* "c:/boot/grub/"
	@echo "device (hd0) $@.tmp" > $(BUILD_DIR)/grub_install_script.tmp
	@echo "root (hd0,0)" >> $(BUILD_DIR)/grub_install_script.tmp
	@echo "setup (hd0)" >> $(BUILD_DIR)/grub_install_script.tmp
	@$(GRUB_CMD) --device-map=/dev/null < $(BUILD_DIR)/grub_install_script.tmp >/dev/null 2>/dev/null
	@#restoring mtools config
	@([ -f ~/.mtoolsrc.bak ] && mv ~/.mtoolsrc.bak ~/.mtoolsrc) || true
	@mv "$@.tmp" "$@"

$(ROOT_IMAGE_FILE): $(BOOTABLE_IMAGE_FILE) $(GRUB_CONFIG) $(USERLAND)
	@echo "[MTOOLS] Copying files to image"
	@cp "$<" "$@.tmp"
	@#saving old mtoolsconfig
	@([ -f ~/.mtoolsrc ] && mv ~/.mtoolsrc ~/.mtoolsrc.bak) || true
	@echo "drive c: file=\"$@.tmp\" partition=1" > ~/.mtoolsrc
	@$(MTOOLS_CMD) -c mcopy -s $(GRUB_CONFIG) "c:/boot/grub/"
	@([ ! -z "$(CONTENT_DIR)" ] && $(MTOOLS_CMD) -c mcopy -s $(CONTENT_DIR)/* c: ) || true
	@#restoring mtools config
	@([ -f ~/.mtoolsrc.bak ] && mv ~/.mtoolsrc.bak ~/.mtoolsrc) || true
	@mv "$@.tmp" "$@"

$(IMAGE_FILE): $(ROOT_IMAGE_FILE) $(GZ_KERNEL)
	@echo "[MTOOLS] Copying Kernel"
	@cp "$<" "$@.tmp"
	@#saving old mtoolsconfig
	@([ -f ~/.mtoolsrc ] && mv ~/.mtoolsrc ~/.mtoolsrc.bak) || true
	@echo "drive c: file=\"$@.tmp\" partition=1" > ~/.mtoolsrc
	@$(MTOOLS_CMD) -c mcopy -s $(GZ_KERNEL) "c:/boot/"
	@#restoring mtools config
	@([ -f ~/.mtoolsrc.bak ] && mv ~/.mtoolsrc.bak ~/.mtoolsrc) || true
	@mv "$@.tmp" "$@"

.PHONY: show_image
show_image:
	@#saving old mtoolsconfig
	@([ -f ~/.mtoolsrc ] && mv ~/.mtoolsrc ~/.mtoolsrc.bak) || true
	@echo "drive c: file=\"$(IMAGE_FILE)\" partition=1" > ~/.mtoolsrc
	@$(MTOOLS_CMD) -c mdir -s c:
	@#restoring mtools config
	@([ -f ~/.mtoolsrc.bak ] && mv ~/.mtoolsrc.bak ~/.mtoolsrc) || true

% vim:ts=4:tw=0:ft=make:
