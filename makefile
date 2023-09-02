BUILD_DIR = build

BIN_DIR = $(BUILD_DIR)/bin
BIN_NAME = aro
BIN = $(BIN_DIR)/$(BIN_NAME)

SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS_DIR = $(BUILD_DIR)/objects
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJECTS_DIR)/%.o, $(SOURCES))

CC = clang++
STD = c++20

$(BIN): $(SRC_DIR) $(make_dirs) $(SOURCES)
	$(CC) -std=$(STD) $(SOURCES) -o $(BIN)

run: $(BIN)
	./$(BIN)

all: $(SRC_DIR) $(make_dirs) $(SOURCES)
	$(CC) -std=$(STD) $(SOURCES) -o $(BIN)

from_objects: $(OBJECTS)
	$(CC) -std=$(STD) -o $(BIN) $(OBJECTS)

objs: $(OBJECTS)

$(OBJECTS): $(OBJECTS_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) -std=$(STD) -c $< -o $@

clean: $(BUILD_DIR)
	rm -r $(BUILD_DIR)

make_dirs: clean $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	mkdir $(BIN_DIR)
	mkdir $(OBJECTS_DIR)
