NAME = artimon_pi 

SRC_DIR = ./src
OBJ_DIR = ./obs
INC_DIR = ./inc

SRC_FILES = \
./src/main.c \
./src/window.c \

OBJ_FILES = $(SRC_FILES:.c=.o)

INC_FILES = \
	./inc/artimon_pi.h \
	./inc/window.h \

OBJ_FILES = $(SRC_FILES:.c=.o)

FLAGS = -Wall -Werror -Wextra

.PHONY: all, clean, fclean, re

$(NAME): $(OBJ_FILES)
	gcc -g3 -o $(NAME) $(OBJ_FILES) -framework SDL2

all: $(NAME)

%.o:%.c $(INC_FILES)
	gcc -g -c $(FLAGS) -I$(INC_DIR) $< -o $@

clean:
	rm -rf $(OBJ_FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all
