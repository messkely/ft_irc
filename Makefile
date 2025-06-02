NAME	= ircserv

FLAGS	= -Werror -Wall -Wextra -std=c++98

HEADERS	= include/Server.hpp include/Client.hpp include/ClientList.hpp include/Channel.hpp				\
		  include/Monitor.hpp include/servSock.h include/error.h include/replies.h						\
		  include/parse.h include/utils.h include/commands/commands.h 									\
		  include/commands/ACommand.hpp include/commands/Pass.hpp include/commands/Nick.hpp				\
		  include/commands/User.hpp include/commands/Join.hpp  include/commands/Part.hpp				\
		  include/commands/Topic.hpp include/commands/Kick.hpp include/commands/Mode.hpp				\
		  include/commands/Quit.hpp include/commands/Privmsg.hpp include/commands/Invite.hpp

# restore src/utils/inet_ntoa6.cpp later
SRCS	= src/main.cpp src/utils/msgUtils.cpp src/utils/servSockGen.cpp		\
		  src/utils/userInpParse.cpp src/utils/error.cpp src/utils/stringUtils.cpp src/Monitor.cpp		\
		  src/Server.cpp src/Client.cpp src/ClientList.cpp src/Channel.cpp src/commands/ACommand.cpp	\
		  src/commands/Pass.cpp src/commands/Nick.cpp src/commands/User.cpp src/commands/Join.cpp		\
		  src/commands/Topic.cpp src/commands/Kick.cpp	src/commands/Topic.cpp src/commands/Part.cpp	\
		  src/commands/Mode.cpp src/commands/Quit.cpp src/commands/Privmsg.cpp src/commands/Invite.cpp	\
		  

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