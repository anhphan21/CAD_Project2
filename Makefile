CXX 		= g++
CXXLAGS 	= -Wall
TARGET_EXEC	= 
BUILD_DIR 	= ./build
SRC_DIRS	= ./src
LIBS		= ./include

#SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
#OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
#${CXX} ${CXXLAGS} ${LIBS}/Graph.cpp ${LIBS}/Scheduling.cpp main.cpp -o out.o

main:
	${CXX} ${CXXLAGS} ${LIBS}/Scheduling.cpp ${LIBS}/Graph.cpp main.cpp -o out.o
test:
	${CXX} ${CXXLAGS} ./include/Graph.h ./include/Graph.cpp test.cpp -o test.o

.PHONY: clean
clean:
	rm -rf out.o test.o