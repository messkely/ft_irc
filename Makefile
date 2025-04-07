NAME	= ircserv

FLAGS	= -Werror -Wall -Wextra -std=c++98

HEADERS	= include/Server.hpp include/Client.hpp include/ClientList.hpp include/Monitor.hpp \
		  include/commands/ACommand.hpp include/commands/Pass.hpp include/error.h \
		  include/parse.h include/servSock.h include/utils.h

SRCS	= src/main.cpp src/Server.cpp src/Monitor.cpp src/Client.cpp src/ClientList.cpp \
		  src/commands/ACommand.cpp src/userInpParse.cpp src/error.cpp src/split.cpp src/commands/Pass.cpp \
		  src/servSockGen.cpp src/utils.cpp

OBJS	= $(SRCS:.cpp=.o)

CXX		= c++

all	: $(NAME)

%.o	: %.cpp $(HEADERS)
	$(CXX) -c $< -o $@ $(FLAGS)

$(NAME)	: $(OBJS)
	$(CXX) $^ -o $@ $(FLAGS) -fsanitize=address

clean	:
	@rm -rf $(OBJS)

fclean	: clean
	@rm -rf $(NAME)

re	: fclean all

.PHONY	: clean