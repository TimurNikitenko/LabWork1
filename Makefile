TARGET = start
CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRC = main.cpp bmp.cpp
OBJ = $(SRC:.cpp=.o)

# for cleaning output files 
RAW_OUT = cwrotated.raw ccwrotated.raw gaussian_blurred.raw
PNG_OUT = $(RAW_OUT:.raw=.png)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(RAW_OUT) $(PNG_OUT)

.PHONY: clean
