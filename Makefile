NAME = irc

CXX = c++

FLAGS = -Wall -Wextra -Werror

SRCS = src/main.cpp src/Server.cpp

OBJS = ${SRCS:.cpp=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${FLAGS} ${OBJS} -o $@

%.o: %.cpp includes/Server.hpp
	${CXX} ${FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS}

fclean: clean
	rm -rf ${NAME}

re: fclean all