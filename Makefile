TARGET = start
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g 
SRC = main.cpp bmp.cpp
OBJ = $(SRC:.cpp=.o)

RAW_OUT = clockwise.raw counter_clockwise.raw gaussian_blurred.raw rotated_and_blurred.raw
PNG_OUT = $(RAW_OUT:.raw=.png)
XMP_OUT = $(RAW_OUT:.raw=.raw.xmp)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(RAW_OUT) $(PNG_OUT) $(XMP_OUT)
