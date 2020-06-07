CXX = g++
LDFLAGS= -lncurses
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

tetris: $(obj)
	$(CXX) -g -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) tetris