# Main settings
PROJECT_NAME = elrond-runtime

BASE_RT_DIR = runtime-base
CLI_RT_DIR = runtime-cli

.PHONY: all clean clean-all run
.DEFAULT_GOAL := all

# Set JSON config file to run rule
cfg?=teste.json

.PHONY: all clean run clean-base clean-cli $(PROJECT_NAME) $(PROJECT_NAME)-wdl lib$(PROJECT_NAME).a lib$(PROJECT_NAME).so

# *********************************** RULES ************************************

all: lib$(PROJECT_NAME).a lib$(PROJECT_NAME).so $(PROJECT_NAME) $(PROJECT_NAME)-wdl

run: $(PROJECT_NAME)
	./build/$(CLI_RT_DIR)/$(PROJECT_NAME) $(cfg)

run-wdl: $(PROJECT_NAME)-wdl
	./build/$(CLI_RT_DIR)/$(PROJECT_NAME)-wdl $(cfg)

$(PROJECT_NAME):
	@cd $(CLI_RT_DIR) && $(MAKE) $(notdir $@)

$(PROJECT_NAME)-wdl:
	@cd $(CLI_RT_DIR) && $(MAKE) $(notdir $@)

lib$(PROJECT_NAME).a:
	@cd $(BASE_RT_DIR) && $(MAKE) $(notdir $@)

lib$(PROJECT_NAME).so:
	@cd $(BASE_RT_DIR) && $(MAKE) $(notdir $@)

clean:
	rm -rf build

clean-base:
	@cd $(BASE_RT_DIR) && $(MAKE) clean

clean-cli:
	@cd $(CLI_RT_DIR) && $(MAKE) clean
