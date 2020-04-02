# Include config file
include Config.mk

BUILD_DIR := $(notdir $(BUILD_DIR))

ELROND_DIR := $(notdir $(ELROND_DIR))
# If has a dev version of the ECL in the project, use it
ifeq ($(shell test -L $(ELROND_DIR)-dev -o -d $(ELROND_DIR)-dev; echo $$?), 0)
    ELROND_DIR := $(ELROND_DIR)-dev
endif

.PHONY: all clean clean-all run
.DEFAULT_GOAL := all

# Set JSON config file to run rule
cfg?=teste.json

.PHONY: all clean run clean-base clean-cli clean-all $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME) $(RUNTIME_CLI_PROG_NAME)

# *********************************** RULES ************************************

all: $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME) $(RUNTIME_CLI_PROG_NAME)

$(RUNTIME_LIB_DYNAMIC_NAME):
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) $@

$(RUNTIME_LIB_STATIC_NAME):
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) $@

$(RUNTIME_CLI_PROG_NAME):
	@cd $(RUNTIME_CLI_DIR) && $(MAKE) $@

run: $(RUNTIME_CLI_PROG_NAME)
	./$(BUILD_DIR)/$(RUNTIME_CLI_DIR)/$^ $(cfg)

clean:
	rm -rf build

clean-base:
	@cd $(RUNTIME_LIB_DIR) && $(MAKE) clean

clean-cli:
	@cd $(RUNTIME_CLI_DIR) && $(MAKE) clean

clean-all: clean
	@cd $(ELROND_DIR) && $(MAKE) clean
