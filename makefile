# Main settings
PROJECT_NAME = elrond-runtime

BASE_RT_DIR = runtime-base
CLI_RT_DIR = runtime-cli

.PHONY: all clean clean-all run
.DEFAULT_GOAL := all

# Set JSON config file to run rule
cfg?=teste.json

.PHONY: all clean run clean-base clean-cli

# *********************************** RULES ************************************

all: build/$(BASE_RT_DIR)/lib$(PROJECT_NAME).a build/$(CLI_RT_DIR)/$(PROJECT_NAME)

run: build/$(CLI_RT_DIR)/$(PROJECT_NAME)
	./build/$(CLI_RT_DIR)/$(PROJECT_NAME) $(cfg)

build/$(CLI_RT_DIR)/$(PROJECT_NAME):
	@cd $(CLI_RT_DIR) && $(MAKE) $(notdir $@)

build/$(BASE_RT_DIR)/lib$(PROJECT_NAME).a:
	@cd $(BASE_RT_DIR) && $(MAKE) $(notdir $@)

clean:
	rm -rf build

clean-base:
	@cd $(BASE_RT_DIR) && $(MAKE) clean

clean-cli:
	@cd $(CLI_RT_DIR) && $(MAKE) clean
