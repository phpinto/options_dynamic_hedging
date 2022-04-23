CC = g++
INCLUDE:= -Iinclude/
SOURCE:= src/main.cpp
EXE=bin/main
TEST_EXE=bin/test
SRC = $(wildcard src/*.cpp)
INC = $(wildcard include/*.h)

all: $(EXE)

$(EXE):
	$(CC) $(quantlib-config --cflags --libs)-w $(INCLUDE) -L/usr/local/Cellar/quantlib/1.19/lib -lQuantLib $(SOURCE) -o $(EXE)

run: 
	./$(EXE)

clean:
	rm $(EXE)
	rm $(TEST_EXE)

compile_test:
	g++ -w -Iinclude/ -L/usr/local/Cellar/quantlib/1.19/lib -lQuantLib src/unittest.cpp -o bin/test

run_test: 
	./$(TEST_EXE)

help:
		@echo "src: $(SRC)"
		@echo "include: $(INC)"
		