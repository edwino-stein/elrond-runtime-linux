################################################################################
#                              Linux build Rules                               #
################################################################################

# Define object files
OBJS_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_FILES))
OBJS := $(addsuffix .$(OBJ_EXT), $(OBJS_FILES))

# Set PIC objects
OBJS_PIC := $(addsuffix .pic.$(OBJ_EXT), $(OBJS_FILES))

# Set test objects
OBJ_TEST_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_TEST_FILES))

# Define dependencies files
DEPS = $(OBJS:.$(OBJ_EXT)=.$(DEP_EXT)) $(OBJS_PIC:.$(OBJ_EXT)=.$(DEP_EXT))

# Add includes and macros to compiler options
CXXFLAGS += $(addprefix -I, $(INCLUDES))
CXXFLAGS += $(addprefix -D, $(MACROS))
DYNAMIC_LIBRARIES += elrond

# LD settings
LD_RPATH = $(ldrp)
LDLIBS := -Wl,-rpath,\$$ORIGIN$(LD_RPATH) -L$(BUILD_DIR) $(LDLIBS)
LDLIBS += $(addprefix -l, $(DYNAMIC_LIBRARIES))

################################## BUILD RULES #################################

# Shared library builder
$(RUNTIME_LIB_DYNAMIC_NAME): $(RUNTIME_LIB_DYNAMIC)
$(RUNTIME_LIB_DYNAMIC): $(OBJS_PIC) $(ELROND_SHARED_LIB_NAME)
	@mkdir -p $(@D)
	$(CXX) -shared $(LDFLAGS) $(OBJS_PIC) -o $@ $(LDLIBS)

# PIC objects builder
$(BUILD_DIR)/%.$(CPP_SRC_EXT).pic.$(OBJ_EXT): $(SRC_DIR)/%.$(CPP_SRC_EXT)
	@mkdir -p $(@D)
	$(CXX) -fPIC $(CXXFLAGS) -DPIC $< -o $@

# Static library builder
$(RUNTIME_LIB_STATIC_NAME): $(RUNTIME_LIB_STATIC)
$(RUNTIME_LIB_STATIC): $(OBJS)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $?

# Objects builder
$(BUILD_DIR)/%.$(CPP_SRC_EXT).$(OBJ_EXT): $(SRC_DIR)/%.$(CPP_SRC_EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

# Include all .d files
-include $(DEPS)
