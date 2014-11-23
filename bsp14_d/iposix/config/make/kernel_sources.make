###################################
# source files (add sources here) #
###################################

KERNEL_ARCH_x86_SOURCES = \
	$(KERNEL_BUILD_DIR)/arch/x86/multiboot/startup.O \
	$(KERNEL_BUILD_DIR)/arch/x86/core/cpu_core.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/gate_descriptor.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/global_descriptor_table.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/interrupt_descriptor_table.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/isr.O \
	$(KERNEL_BUILD_DIR)/arch/x86/core/isr_handler.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/privilege.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/task_state_segment.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/memory_descriptor.o \
	$(KERNEL_BUILD_DIR)/arch/x86/devices/memory.o \
	$(KERNEL_BUILD_DIR)/arch/x86/devices/video.o \
	$(KERNEL_BUILD_DIR)/arch/x86/devices/timer.o \
	$(KERNEL_BUILD_DIR)/arch/x86/devices/keyboard.o \
	$(KERNEL_BUILD_DIR)/arch/x86/devices/sound.o \
	$(KERNEL_BUILD_DIR)/arch/x86/devices/serial_port.o \
	$(KERNEL_BUILD_DIR)/arch/x86/elf/elf_loader.o \
	$(KERNEL_BUILD_DIR)/arch/x86/ata/ata.o \
	$(KERNEL_BUILD_DIR)/arch/x86/ata/ata_controller.o \
	$(KERNEL_BUILD_DIR)/arch/x86/ata/ata_drive.o \
	$(KERNEL_BUILD_DIR)/arch/x86/multiboot/multiboot.o \
	$(KERNEL_BUILD_DIR)/arch/x86/syscalls/syscaller.o \
	$(KERNEL_BUILD_DIR)/arch/x86/core/interrupts.o \
	$(KERNEL_BUILD_DIR)/arch/x86/vm/vmstructs.o \
	$(KERNEL_BUILD_DIR)/arch/x86/scheduler/process.o \
	$(KERNEL_BUILD_DIR)/arch/x86/scheduler/process_schedule_data.o \
	$(KERNEL_BUILD_DIR)/arch/x86/scheduler/scheduler.O \
	$(KERNEL_BUILD_DIR)/arch/x86/scheduler/scheduler.o


KERNEL_SHARED_SOURCES= \
	$(KERNEL_BUILD_DIR)/iposix/main.o \
	$(KERNEL_BUILD_DIR)/iposix/iposix_kernel.o \
	$(KERNEL_BUILD_DIR)/lib/cppruntime/cppruntime.o \
	$(KERNEL_BUILD_DIR)/lib/cppruntime/runtime_startup.o \
	$(KERNEL_BUILD_DIR)/lib/cppruntime/libcalls.o \
	$(KERNEL_BUILD_DIR)/devices/i_block_device.o \
	$(KERNEL_BUILD_DIR)/devices/i_char_device.o \
	$(KERNEL_BUILD_DIR)/devices/i_console.o \
	$(KERNEL_BUILD_DIR)/devices/i_core_holder.o \
	$(KERNEL_BUILD_DIR)/fs/partitiontable_crawler.o \
	$(KERNEL_BUILD_DIR)/elf/i_elf_loader.o \
	$(KERNEL_BUILD_DIR)/fs/filehandle.o \
	$(KERNEL_BUILD_DIR)/fs/i_fs.o \
	$(KERNEL_BUILD_DIR)/fs/i_fs_mounting.o \
	$(KERNEL_BUILD_DIR)/fs/i_fs_node_operations.o \
	$(KERNEL_BUILD_DIR)/fs/i_fs_dummy_node.o \
	$(KERNEL_BUILD_DIR)/fs/i_fs_node.o \
	$(KERNEL_BUILD_DIR)/fs/fat_fs.o \
	$(KERNEL_BUILD_DIR)/fs/fat_fs_node.o \
	$(KERNEL_BUILD_DIR)/fs/dev_fs.o \
	$(KERNEL_BUILD_DIR)/fs/dev_fs_node.o \
	$(KERNEL_BUILD_DIR)/lib/logger/logger.o \
	$(KERNEL_BUILD_DIR)/lib/logger/message.o \
	$(KERNEL_BUILD_DIR)/mm/memory_area.o \
	$(KERNEL_BUILD_DIR)/scheduler/i_process.o \
	$(KERNEL_BUILD_DIR)/syscalls/syscall_dispatcher.o \
	$(KERNEL_BUILD_DIR)/scheduler/i_scheduler.o

KERNEL_TEST_SOURCES= \
	$(KERNEL_BUILD_DIR)/tests/all_tests.o \
	$(KERNEL_BUILD_DIR)/tests/elf_loader_test.o \
	$(KERNEL_BUILD_DIR)/tests/process_test.o \
	$(KERNEL_BUILD_DIR)/tests/memory_test.o \
	$(KERNEL_BUILD_DIR)/tests/i_fs_test.o


#################
# include files #
#################

KERNEL_INCLUDE_DIRS = \
	$(SOURCE_DIR)/common/include \
	$(SOURCE_DIR)/kern/include \
	$(SOURCE_DIR)/kern/arch \
	$(SOURCE_DIR)/kern/lib

#################
# compile flags #
#################

KERNEL_CXXFLAGS  = $(CXXFLAGS)						# use generel flags
KERNEL_CXXFLAGS	+= -lstdc++							# use libstdc++

KERNEL_LDFLAGS	 = $(LDFLAGS)
KERNEL_LDFLAGS	+= -T$(LINKER_SCRIPT_ELF)

############################
# change architekture here #
############################

KERNEL_ARCH = x86
KERNEL_ARCH_SOURCES = $(KERNEL_ARCH_x86_SOURCES)

##################################################
# do not edit unless you know what you are doing #
##################################################

# config
KERNEL_SOURCE_DIR		= $(SOURCE_DIR)/kern
KERNEL_BUILD_DIR		= $(BUILD_DIR)/kern

KERNEL = $(BOOT_DIR)/kernel.elf

# generate lists
KERNEL_INCLUDES		= $(addsuffix /,$(addprefix -I,$(KERNEL_INCLUDE_DIRS)))
KERNEL_SOURCES		= $(KERNEL_ARCH_SOURCES) $(KERNEL_SHARED_SOURCES) $(KERNEL_TEST_SOURCES)

# how to link the kernel

$(KERNEL): $(KERNEL_SOURCES)
	@echo "[LD] $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) -nostartfiles -o $@ $^ $(addprefix -Xlinker ,$(KERNEL_LDFLAGS))

# how to compile kernel sources

$(KERNEL_BUILD_DIR)/%.o: $(KERNEL_SOURCE_DIR)/%.c
	@echo "[GCC] $@"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(KERNEL_INCLUDES) -c $< -o $@

$(KERNEL_BUILD_DIR)/%.o: $(KERNEL_SOURCE_DIR)/%.cpp
	@echo "[GXX] $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(KERNEL_CXXFLAGS) $(KERNEL_INCLUDES) -c $< -o $@

$(KERNEL_BUILD_DIR)/%.O: $(KERNEL_SOURCE_DIR)/%.asm
	@echo "[ASM] $@"
	@mkdir -p $(shell dirname $@)
	@$(NASM) -g -f elf -I./$(shell dirname $@)/ $< -o $@

# how to generate an assembler include from a c/c++ header file

$(KERNEL_BUILD_DIR)/%.inc: $(KERNEL_SOURCE_DIR)/%.h
	@echo "[CONV] $< -> $@"
	@#find the magic line "#ifndef ASM" and delete every thing from this line 'til the end
	@cat $< | $(SED) '$(shell $(GREP) -n "^#ifndef ASM" $< | $(SED) "s/:.*//g"),$$d' > $@
	@$(SED) --in-place "s/^#pragma.*//g" $@
	@$(SED) --in-place "s/#/%/g" $@
	@$(SED) --in-place "s%/%;%g" $@

# how to generate dependencies

$(KERNEL_BUILD_DIR)/%.dep: $(KERNEL_SOURCE_DIR)/%.cpp
	@echo "[DEP] $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(KERNEL_CXXFLAGS) $(KERNEL_INCLUDES) -MM -MP -MT '$(@:.dep=.o) $@' -o $@ $<

$(KERNEL_BUILD_DIR)/%.dep: $(KERNEL_SOURCE_DIR)/%.asm
	@echo "[DEP] $@"
	@mkdir -p $(shell dirname $@)
	@$(NASM) -g -f elf -M -MG -MT '$(@:.dep=.O) $@' -MD $@ $<
	@$(SED) --in-place "s%[^ ]*\.inc%$(shell dirname $@)/&%g" $@
	@$(SED) --in-place "s%[^ ]*\.h%%g" $@

% vim:ts=4:tw=0:ft=make:
