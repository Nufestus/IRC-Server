NAME = ircserv

CXX = c++

FLAGS = -Wall -Wextra -Werror

SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Command.cpp src/CommandManager.cpp src/Channel.cpp \
		src/Command/Privmsg.cpp src/Command/Pass.cpp src/Command/Nick.cpp src/Command/User.cpp src/Command/Quit.cpp \
		src/Command/Join.cpp src/Command/Invite.cpp src/Command/Topic.cpp src/Command/Kick.cpp src/Command/Mode.cpp

OBJS = ${SRCS:.cpp=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${FLAGS} ${OBJS} -o $@

%.o: %.cpp includes/Server.hpp includes/Client.hpp includes/Command.hpp includes/CommandManager.hpp includes/Channel.hpp includes/IRCReplies.hpp
	${CXX} ${FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS}

fclean: clean
	rm -rf ${NAME}

re: fclean all