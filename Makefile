filename=main

all: compile link run

compile:
	g++ -c $(filename).cpp -I"C:\Users\Daniil\Documents\libraries\SFML-2.5.1\include"

link:
	g++ $(filename).o -o $(filename) -L"C:\Users\Daniil\Documents\libraries\SFML-2.5.1\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

run:
	.\$(filename).exe

clean:
	rm -rf *.o $(filename)