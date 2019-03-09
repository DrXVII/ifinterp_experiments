NAME = ifinterp
LD = g++
CXX = g++
CXX_FLAGS = -std=c++17 -Wall -Wextra
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
_OBJ = main.o
OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

all: $(NAME)

$(NAME): $(OBJ)
	$(LD) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(INCLUDE) $(CXX_FLAGS) -c $<
