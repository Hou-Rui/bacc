COMPILER = g++
OPTIONS = -std=c++11 -O2 -Wall
TARGET = bacc
SOURCES = $(wildcard src/*.cpp)

$(TARGET): $(SOURCES)
	$(COMPILER) $(OPTIONS) -o $(TARGET) $(SOURCES)

.PHONY: run
run:
	$(TARGET)
