# Include config file
include ../Config.mk

#Specific settings
BUILD_DIR := $(BUILD_DIR)/$(RUNTIME_CLI_DIR)

# Define includes
INCLUDES += $(INCLUDE_DIR) ../$(RUNTIME_LIB_DIR)/include $(ELROND_DIR)/include

# Search for all srcs and includes files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name "*.$(CPP_SRC_EXT)" -or -name "*.$(C_SRC_EXT)" \) -and -not -name "main.$(CPP_SRC_EXT)" )

# Define paths and source file extensions
VPATH = src: $(SRC_DIR)
vpath %.$(CPP_SRC_EXT) $(SRC_DIR)
vpath %.$(C_SRC_EXT) $(SRC_DIR)

.PHONY: all clean prepare-build test test-all $(RUNTIME_CLI_PROG_NAME) $(RUNTIME_CLI_STATIC_NAME) $(RUNTIME_LIB_DYNAMIC_NAME)
.DEFAULT_GOAL := all

################################## BUILD RULES #################################

# Include Platform target build rules
include $(TARGET_OS).mk

all: $(RUNTIME_CLI_STATIC_NAME) $(RUNTIME_CLI_PROG_NAME)

clean:
	rm -rf $(BUILD_DIR)

test: $(RUNTIME_CLI_STATIC) $(RUNTIME_LIB_STATIC) $(ELROND_TEST_STATIC_LIB) $(ELROND_STATIC_LIB)
	@rm -f $(BUILD_DIR)/test/$(notdir $(basename $(t)))
	@$(MAKE) --no-print-directory -f $(TEST_MAKEFILE) run t="$(t)" l="$^" i="$(INCLUDES)" a="$a" ld="dl"

test-all: $(RUNTIME_CLI_STATIC) $(RUNTIME_LIB_STATIC) $(ELROND_TEST_STATIC_LIB) $(ELROND_STATIC_LIB)
	@rm -f $(BUILD_DIR)/test/all.test
	@$(MAKE) --no-print-directory -f $(TEST_MAKEFILE) all l="$^" i="$(INCLUDES)" a="$a" ld="dl"

$(RUNTIME_LIB_DYNAMIC_NAME): $(BUILD_DIR)/$(RUNTIME_LIB_DYNAMIC_NAME)
$(BUILD_DIR)/$(RUNTIME_LIB_DYNAMIC_NAME):
	@mkdir -p $(@D)
	cp $(RUNTIME_LIB_DYNAMIC) $@
	cp $(dir $(RUNTIME_LIB_DYNAMIC))$(ELROND_SHARED_LIB_NAME) $(dir $@)
