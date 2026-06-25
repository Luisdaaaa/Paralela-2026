TARGET = cenatMD

CXX = mpic++

CXXFLAGS = -O3 -fopenmp -Wall

SRCS = main.cpp particle_sim.cpp

OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean