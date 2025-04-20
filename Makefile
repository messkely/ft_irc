NAME	= ircserv

FLAGS	= -Werror -Wall -Wextra -std=c++98

HEADERS	= include/Server.hpp include/Client.hpp include/ClientList.hpp include/Channel.hpp		\
		  include/Monitor.hpp include/servSock.h include/error.h include/replies.h				\
		  include/parse.h include/utils.h include/commands/commands.h 							\
		  include/commands/ACommand.hpp include/commands/Pass.hpp include/commands/Nick.hpp		\
		  include/commands/Join.hpp include/commands/Mode.hpp include/commands/Part.hpp			\
		  include/commands/Topic.hpp include/commands/Kick.hpp

SRCS	= src/main.cpp src/msgUtils.cpp src/servSockGen.cpp src/Server.cpp src/Monitor.cpp		\
		  src/Client.cpp src/ClientList.cpp src/Channel.cpp src/userInpParse.cpp src/error.cpp	\
		  src/commands/ACommand.cpp src/commands/Pass.cpp src/commands/Nick.cpp					\
		  src/commands/Join.cpp src/commands/Mode.cpp src/commands/Topic.cpp					\
		  src/commands/Kick.cpp src/commands/Topic.cpp src/commands/Part.cpp

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