main:
	g++ ./include/Graph.h main.cpp -o out.o
test:
	g++ test.cpp -o test.o
clean:
	rm -rf out.o test.o