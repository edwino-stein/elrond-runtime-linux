################################################################################
#                            General build settings                            #
################################################################################

# Main settings
PROJECT_NAME = runtime
SRC_DIR = src
TEST_DIR = test
INCLUDE_DIR = include
BUILD_DIR = ../build
DIST_DIR = dist

# Util
TEST_MAKEFILE = Test.mk
INCLUDES =
MACROS =

# File extensions
CPP_SRC_EXT = cpp
C_SRC_EXT = c
HPP_SRC_EXT = hpp
IPP_SRC_EXT = ipp
OBJ_EXT = o
DEP_EXT = d
DYNAMIC_LIB_EXT = so
STATIC_LIB_EXT = a

TARGET_OS = $(shell uname -s)
ifeq ($(TARGET_OS), Linux)
    # Linux compiler and linker settings
    CXX = g++
    CXXFLAGS = -std=c++11 -Wall -MMD -c
    LDFLAGS = -std=c++11 -Wall
    LDLIBS =
    AR = ar
    ARFLAGS = rs
    LN = ln
    LNFLAGS = -s
    DYNAMIC_LIBRARIES = pthread m
endif

# Elrond Common library
ELROND_DIR = ../elrond
ELROND_BUILD_DIR := $(BUILD_DIR)/$(notdir $(ELROND_DIR))
ELROND_SHARED_LIB_NAME = libelrond.$(DYNAMIC_LIB_EXT)
ELROND_STATIC_LIB_NAME = libelrond.$(STATIC_LIB_EXT)
ELROND_TEST_LIB_NAME = libelrond-test.$(STATIC_LIB_EXT)
ELROND_HEADERONLY_NAME = elrond.inline.$(HPP_SRC_EXT)
ELROND_CATCH_OBJ_NAME = catch.$(CPP_SRC_EXT).$(OBJ_EXT)

# If has a dev version of the ECL in the project, use it
ifeq ($(shell test -L $(ELROND_DIR)-dev -o -d $(ELROND_DIR)-dev; echo $$?), 0)
    ELROND_DIR := $(ELROND_DIR)-dev
endif

ELROND_SHARED_LIB := $(ELROND_BUILD_DIR)/$(ELROND_SHARED_LIB_NAME)
ELROND_STATIC_LIB := $(ELROND_BUILD_DIR)/$(ELROND_STATIC_LIB_NAME)
ELROND_TEST_STATIC_LIB = $(ELROND_BUILD_DIR)/$(ELROND_TEST_LIB_NAME)
ELROND_CATCH_OBJ = $(ELROND_BUILD_DIR)/$(ELROND_CATCH_OBJ_NAME)

#Runtime library settings
RUNTIME_LIB_DIR := $(PROJECT_NAME)-lib
RUNTIME_LIB_DYNAMIC_NAME = libelrond-runtime.$(DYNAMIC_LIB_EXT)
RUNTIME_LIB_STATIC_NAME = libelrond-runtime.$(STATIC_LIB_EXT)
RUNTIME_LIB_DYNAMIC := $(BUILD_DIR)/$(RUNTIME_LIB_DIR)/$(RUNTIME_LIB_DYNAMIC_NAME)
RUNTIME_LIB_STATIC := $(BUILD_DIR)/$(RUNTIME_LIB_DIR)/$(RUNTIME_LIB_STATIC_NAME)

#Runtime cli settings
RUNTIME_CLI_DIR := $(PROJECT_NAME)-cli
RUNTIME_CLI_PROG_NAME = elrond-runtime
RUNTIME_CLI_STATIC_NAME = $(RUNTIME_CLI_PROG_NAME).$(STATIC_LIB_EXT)
RUNTIME_CLI_PROG := $(BUILD_DIR)/$(RUNTIME_CLI_DIR)/$(RUNTIME_CLI_PROG_NAME)
RUNTIME_CLI_STATIC := $(BUILD_DIR)/$(RUNTIME_CLI_DIR)/$(RUNTIME_CLI_STATIC_NAME)
