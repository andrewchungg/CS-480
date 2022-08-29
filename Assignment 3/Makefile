#Author: Andrew Chung
#RedID: 821542300

# CXX Make Variable for compiler
CXX=g++
# Make variable for compiler options
CXXFLAGS=-std=c++11 -g

pagingwithtlb: main.o output_mode_helpers.o pagetable.o tracereader.o
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^

main.o: main.cpp

output_mode_helpers.o: output_mode_helpers.h output_mode_helpers.cpp

pagetable.o: pagetable.h pagetable.cpp

tracereader.o: tracereader.h tracereader.cpp

clean:
	rm *.0