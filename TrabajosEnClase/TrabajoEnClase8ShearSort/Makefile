CXX = g++
CXXFLAGS = -fopenmp -Wall -std=c++11

TARGET = programa
OBJS = main.o matriz.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp matriz.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

matriz.o: matriz.cpp matriz.hpp
	$(CXX) $(CXXFLAGS) -c matriz.cpp

clean:
	rm -f $(OBJS) $(TARGET)