CXX 		= g++
CXXLAGS 	= -Wall
SRC_DIRS	= ./src
LIBS		= ./include
OUT_DIRS	= ./output
BENCHMARK_DIRS = ./aoi_benchmark

.PHONY: main
main:
	${CXX} ${CXXLAGS} ${LIBS}/Scheduling.cpp ${LIBS}/Graph.cpp ${SRC_DIRS}/m11207805.cpp -o mlrcs


.PHONY: clean
clean:
	rm -rf mlrcs
	find ${OUT_DIRS} -type f -name "*.lp" -delete
	find ${OUT_DIRS} -type f -name "*.sol"  -delete
	find . -type f -name "*.log"  -delete