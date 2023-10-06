CXX 	= g++
CXXLAGS = -Wall
EXEC	=

main:
	${CXX} ${CXXLAGS} ./include/Graph.cpp main.cpp -o out.o
test:
	${CXX} ${CXXLAGS} ./include/Graph.h ./include/Graph.cpp test.cpp -o test.o

.PHONY: clean
clean:
	rm -rf out.o test.o