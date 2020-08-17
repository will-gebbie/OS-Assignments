CXX=g++
CXXFLAGS=-std=c++11 -g
LIBS= -pthread

CC=$(CXX)

mizzo: main.o consumer.o producer.o belt.o
	$(CC) -o mizzo main.o consumer.o producer.o belt.o $(LIBS)
