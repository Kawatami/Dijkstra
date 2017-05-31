CXX=g++
CXXFLAGS= -Wall -Wextra -Werror -std=c++14 -pedantic -g 

EXEC=main
VPATH=src

all : ${EXEC}

clean : ; ${RM} ${EXEC}
