
.PHONY : clean

CPPFLAGS=-g --std=c++1z
LDFLAGS= -pthread

SOURCES = main.cpp

HEADERS = allocated_ref.hpp  allocator.hpp  array.hpp  ast.hpp  ctutils.hpp  ctutils-old.hpp  union.hpp parse.hpp Makefile
OBJECTS=$(SOURCES:.cpp=.oo)

TARGET = main

all: $(TARGET)

%pp : %pp.php 
	php $< | clang-format > $@

%.oo: %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

CXX = clang++ -ferror-limit=1

$(TARGET) : $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
