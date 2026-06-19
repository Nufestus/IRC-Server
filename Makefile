NAME = ircserv

B_NAME = ircbot

CXX = c++

FLAGS = -Wall -Wextra -Werror

SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Command.cpp src/CommandManager.cpp src/Channel.cpp src/Command/Privmsg.cpp src/Command/Pass.cpp src/Command/Nick.cpp src/Command/User.cpp src/Command/Quit.cpp src/Command/Join.cpp src/Command/Invite.cpp

B_SRCS = bonus/IrcBot.cpp bonus/main.cpp

OBJS = ${SRCS:.cpp=.o}

B_OBJS = $(B_SRCS:.cpp=.o)

all: ${NAME}

bonus: $(B_NAME)

$(B_NAME) : $(B_OBJS)
	$(CXX) $(FLAGS) $(B_OBJS) -o $@

${NAME}: ${OBJS}
	${CXX} ${FLAGS} ${OBJS} -o $@

%.o: %.cpp includes/Server.hpp includes/Client.hpp includes/Command.hpp includes/CommandManager.hpp includes/Channel.hpp includes/IRCReplies.hpp bonus/IrcBot.hpp
	${CXX} ${FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS}

bclean:
	rm -rf $(B_OBJS)

fclean: clean
	rm -rf ${NAME}

bfclean: bclean
	rm -rf $(B_NAME)

re: fclean all

rebonus: bfclean bonus