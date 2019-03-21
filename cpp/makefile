NAME = ifinterp
LD = g++
CXX = g++
CXX_FLAGS = -std=c++17 -Wall -Wextra
CDBG_FLAGS = -ggdb
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
INCLUDE = -I$(INC_DIR)
_OBJ = main.o \
       Object.o
OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(LD) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(INCLUDE) $(CXX_FLAGS) $(CDBG_FLAGS) -c $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean
clean:
	@rm -rfv $(OBJ_DIR)
	@rm -fv $(NAME)
