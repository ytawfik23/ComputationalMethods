OFILES = parse.o add.o print.o

main: main.o libMain.a
	g++ -o $@ $< -L. -lMain 

libMain.a: $(OFILES)
	ar rc $@ $?

clean:
	rm -f *.o *.a *~ main

