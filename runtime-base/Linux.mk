# General Linux build settings
BUILD_DIR = ../build/runtime-base
OBJ_EXTENSION = .o
DEPENDENCE_EXTENSION = .d

# Compiler and linker settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -MMD -c
LDFLAGS = -std=c++11 -Wall
LDLIBS =
AR = ar
ARFLAGS = rs

# Params lists
INCLUDES = $(INCLUDE_DIR)
MACROS = GENERIC_STD_PLATFORM LINUX_PLATFORM
STATIC_LIBRARIES =
DYNAMIC_LIBRARIES =

# Elrond COMMON library setup
COMMON_DIR = ../elrond
COMMON_NAME_LIB = libelrond
COMMON_NONPIC_LIB = build/$(COMMON_NAME_LIB).a

# if has a dev common version in the project, use it
ifeq ($(shell test -L $(COMMON_DIR)-dev -o -d $(COMMON_DIR)-dev; echo $$?), 0)
    COMMON_DIR := $(COMMON_DIR)-dev
endif
INCLUDES += $(COMMON_DIR)/include

# Define object files
OBJS_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_FILES))
OBJS := $(addsuffix $(OBJ_EXTENSION), $(OBJS_FILES))

# Define dependencies files
DEPS = $(OBJS:$(OBJ_EXTENSION)=$(DEPENDENCE_EXTENSION))

# Add includes and macros to compiler options
CXXFLAGS += $(addprefix -I, $(INCLUDES))
CXXFLAGS += $(addprefix -D, $(MACROS))

# Set Dynamic library to linkage
LDLIBS += $(addprefix -l, $(DYNAMIC_LIBRARIES))

.PHONY: lib$(PROJECT_NAME).a clean-build

# ************************** BUILD RULES **************************

# Static library builder
lib$(PROJECT_NAME).a: $(BUILD_DIR)/lib$(PROJECT_NAME).a
$(BUILD_DIR)/lib$(PROJECT_NAME).a: $(OBJS) $(COMMON_DIR)/$(COMMON_NONPIC_LIB)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $(BUILD_DIR)/libelrond-runtime-base.a $(OBJS)
	$(AR) -crT $@ $(BUILD_DIR)/libelrond-runtime-base.a $(COMMON_DIR)/$(COMMON_NONPIC_LIB)

# Objects builder
$(BUILD_DIR)/%.cpp$(OBJ_EXTENSION): $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

$(COMMON_DIR)/$(COMMON_NONPIC_LIB):
	@cd $(COMMON_DIR) && $(MAKE) $(notdir $@)

# Cleaning rules
clean-build:
	rm -rf $(BUILD_DIR)
	@cd $(COMMON_DIR) && $(MAKE) clean

# Include all .d files
-include $(DEPS)
