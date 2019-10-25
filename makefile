# Main settings
PROJECT_NAME = elrond-runtime
SRC_DIR_ROOT = src
INCLUDE_DIR_ROOT = include

# Search for all srcs and includes files
SRC_FILES = $(shell find $(SRC_DIR_ROOT) -type f \( -name "*.cpp" -or -name "*.c" \) )
INCLUDE_FILES = $(shell find $(INCLUDE_DIR_ROOT) -type f \( -name "*.hpp" -or -name "*.h" \) )

# Defines src path and source file extensions
VPATH = src: $(SRC_DIR_ROOT)
vpath %.hpp $(INCLUDE_DIR_ROOT)
vpath %.h $(INCLUDE_DIR_ROOT)
vpath %.cpp $(SRC_DIR_ROOT)
vpath %.c $(SRC_DIR_ROOT)

# Set JSON config file to run rule
# runcfg?=config.json

.PHONY: all clean clean-all run
.DEFAULT_GOAL := all
cfg?=teste.json

# *********************************** RULES ************************************
include Linux.mk

all: $(PROJECT_NAME)
clean: clean-build
clean-all: clean-build-all

run: $(PROJECT_NAME)$(RT_EXTENSION)
	./$(BUILD_DIR_ROOT)/$(PROJECT_NAME)$(RT_EXTENSION) $(cfg)
