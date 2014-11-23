##################
# add progs here #
##################

BIN_PROGS	= false mkdir mv pwd rm rmdir sync true hello iLoop shell init abtest
SBIN_PROGS	=

##########################
# includes/kernel header #
##########################

USER_INCLUDE_DIRS = \
	$(SOURCE_DIR)/common/include \
	$(SOURCE_DIR)/user/include

USER_KERNEL_HEADERS = \
	$(wildcard $(SOURCE_DIR)/user/syscalls/*.cpp) \
	$(wildcard $(SOURCE_DIR)/user/utils/*.cpp) \
	$(SOURCE_DIR)/kern/arch/x86/syscalls/syscaller.cpp

#################
# compile flags #
#################

USER_CXXFLAGS	 = $(CXXFLAGS)				# use generel flags
#USER_CXXFLAGS	+= -lstdc++					# use libstdc++
USER_CXXFLAGS	+= -nostdlib				# dont compile with standard library
USER_CXXFLAGS	+= -fno-builtin				# no support for TODO ???
USER_CXXFLAGS	+= -fno-rtti				# no support for runtime type information
USER_CXXFLAGS	+= -fno-exceptions			# no support for exceptions

USER_LDFLAGS	 = $(LDFLAGS)


##################################################
# do not edit unless you know what you are doing #
##################################################

# config

USER_SOURCE_DIR			= $(SOURCE_DIR)/user
USER_BUILD_DIR			= $(BUILD_DIR)/user

USER_BIN_SOURCE_DIR		= $(USER_SOURCE_DIR)/bin
USER_SBIN_SOURCE_DIR	= $(USER_SOURCE_DIR)/sbin

USER_BIN_INST_DIR		= $(FS_ROOT_DIR)/bin
USER_SBIN_INST_DIR		= $(FS_ROOT_DIR)/sbin

# generate lists

KERNEL_HEADER_OBJECTS= $(subst $(SOURCE_DIR),$(BUILD_DIR),$(USER_KERNEL_HEADERS:.cpp=.o))
USER_INCLUDES    	 = $(addsuffix /,$(addprefix -I,$(USER_INCLUDE_DIRS)))
USERLAND			 = $(addprefix $(USER_BIN_INST_DIR)/,$(BIN_PROGS))
USERLAND			+= $(addprefix $(USER_SBIN_INST_DIR)/,$(SBIN_PROGS))

# how to link the userland progs

$(FS_ROOT_DIR)/%: $(USER_BUILD_DIR)/%/main.o $(KERNEL_HEADER_OBJECTS)
	@echo "[LD] $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) -o $@ $^ $(addprefix -Xlinker ,$(USER_LDFLAGS))

# how to compile the userland progs

$(USER_BUILD_DIR)/%.o: $(USER_SOURCE_DIR)/%.cpp
	@echo "[GXX] $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(USER_CXXFLAGS) $(USER_INCLUDES) -c $< -o $@

# how to generate dependencies

$(USER_BUILD_DIR)/%.dep: $(USER_SOURCE_DIR)/%.cpp
	@echo "[DEP] $@"
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(USER_CXXFLAGS) $(USER_INCLUDES) -MM -MP -MT '$(@:.dep=.o) $@' -o $@ $<

% vim:ts=4:tw=0:ft=make:
