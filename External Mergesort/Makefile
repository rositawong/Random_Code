CXX = g++
CXXFLAGS = -Wall -g -std=c++11

MAIN_FILE_OBJ = main.o

MAIN_FILE = main

default: $(MAIN_FILE) 

$(MAIN_FILE): $(MAIN_FILE_OBJ)
	$(CXX) $(CXXFLAGS) $(MAIN_FILE_OBJ) FilesGrouper.cpp -o  $(MAIN_FILE)

# removes the last output file and executable 
clean:
	rm output_file_* *.o 
