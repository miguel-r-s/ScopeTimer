CC=g++
FLAGS=-Wall -std=c++11
TARGET=main.exe
SOURCE=main.cpp

main: raii_timer.hpp $(SOURCE) 
	$(CC) $(FLAGS) -o $(TARGET) $(SOURCE) 

clean:
	rm $(TARGET) 
