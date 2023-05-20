CC = g++

ifeq ($(MODE),debug)
    CFLAGS = -g -w -std=c++17 -m64 -O0 -Wno-missing-braces -Wall
else
    CFLAGS = -std=c++17 -m64 -O0 -mwindows -Wno-missing-braces -Wall
endif

CFLAGS += $(DEFINES)

LDFLAGS = -static-libgcc -static-libstdc++ --static -L./libs/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = dist

SOURCES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/mybasic/*.c $(SRC_DIR)/vm/*.c $(SRC_DIR)/imgui/*.cpp)

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(SOURCES))) \
		  $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SOURCES))) \
          $(patsubst $(SRC_DIR)/vm/%.c,$(OBJ_DIR)/vm/%.o,$(filter $(SRC_DIR)/vm/%.c,$(SOURCES))) \
		  $(patsubst $(SRC_DIR)/imgui/%.cpp,$(OBJ_DIR)/imgui/%.o,$(filter $(SRC_DIR)/imgui/%.cpp,$(SOURCES))) \
		  $(patsubst $(SRC_DIR)/mybasic/%.c,$(OBJ_DIR)/mybasic/%.o,$(filter $(SRC_DIR)/mybasic/%.c,$(SOURCES)))
		  
EXECUTABLE = $(BUILD_DIR)/peryEngine.exe

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -I./src -I./src/include -I./libs/raylib/include -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I./src -I./src/include -I./libs/raylib/include -c $< -o $@

clean:
	del $(EXECUTABLE) $(OBJ_DIR)/*.o
	