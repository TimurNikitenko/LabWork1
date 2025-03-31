PROJECT = start

# Компилятор
CXX = g++

# Флаги компиляции с указанием стандарта C++17
CXXFLAGS = -std=c++17 -Wall

# Исходные файлы
SRC = main.cpp bmp.cpp

# Объектные файлы
OBJ = $(SRC:.cpp=.o)

# Правило для создания исполнимого файла
$(PROJECT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(PROJECT) $(OBJ)

# Правила для компиляции исходных файлов в объектные файлы
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки промежуточных файлов
clean:
	rm -f $(OBJ) $(PROJECT)
