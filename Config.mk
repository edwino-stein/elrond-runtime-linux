################################################################################
#                            General build settings                            #
################################################################################

# Main settings
PROJECT_NAME = runtime
SRC_DIR = src
SRC_TEST_DIR = $(SRC_DIR)/test
INCLUDE_DIR = include
BUILD_DIR = ../build
DIST_DIR = dist

# Util
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
ELROND_SHARED_LIB_NAME = libelrond.$(DYNAMIC_LIB_EXT)
ELROND_STATIC_LIB_NAME = libelrond.$(STATIC_LIB_EXT)

# If has a dev version of the ECL in the project, use it
ifeq ($(shell test -L $(ELROND_DIR)-dev -o -d $(ELROND_DIR)-dev; echo $$?), 0)
    ELROND_DIR := $(ELROND_DIR)-dev
endif

ELROND_SHARED_LIB = $(ELROND_DIR)/build/$(ELROND_SHARED_LIB_NAME)

#Runtime library settings
RUNTIME_LIB_DIR := $(PROJECT_NAME)-lib
RUNTIME_LIB_DYNAMIC_NAME = libelrond-runtime.$(DYNAMIC_LIB_EXT)
RUNTIME_LIB_STATIC_NAME = libelrond-runtime.$(STATIC_LIB_EXT)

#Runtime cli settings
RUNTIME_CLI_DIR := $(PROJECT_NAME)-cli
RUNTIME_CLI_PROG_NAME = elrond-runtime
