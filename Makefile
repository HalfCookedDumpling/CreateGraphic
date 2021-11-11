CreateGraphicTest: Main.o Keyboard.o Mouse.o Shape.o IconResource.o
	g++ -o CreateGraphicTest -mwindows -O2 Main.o Keyboard.o Mouse.o Shape.o IconResource.o

Main.o: Main.cpp Main.h
	g++ -c Main.cpp -D UNICODE -O2

Keyboard.o: Keyboard.cpp Keyboard.h
	g++ -c Keyboard.cpp -O2

Mouse.o: Mouse.cpp Mouse.h
	g++ -c Mouse.cpp -O2

Shape.o: Shape.cpp Shape.h
	g++ -c Shape.cpp -O2

IconResource.o: IconResource.rc IconResource.h
	windres -o IconResource.o IconResource.rc
