# 1. Variables
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3 -fopenmp

# 2. Archivos fuente y objeto
# Listamos los tres archivos .cpp que tienes en Tarea2
SRCS = main.cpp menu.cpp programa.cpp
# Genera los nombres .o automáticamente (main.o, menu.o, programa.o)
OBJS = $(SRCS:%.cpp=%.o)

# Nombre del ejecutable final
TARGET = mi_programa

# 3. Reglas de compilación
all: $(TARGET)

# Une los archivos .o para crear el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compila cada archivo .cpp por separado para generar su .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 4. Limpieza
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean