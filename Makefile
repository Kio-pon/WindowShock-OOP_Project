# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O0 -pipe -I"include" -I"C:/msys64/msys64/include" -DSFML_STATIC
LDFLAGS = -L"C:/msys64/msys64/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lharfbuzz -lopengl32 -lwinmm -lgdi32

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/windowshock.exe

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(TARGET) del $(TARGET)

.PHONY: all clean
