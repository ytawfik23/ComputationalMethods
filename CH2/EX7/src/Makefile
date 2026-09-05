CXXFLAGS = -I../include -std=c++20
OFILES = step.o getCoefficient.o

step: step.cpp getCoefficient.cpp
	g++ -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OFILES) step