TARGET := openbaba
CXX := g++
CXXFLAGS := -std=c++11 -g3 -Wall -Wextra -pedantic -I/usr/include/SDL2
LDLIBS := -lSDL2 -lSDL2_image

SRC := $(shell find . -name "*.cc")
OBJ  := $(patsubst %.cc, %.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

depend: .depend

.depend: $(SRC)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(OBJ)

dist-clean: clean
	rm -f *~ .depend

include .depend