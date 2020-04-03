# General settings
include ../Config.mk

#Specific settings
BUILD_DIR := $(BUILD_DIR)/$(RUNTIME_CLI_DIR)/$(TEST_DIR)

INCLUDES += $(ELROND_BUILD_DIR)

INCLUDES := $(addprefix -I, $(i) $(INCLUDES))
MACROS := $(addprefix -D, $(m) $(MACROS))

LDLIBS = $(addprefix -l, $(DYNAMIC_LIBRARIES) $(ld))
LSLIBS = $(ELROND_CATCH_OBJ) $(l)

ALL_TEST_SRC = $(shell find $(TEST_DIR) -type f \( -name "*.test.cpp" \))

VPATH = src: $(TEST_DIR)
vpath %.cpp $(TEST_DIR)

CXXFLAGS += $(INCLUDES) $(MACROS)
LDFLAGS += $(INCLUDES) $(MACROS)

.PHONY: all run clean

# *********************************** RULES ************************************

all: $(ALL_TEST_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(ALL_TEST_SRC) $(LSLIBS) -o $(BUILD_DIR)/all.test $(LDLIBS)
	./$(BUILD_DIR)/all.test $(a)

run: $(BUILD_DIR)/$(notdir $(basename $(t)))
	./$^ $a

# Generic test runner builder
$(BUILD_DIR)/%.test: %.test.cpp
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ $(LSLIBS) $(MACROS) -o $@  $(LDLIBS)
