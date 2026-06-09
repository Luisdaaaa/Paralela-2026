# Makefile para compilar el ejercicio MPI (main.cpp)
CXX = mpic++
CXXFLAGS = -O2 -std=c++11 -Wall
SRCS = main.cpp
TARGET = programa
NP ?= 4

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	mpirun -np $(NP) ./$(TARGET)

clean:
	rm -f $(TARGET)
