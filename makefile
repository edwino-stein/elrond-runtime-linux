# Include config file
include Config.mk

BUILD_DIR := $(notdir $(BUILD_DIR))
ELROND_DIR := $(notdir $(ELROND_DIR))
ELROND_BUILD_DIR := $(BUILD_DIR)/$(ELROND_DIR)

# If has a dev version of the ECL in the project, use it
ifeq ($(shell test -L $(ELROND_DIR)-dev -o -d $(ELROND_DIR)-dev; echo $$?), 0)
    ELROND_DIR := $(ELROND_DIR)-dev
endif

.PHONY: all clean clean-all run
.DEFAULT_GOAL := all

# Set JSON config file to run rule
cfg?=teste.json

.PHONY: all clean run clean-base clean-cli clean-all test-lib test-lib-all test-cli test-cli-all\
	    $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME) $(RUNTIME_CLI_PROG_NAME)

# *********************************** RULES ************************************

all: $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME) $(RUNTIME_CLI_PROG_NAME)

$(RUNTIME_LIB_DYNAMIC_NAME): $(ELROND_BUILD_DIR)
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) $@

$(RUNTIME_LIB_STATIC_NAME): $(ELROND_BUILD_DIR)
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) $@

$(RUNTIME_CLI_PROG_NAME): $(RUNTIME_LIB_DYNAMIC_NAME)
	@cd $(RUNTIME_CLI_DIR) && $(MAKE) $@

run: $(RUNTIME_CLI_PROG_NAME)
	./$(BUILD_DIR)/$(RUNTIME_CLI_DIR)/$^ $(cfg)

test-lib: $(ELROND_BUILD_DIR)
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) test t="test/$(notdir $(t))" a="$a"

test-lib-all: $(ELROND_BUILD_DIR)
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) test-all a="$a"

test-cli: $(ELROND_BUILD_DIR) $(RUNTIME_LIB_STATIC_NAME)
	@cd $(RUNTIME_CLI_DIR) && $(MAKE) test t="test/$(notdir $(t))" a="$a"

test-cli-all: $(ELROND_BUILD_DIR) $(RUNTIME_LIB_STATIC_NAME)
	@cd $(RUNTIME_CLI_DIR) && $(MAKE) test-all a="$a"

$(ELROND_BUILD_DIR):
	@cd $(ELROND_DIR) && $(MAKE) all
	@cd $(ELROND_DIR) && $(MAKE) -f $(TEST_MAKEFILE) build/test/catch.cpp.o build/test/external-module.so
	@mkdir -p $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/build/$(ELROND_SHARED_LIB_NAME) $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/build/$(ELROND_STATIC_LIB_NAME) $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/build/$(ELROND_TEST_LIB_NAME) $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/test/elrond-catch.hpp $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/test/catch.hpp $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/build/test/catch.cpp.o $(BUILD_DIR)/elrond
	cp $(ELROND_DIR)/build/test/external-module.so $(BUILD_DIR)/elrond

clean: clean-base clean-cli

clean-base:
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) clean

clean-cli:
	@cd $(RUNTIME_CLI_DIR) && $(MAKE) clean

clean-all:
	rm -rf $(BUILD_DIR)
	@cd $(ELROND_DIR) && $(MAKE) clean
