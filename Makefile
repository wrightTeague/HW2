all:
	g++ -Wall -O3 -g -std=c++17 TimeCodeTests.cpp TimeCode.cpp  -o tct

clean:
	rm -f tct