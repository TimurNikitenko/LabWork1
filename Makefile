TARGET = start
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g 
SRC = main.cpp bmp.cpp
OBJ = $(SRC:.cpp=.o)

RAW_OUT = clockwise.raw counter_clockwise.raw gaussian_blurred.raw blurred_then_rotated.raw
PNG_OUT = $(RAW_OUT:.raw=.png)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(RAW_OUT) $(PNG_OUT)
