##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Compiles CXX files
##

CC	= clang++

SRC_DIR	=	src/

SRC_FILES	=	main.cpp

SRC	=	$(addprefix $(SRC_DIR), $(SRC_FILES))

DIR_OBJ	=	.obj/

OBJ	=	$(SRC:%.cpp=$(DIR_OBJ)%.o)

CPPFLAGS	=	-std=c++20 -Iinclude

DFLAGS	=	-g -Wall -Wextra -Werror

BINARY	=	my_ftp

all: 	$(BINARY)

$(BINARY):	$(OBJ)
		@$(CC) $^ -o $@ $(CPPFLAGS)
		@echo -e "\x1b[36mMakefile -> compile\x1b[0m"

$(DIR_OBJ)%.o:	%.cpp
		@mkdir -p $(dir $@)
		@$(CC) $^ -c -o $@ $(CPPFLAGS)
		@echo -e "\x1b[36mMakefile -> $@\x1b[0m"

clean:
		@rm -rf $(DIR_OBJ)
		@rm -rf .ropeproject
		@rm -rf .cache
		@echo -e "\x1b[35mMakefile -> clean\x1b[0m"

fclean: clean
		@rm -f $(BINARY)
		@rm -f $(BINARY_TEST)
		@rm -f *.log
		@rm -f *.gcov
		@rm -f *.gcda
		@rm -f *.gcno
		@rm -rf .cache
		@rm -f vgcore.*
		@rm -rf $(DIR_OBJ)
		@echo -e "\x1b[35mMakefile -> fclean\x1b[34m"

re:		fclean all
		@rm -rf .ropeproject
		@rm -rf $(DIR_OBJ)
		@echo -e "\x1b[31mMakefile -> re\x1b[0m"


.PHONY: all clean fclean re
