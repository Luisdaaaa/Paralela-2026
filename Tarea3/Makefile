
TARGET = cenatMD

CXX = mpic++


CXXFLAGS = -O3 -fopenmp -Wall


SRCS = main.cpp particle_sim.cpp


OBJS = $(SRCS:.cpp=.o)

# Regla principal
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

to
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean