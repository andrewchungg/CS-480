CXX=g++
# Make variable for compiler options
#    -std=c++11  C/C++ variant to use, e.g. C++ 2011
#    -g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

LDFLAGS=-pthread
maintest: main.o populatetree.o countwords.o 
    $(CXX) $(CXXFLAGS) -o maintest $^ $(LDFLAGS)

main.o: main.cpp EXEC_STATUS.h

populatetree.o: populatetree.h populatetree.cpp 

countwords.o: populatetree.h countwords.h countwords.cpp

clean:
    rm *.0