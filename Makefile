start: main.o bmp.o
        g++ main.o bmp.o -o start

main.o: main.cpp
        g++ -c main.cpp

bmp.o: bmp.cpp bmp.hpp
        g++ -c bmp.cpp

clean:
	rm *.o start






