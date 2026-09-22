all: tct nasa pdt

tct:
	g++ -Wall -O3 -g -std=c++17 TimeCodeTests.cpp TimeCode.cpp -o tct

nasa:
	g++ -Wall -O3 -g -std=c++17 NasaLaunchAnalysis.cpp TimeCode.cpp -o nasa

pdt:
	g++ -Wall -O3 -g -std=c++17 PaintDryTimer.cpp TimeCode.cpp -o pdt

clean:
	rm -f tct nasa pdt
