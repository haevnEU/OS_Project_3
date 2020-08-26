# compiler
CXX      := -c++

# flags vor the compiler
CXXFLAGS :=  -Werror 

# Libraries
LDFLAGS  := -L/usr/lib -lstdc++ -lm 

# Important directories
BUILD    := ./build
# Object directory
OBJ_DIR  := $(BUILD)/objects
# 
APP_DIR  := $(BUILD)/apps
# Directory for custom libraries
LIBDIR   := $(BUILD)/apps/lib

# Application name
TARGET   := OperatingSystemProject

# Include path
INCLUDE  := -Iinclude/
# Source files
SRC      :=                      \
   $(wildcard src/core/*.cpp)   \
   $(wildcard src/tui/*.cpp)   \
   $(wildcard src/*.cpp)         

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# Step for all builds
all: build $(APP_DIR)/$(TARGET)

# Step to create the Object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -c $< -o $@ $(LDFLAGS) ./lib/liblua54.so -Wl,--no-as-needed -ldl

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(APP_DIR)/$(TARGET) $^  $(LDFLAGS) ./lib/liblua54.so -Wl,--no-as-needed -ldl

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	@mkdir -p $(LIBDIR)
	@cp ./lib/liblua54.so $(LIBDIR)/liblua54.so