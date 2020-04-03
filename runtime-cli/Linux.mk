################################################################################
#                              Linux build Rules                               #
################################################################################

# Define object files
OBJS_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_FILES))
OBJS := $(addsuffix .$(OBJ_EXT), $(OBJS_FILES))
MAIN_OBJ = $(BUILD_DIR)/main.$(CPP_SRC_EXT).$(OBJ_EXT)

# Define dependencies files
DEPS = $(OBJS:.$(OBJ_EXT)=.$(DEP_EXT)) $(MAIN_OBJ:.$(OBJ_EXT)=.$(DEP_EXT))

# Add includes and macros to compiler options
CXXFLAGS += $(addprefix -I, $(INCLUDES))
CXXFLAGS += $(addprefix -D, $(MACROS))
DYNAMIC_LIBRARIES += dl elrond-runtime elrond

# LD settings
LD_RPATH = $(ldrp)
LDLIBS := -Wl,-rpath,\$$ORIGIN$(LD_RPATH) -L$(BUILD_DIR) $(LDLIBS)
LDLIBS += $(addprefix -l, $(DYNAMIC_LIBRARIES))

################################## BUILD RULES #################################

# Runtime binary builder
$(RUNTIME_CLI_PROG_NAME): $(RUNTIME_CLI_PROG)
$(RUNTIME_CLI_PROG): $(RUNTIME_CLI_STATIC_NAME) $(MAIN_OBJ) $(RUNTIME_LIB_DYNAMIC_NAME)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJS) $(MAIN_OBJ) $(BUILD_DIR)/$(RUNTIME_CLI_STATIC_NAME) -o $@ $(LDLIBS)

# Static library builder
$(RUNTIME_CLI_STATIC_NAME): $(BUILD_DIR)/$(RUNTIME_CLI_STATIC_NAME)
$(BUILD_DIR)/$(RUNTIME_CLI_STATIC_NAME): $(OBJS)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $?

# Objects builder
$(BUILD_DIR)/%.$(CPP_SRC_EXT).$(OBJ_EXT): $(SRC_DIR)/%.$(CPP_SRC_EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

# Include all .d files
-include $(DEPS)
