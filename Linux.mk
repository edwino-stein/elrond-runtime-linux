# General Linux build settings
BUILD_DIR_ROOT = build
OBJ_EXTENSION = .o
RT_EXTENSION =
DEPENDENCE_EXTENSION = .d

# Compiler and linker settings
COMPILER = g++
COMPILER_COMMON_OPTIONS = -std=c++11 -Wall -MMD -c
LINKER_COMMON_OPTIONS = -std=c++11 -Wall -rdynamic
LIB_BUILDER = ar rvs

# Params lists
INCLUDES = $(INCLUDE_DIR_ROOT)
MACROS = GENERIC_STD_PLATFORM LINUX_PLATFORM
STATIC_LIBRARIES =
DYNAMIC_LIBRARIES = dl pthread m
RAW_DLIBRARIES =

# Elrond Common library setup
COMMON_DIR = elrond
COMMON_NAME_LIB = lib-elrond-common
COMMON_LIB = build/$(COMMON_NAME_LIB).a

# if has a dev common version in the project, use it
ifeq ($(shell test -L $(COMMON_DIR)-dev -o -d $(COMMON_DIR)-dev; echo $$?), 0)
    COMMON_DIR := $(COMMON_DIR)-dev
endif
INCLUDES += $(COMMON_DIR)/include

#Define build dir as target for all src files
OBJS_FILES = $(subst $(SRC_DIR_ROOT)/,$(BUILD_DIR_ROOT)/,$(SRC_FILES))

# Set NOPIC objects
OBJS := $(addsuffix $(OBJ_EXTENSION), $(OBJS_FILES))
DEPS = $(OBJS:$(OBJ_EXTENSION)=$(DEPENDENCE_EXTENSION))

# Add includes and macros to compiler options
COMPILER_COMMON_OPTIONS += $(addprefix -I, $(INCLUDES))
COMPILER_COMMON_OPTIONS += $(addprefix -D, $(MACROS))

# Set Dynamic library to linkage
RAW_DLIBRARIES += $(addprefix -l, $(DYNAMIC_LIBRARIES))

# ************************** BUILD RULES **************************

.PHONY: $(PROJECT_NAME)$(RT_EXTENSION) clean-build clean-build-all clean-common run

# Runtime object builder
$(PROJECT_NAME)$(RT_EXTENSION): $(BUILD_DIR_ROOT)/$(PROJECT_NAME)$(RT_EXTENSION)
$(BUILD_DIR_ROOT)/$(PROJECT_NAME)$(RT_EXTENSION): $(BUILD_DIR_ROOT)/$(PROJECT_NAME).a $(COMMON_DIR)/$(COMMON_LIB)
	@mkdir -p $(@D)
	$(COMPILER) $(LINKER_COMMON_OPTIONS) $^ $(STATIC_LIBRARIES) -o $@ $(RAW_DLIBRARIES)

$(BUILD_DIR_ROOT)/$(PROJECT_NAME).a: $(OBJS)
	@mkdir -p $(@D)
	@$(LIB_BUILDER) $@ $?

# Objects builder
$(BUILD_DIR_ROOT)/%$(OBJ_EXTENSION): $(SRC_DIR_ROOT)/%
	@mkdir -p $(@D)
	$(COMPILER) $(COMPILER_COMMON_OPTIONS) $< -o $@

# Elrond common library builder
$(COMMON_DIR)/$(COMMON_LIB):
	cd $(COMMON_DIR) && $(MAKE) $(notdir $@)

# Cleaning rules
clean-common:
	cd $(COMMON_DIR) && $(MAKE) clean

clean-build:
	rm -rf $(BUILD_DIR_ROOT)

clean-build-all: clean-build clean-common

# Include all .d files
-include $(DEPS)
