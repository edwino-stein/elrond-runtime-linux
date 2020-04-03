# Include config file
include ../Config.mk

#Specific settings
BUILD_DIR := $(BUILD_DIR)/$(RUNTIME_LIB_DIR)

# Define includes
INCLUDES += $(INCLUDE_DIR) $(ELROND_DIR)/include

# Search for all srcs and includes files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name "*.$(CPP_SRC_EXT)" -or -name "*.$(C_SRC_EXT)" \) )

# Define paths and source file extensions
VPATH = src: $(SRC_DIR)
vpath %.$(CPP_SRC_EXT) $(SRC_DIR)
vpath %.$(C_SRC_EXT) $(SRC_DIR)

.PHONY: all clean test test-all $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME) $(ELROND_SHARED_LIB_NAME)
.DEFAULT_GOAL := all

################################## BUILD RULES #################################

# Include Platform target build rules
include $(TARGET_OS).mk

all: $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME)

clean:
	rm -rf $(BUILD_DIR)

test: $(RUNTIME_LIB_STATIC) $(ELROND_TEST_STATIC_LIB) $(ELROND_STATIC_LIB)
	@rm -f $(BUILD_DIR)/test/$(notdir $(basename $(t)))
	@$(MAKE) --no-print-directory -f $(TEST_MAKEFILE) run t="$(t)" l="$^" i="$(INCLUDES)" a="$a"

test-all: $(RUNTIME_LIB_STATIC) $(ELROND_TEST_STATIC_LIB) $(ELROND_STATIC_LIB)
	@rm -f $(BUILD_DIR)/test/all.test
	@$(MAKE) --no-print-directory -f $(TEST_MAKEFILE) all l="$^" i="$(INCLUDES)" a="$a"

# Dynamic Elrond Common Library
$(ELROND_SHARED_LIB_NAME): $(BUILD_DIR)/$(ELROND_SHARED_LIB_NAME)
$(BUILD_DIR)/$(ELROND_SHARED_LIB_NAME):
	cp $(ELROND_SHARED_LIB) $@
