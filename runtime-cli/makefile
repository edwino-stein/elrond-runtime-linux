# Main settings
PROJECT_NAME = elrond-runtime
SRC_DIR = src
INCLUDE_DIR = include

# Search for all srcs and includes files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name "*.cpp" -or -name "*.c" \))
INCLUDE_FILES = $(shell find $(INCLUDE_DIR) -type f \( -name "*.hpp" -or -name "*.h" \) )

# Defines src path and source file extensions
VPATH = src: $(SRC_DIR)
vpath %.hpp $(INCLUDE_DIR)
vpath %.h $(INCLUDE_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.c $(SRC_DIR)

.PHONY: all clean clean-all run
.DEFAULT_GOAL := all

# *********************************** RULES ************************************
include Linux.mk

all: $(PROJECT_NAME) $(PROJECT_NAME)-wdl
clean: clean-build
clean-all: clean-build-all
