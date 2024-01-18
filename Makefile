# Define variables
CXX = c++
CFLAGS = --std=c++11
LINKER = -o
CXXSOURCE = cmdline.cpp Expr.cpp main.cpp Tests.cpp
HEADERS = cmdline.hpp Expr.hpp catch.h
OBJFILES = cmdline.o main.o Expr.o Tests.o

all: msdscript

# Generate object files for source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Link the object files to create the executable
msdscript: $(OBJFILES)
	$(CXX) $(CFLAGS) $(LINKER) $@ $^

# Defines a target for cleaning up the project
.PHONY: clean

# 'make clean' will remove the executable and the .o files
clean:
	rm -rf *.o
	rm -f msdscript
