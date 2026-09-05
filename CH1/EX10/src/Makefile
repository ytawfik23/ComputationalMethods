OFILES=parseInput.o getCanvas.o plotHermite.o
CXXFLAGS = -I../include -std=c++20

hermite:hermite.o libHermite.a
	c++ -o $@ $< -L. -lHermite

libHermite.a:$(OFILES)
	ar rc $@ $?
clean:
	rm -f *.a *.o *~ hermite
