CXX 		= g++
CXXLAGS 	= -Wall
TARGET_EXEC	= 
BUILD_DIR 	= ./build
SRC_DIRS	= ./src
LIBS		= ./include
BENCHMARK_DIRS = ./aoi_benchmark

#SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
#OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
#${CXX} ${CXXLAGS} ${LIBS}/Graph.cpp ${LIBS}/Scheduling.cpp main.cpp -o out.o
#	./mlrcs -h ${BENCHMARK_DIRS}/sample02.blif 2 1 1

.PHONY: main
main:
	${CXX} ${CXXLAGS} ${LIBS}/Scheduling.cpp ${LIBS}/Graph.cpp ${SRC_DIRS}/m11207805.cpp -o mlrcs


.PHONY: clean
clean:
	rm -rf mlrcs
	find . -type f -name "*.lp" -delete
	find . -type f -name "*.sol"  -delete
	find . -type f -name "*.log"  -delete