# General Linux build settings
BUILD_DIR = ../build/runtime-cli
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
INCLUDES = $(INCLUDE_DIR) ../runtime-base/include
MACROS = GENERIC_STD_PLATFORM LINUX_PLATFORM
STATIC_LIBRARIES =
DYNAMIC_LIBRARIES = dl pthread m

# Elrond Runtime base
RT_BASE_DIR = runtime-base
RT_BASE_NAME_LIB = libelrond-runtime
RT_BASE_NOPIC_LIB = ../build/$(RT_BASE_DIR)/$(RT_BASE_NAME_LIB).a
RT_BASE_SO_LIB = ../build/$(RT_BASE_DIR)/$(RT_BASE_NAME_LIB).so
INCLUDES += ../$(RT_BASE_DIR)/include

# Elrond COMMON library setup
COMMON_DIR = ../elrond

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

.PHONY: $(PROJECT_NAME) $(PROJECT_NAME)-wdl clean-build

# ************************** BUILD RULES **************************

# Runtime object builder
$(PROJECT_NAME): $(BUILD_DIR)/$(PROJECT_NAME)
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS) $(RT_BASE_NOPIC_LIB)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(PROJECT_NAME)-wdl: $(BUILD_DIR)/$(PROJECT_NAME)-wdl
$(BUILD_DIR)/$(PROJECT_NAME)-wdl: $(OBJS) $(RT_BASE_SO_LIB)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS) -L$(dir $(RT_BASE_SO_LIB)) -l$(PROJECT_NAME)

# Objects builder
$(BUILD_DIR)/%.cpp$(OBJ_EXTENSION): $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

# Elrond common library builder
$(RT_BASE_NOPIC_LIB):
	@cd ../$(RT_BASE_DIR) && $(MAKE) $(notdir $@)

$(RT_BASE_SO_LIB):
	@cd ../$(RT_BASE_DIR) && $(MAKE) $(notdir $@)

# Cleaning rules
clean-build:
	rm -rf $(BUILD_DIR)

# Include all .d files
-include $(DEPS)
