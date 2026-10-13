NAME        = Webserv
CXX         = c++
INCLUDEDIR	= include
CXXFLAGS    = -Wall -Wextra -Werror -std=c++20 -I $(INCLUDEDIR) -MD

OBJ_DIR     = obj

SRC         = main.cpp \
			  Tokenizer.cpp \

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

DEPS        = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re