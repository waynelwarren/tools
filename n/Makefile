CXX = g++
LDFLAGS = 
SRC_DIR = .
INC_DIR = .
EXECUTABLE = n
SOURCES = $(shell find $(SRC_DIR) -name *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	cp $@ ~/bin

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

.PHONY: clean

clean:
	rm $(EXECUTABLE) $(OBJECTS)
