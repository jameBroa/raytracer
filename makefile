CXX = g++

CXXFLAGS = -std=c++11

SRCS = $(shell find . -name "*.cpp")

TARGET = program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
