CXXFLAGS = -fPIC

OFILES = parse.o add.o print.o

main: main.o libMain.so
	g++ -o $@ $< -L. -lMain -Wl,-rpath,.


libMain.so: $(OFILES)
	g++ -o $@ -shared $? 

clean:
	rm -f *.o *.so *~ main

