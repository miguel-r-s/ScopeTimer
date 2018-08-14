CC=g++
FLAGS=-Wall -std=c++11
TARGET=main.exe
SOURCE=main.cpp

main: scope_timer.hpp $(SOURCE) 
	$(CC) $(FLAGS) -o $(TARGET) $(SOURCE) 

clean:
	rm $(TARGET) 
