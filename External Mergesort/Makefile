CXX = g++
CXXFLAGS = -Wall -g -std=c++11

MAIN_FILE_CPP = main.cpp
MAIN_FILE_OBJ = main.o
MAIN_FILE = main

FILESGROUPER_CPP = FilesGrouper.cpp
FILESGROUPER_OBJ = FilesGrouper.o
FILESGROUPER_H = FilesGrouper.h

default: $(MAIN_FILE)

$(MAIN_FILE): $(MAIN_FILE_OBJ) $(FILESGROUPER_OBJ) $(FILESGROUPER_H)
	$(CXX) $(CXXFLAGS) $(FILESGROUPER_CPP) -o $(MAIN_FILE) $(MAIN_FILE_CPP)

# removes the last output file and executable 
clean:
	rm output_file_* *.o 
