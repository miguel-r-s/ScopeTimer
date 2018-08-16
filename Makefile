CC=g++
FLAGS=-Wall -std=c++11 -O3
TARGET=main.exe
MAIN=main.cpp

SOURCE=scope_timer.cpp
HEADER=scope_timer.hpp
OBJECT=scope_timer.o

main: $(MAIN) $(OBJECT) 
	$(CC) $(FLAGS) -o $(TARGET) $(MAIN) $(OBJECT)

scope_timer.o: $(HEADER) $(SOURCE)
	$(CC) $(FLAGS) -c $(SOURCE)

clean:
	rm $(TARGET) $(OBJECT)
