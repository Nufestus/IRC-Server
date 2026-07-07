NAME = ircserv
BONUS_NAME = ircbot

CXX = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = mandatory/src/main.cpp mandatory/src/Server.cpp mandatory/src/Client.cpp mandatory/src/Command.cpp mandatory/src/CommandManager.cpp mandatory/src/Channel.cpp \
		mandatory/src/Command/Privmsg.cpp mandatory/src/Command/Pass.cpp mandatory/src/Command/Nick.cpp mandatory/src/Command/User.cpp mandatory/src/Command/Quit.cpp \
		mandatory/src/Command/Join.cpp mandatory/src/Command/Invite.cpp mandatory/src/Command/Topic.cpp mandatory/src/Command/Kick.cpp mandatory/src/Command/Mode.cpp

BONUS_SRCS = bonus/IrcBot.cpp bonus/main.cpp

OBJS = ${SRCS:.cpp=.o}

BONUS_OBJS = ${BONUS_SRCS:.cpp=.o}

all: ${NAME}

bonus: ${BONUS_NAME}

${NAME}: ${OBJS}
	${CXX} ${FLAGS} ${OBJS} -o $@

${BONUS_NAME}: ${BONUS_OBJS}
	${CXX} ${FLAGS} ${BONUS_OBJS} -o $@

mandatory/%.o: mandatory/%.cpp mandatory/includes/Server.hpp mandatory/includes/Client.hpp mandatory/includes/Command.hpp mandatory/includes/CommandManager.hpp mandatory/includes/Channel.hpp mandatory/includes/ModeChange.hpp
	${CXX} ${FLAGS} -c $< -o $@

bonus/%.o: bonus/%.cpp bonus/IrcBot.hpp
	${CXX} ${FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS} ${BONUS_OBJS}

fclean: clean
	rm -rf ${NAME} ${BONUS_NAME}

re: fclean all