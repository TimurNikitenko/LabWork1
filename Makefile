CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = 

# Targets
SERIAL_TARGET = image_processor_serial
PARALLEL_TARGET = image_processor_parallel

# Sources
COMMON_SRC = main.cpp bmp.hpp
SERIAL_SRC = bmp_serial.cpp
PARALLEL_SRC = bmp_parallel.cpp

# Output files
OUTPUT_RAWS = clockwise.raw counter_clockwise.raw rotated_and_blurred.raw gaussian_blurred.raw
OUTPUT_PNGS = *.png

.PHONY: all serial parallel clean benchmark

all: serial parallel

serial: $(SERIAL_TARGET)

parallel: $(PARALLEL_TARGET)

$(SERIAL_TARGET): $(COMMON_SRC) $(SERIAL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(SERIAL_SRC) $(LDFLAGS)

$(PARALLEL_TARGET): $(COMMON_SRC) $(PARALLEL_SRC)
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ main.cpp $(PARALLEL_SRC) $(LDFLAGS) -fopenmp

benchmark: serial parallel
	@echo "\n=== SERIAL BENCHMARK ==="
	@time ./$(SERIAL_TARGET)
	@echo "\n=== PARALLEL BENCHMARK (4 threads) ==="
	@OMP_NUM_THREADS=4 time ./$(PARALLEL_TARGET)

clean:
	rm -f $(SERIAL_TARGET) $(PARALLEL_TARGET) $(OUTPUT_RAWS) $(OUTPUT_PNGS)