NAME = ircserv

CXX = c++

FLAGS = -Wall -Wextra -Werror

SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Command.cpp src/Command/Pass.cpp src/Command/Nick.cpp src/Command/executeCommand.cpp 

OBJS = ${SRCS:.cpp=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${FLAGS} ${OBJS} -o $@

%.o: %.cpp includes/Server.hpp includes/Client.hpp includes/Command.hpp
	${CXX} ${FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS}

fclean: clean
	rm -rf ${NAME}

re: fclean all