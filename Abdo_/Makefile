NAME		:=	Abdo_Part.out
CC			:=	c++
CC_FLAGS	:=	-Wall -Wextra -Werror -std=c++20
SRC_DIR		:=	src
OBJ_DIR		:=	build
SRC			:=	main.cpp \
				
OBJ			:=	$(addprefix $(OBJ_DIR)/,$(SRC:.cpp=.o))
DEP			:=	$(OBJ:%.o=%.d)
HEADERS		:=	-I include/
VPATH		:=	$(SRC_DIR)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CC_FLAGS) $(HEADERS) $(OBJ) -o $(@)

$(OBJ_DIR)/%.o : %.cpp | build
	bear --append -- $(CC) $(CC_FLAGS) -MMD -o $@ -c $< $(HEADERS)

build :
	mkdir -p $(OBJ_DIR)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

-include $(DEP)

.PHONY : all clean re fclean
