TARGET_EXEC ?= a.out

BUILD_DIR ?= build/linux
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.s")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

ADDITIONAL_OBJS := 
#thirdparty/glad/glad.o


INC_DIRS := $(shell find $(SRC_DIRS) -type d) include/ thirdparty/
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_DIRS := lib/
LIB_FLAGS := $(addprefix -L,$(LIB_DIRS))
LDFLAGS := -lGL -lglfw3 -lX11 -lpthread -ldl -lglad

CPPFLAGS ?= $(INC_FLAGS) $(LIB_FLAGS) -MMD -MP -std=c++17 -DGLFW_INCLUDE_NONE

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) ${LIB_FLAGS} ${ADDITIONAL_OBJS} -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

run:
	build/linux/a.out


-include $(DEPS)

MKDIR_P ?= mkdir -p