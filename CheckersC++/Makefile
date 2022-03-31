all: CheckerCage.o CheckerBoard.o Play.o
	g++ -std=c++17 CheckerCage.o CheckerBoard.o Play.o -o play

CheckerCage.o:
	g++ -std=c++17 CheckerCage.cpp -c -o CheckerCage.o

CheckerBoard.o:
	g++ -std=c++17 CheckerBoard.cpp -c -o CheckerBoard.o

Play.o:
	g++ -std=c++17 Play.cpp -c -o Play.o

clean:
	rm -rf *.o

run: all
	./play
