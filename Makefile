CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -L. -llogger
LDLIBS = -pthread

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
LIB_NAME = liblogger.so

LIB_SRC = $(SRC_DIR)/logger.cpp
LIB_OBJ = $(OBJ_DIR)/logger.o

MAIN_SRC = $(SRC_DIR)/main.cpp
MAIN_OBJ = $(OBJ_DIR)/main.o
MAIN_BIN = $(BIN_DIR)/logapp

APP_SRC = $(SRC_DIR)/app.cpp
APP_OBJ = $(OBJ_DIR)/app.o
APP_BIN = $(BIN_DIR)/logconsole

all: lib app test

lib: $(LIB_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -shared -o $(BIN_DIR)/$(LIB_NAME) $(LIB_OBJ)

test: $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) -o $(MAIN_BIN) -L$(BIN_DIR) -llogger

app: $(APP_OBJ)
	$(CXX) $(CXXFLAGS) $(APP_OBJ) -o $(APP_BIN) -L$(BIN_DIR) -llogger $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) log.txt

.PHONY: all lib app test clean