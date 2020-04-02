# Include config file
include ../Config.mk

#Specific settings
PROJECT_NAME := $(RUNTIME_LIB_DIR)
BUILD_DIR := $(BUILD_DIR)/$(PROJECT_NAME)

# Define includes
INCLUDES = $(INCLUDE_DIR) $(ELROND_DIR)/include

# Search for all srcs and includes files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name "*.$(CPP_SRC_EXT)" -or -name "*.$(C_SRC_EXT)" \) -and -not -path "$(SRC_TEST_DIR)/*" )
# SRC_TEST_FILES = $(shell find $(SRC_TEST_DIR) -type f \( -name "*.$(CPP_SRC_EXT)" -or -name "*.$(C_SRC_EXT)" \) )

# Define paths and source file extensions
VPATH = src: $(SRC_DIR)
vpath %.$(CPP_SRC_EXT) $(SRC_DIR)
vpath %.$(C_SRC_EXT) $(SRC_DIR)

.PHONY: all clean prepare-build test test-all lib$(PROJECT_NAME).$(STATIC_LIB_EXT) lib$(PROJECT_NAME).$(DYNAMIC_LIB_EXT)
.DEFAULT_GOAL := all

################################## BUILD RULES #################################

# Include Platform target build rules
include $(TARGET_OS).mk

all: $(RUNTIME_LIB_DYNAMIC_NAME) $(RUNTIME_LIB_STATIC_NAME)

clean:
	rm -rf $(BUILD_DIR)
