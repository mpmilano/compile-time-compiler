
.PHONY : clean

CPPFLAGS=-g --std=c++1z
LDFLAGS= -pthread

SOURCES = main.cpp
CXX = clang++ -ferror-limit=1
HEADERS = allocated_ref.hpp  allocator.hpp  array.hpp  ast.hpp  mutils/cstring.hpp ctutils.hpp  ctutils-old.hpp  union.hpp parse.hpp Makefile mutils/cstring_tests.hpp
PHP_HEADERS = ast.php common.php util.php
OBJECTS=$(SOURCES:.cpp=.oo)

TARGET = main

all: $(TARGET)

%pp : %pp.php $(PHP_HEADERS)
	php $< | clang-format > $@

%.oo: %.cpp $(HEADERS)
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET); for foo in *pp.php; do echo $$foo | sed s/.php$$//g | xargs rm ; done


$(TARGET) : $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
