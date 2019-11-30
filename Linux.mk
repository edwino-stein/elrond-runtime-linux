# General Linux build settings
BUILD_DIR = build
OBJ_EXTENSION = .o
DEPENDENCE_EXTENSION = .d

# Compiler and linker settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -MMD -c
LDFLAGS = -std=c++11 -Wall -rdynamic
LDLIBS =
AR = ar
ARFLAGS = rvs

# Params lists
INCLUDES = $(INCLUDE_DIR)
MACROS = GENERIC_STD_PLATFORM LINUX_PLATFORM
STATIC_LIBRARIES =
DYNAMIC_LIBRARIES = dl pthread m

# Elrond COMMON library setup
COMMON_DIR = elrond
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

OBJS_STANDALONE_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_STANDALONE_FILES))
OBJS_STANDALONE := $(addsuffix $(OBJ_EXTENSION), $(OBJS_STANDALONE_FILES))

# Define dependencies files
DEPS = $(OBJS:$(OBJ_EXTENSION)=$(DEPENDENCE_EXTENSION))

# Add includes and macros to compiler options
CXXFLAGS += $(addprefix -I, $(INCLUDES))
CXXFLAGS += $(addprefix -D, $(MACROS))

# Set Dynamic library to linkage
LDLIBS += $(addprefix -l, $(DYNAMIC_LIBRARIES))

.PHONY: $(PROJECT_NAME) lib$(PROJECT_NAME).a clean-build clean-common clean-build-all

# ************************** BUILD RULES **************************

# Runtime object builder
$(PROJECT_NAME): $(BUILD_DIR)/$(PROJECT_NAME)
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS_STANDALONE) $(BUILD_DIR)/lib$(PROJECT_NAME).a $(COMMON_DIR)/$(COMMON_NONPIC_LIB)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Static library builder
lib$(PROJECT_NAME).a: $(BUILD_DIR)/lib$(PROJECT_NAME).a
$(BUILD_DIR)/lib$(PROJECT_NAME).a: $(OBJS)
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $?

# Objects builder
$(BUILD_DIR)/%.cpp$(OBJ_EXTENSION): $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

# Elrond common library builder
$(COMMON_DIR)/$(COMMON_NONPIC_LIB):
	@cd $(COMMON_DIR) && $(MAKE) $(notdir $@)

# Cleaning rules
clean-build:
	rm -rf $(BUILD_DIR)

clean-common:
	@cd $(COMMON_DIR) && $(MAKE) clean

clean-build-all: clean-build clean-common

# Include all .d files
-include $(DEPS)
