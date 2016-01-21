#ifndef __FILESGROUPER_H_
#define __FILESGROUPER_H_

#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <stdio.h> 
#include <vector> 
#include <string> 
#include <cmath> 
#include <cstdio>
using namespace std; 

class FilesGrouper {
	unsigned int max_vector_size;	// Arbitrary size (200000000), assuming 4GB RAM and max assume 20 bytes/name 
									// Alllows sortable chunks that fit into memory 
	int subfile_count;  			// Used to generate unique names for subfiles

	string current_file; 			// Arbitrary file used to track merging process
	string new_file; 				// Arbitrary file used to track merging process
	// Opens and sorts the contents of file 
	bool sortFile(string file); 

	// Sorts a chunk (names_in_file) of the file, and places results into output_file
	void sortChunk(vector<string> *names_in_file, string output_file); 

	// Takes an opened ifstream and puts that tile into chunks that are sortable in memory
	bool parseValidFile(string file, ifstream &ifs); 

	// Iterates through two sorted files to merge both files' values output_file
	void mergeSortedFiles(string fs1s, string fs2s, string output_file); 

	// Switches between current_file and new_file to read files that will be merged
	void switchFstreams(ifstream *ifs, ofstream *ofs); 

	// Deletes intermediate files generated 
	void deleteGeneratedFiles(int output_file_count); 
public: 
	string final_output; 	// file containing all merged files' content 
	string listed_files; 	// captures all the entered files from -l 
	bool duplicate_original_file; 	// default = true, false if wish to overwrite the files to be passed in 
									// Recommend to set false if files are not used after they're merged

	// Default constructor
	FilesGrouper(); 

	// User specified total memory they wish to use in GB, and 
	// To duplicate files (default = true) to be merged (not recommended if large files)
	FilesGrouper(int gb_mem, bool duplicate); 

	// User call to extract the names of files to be merged into class structure 
	// Only sort valid files
	void addFilesToSort(istream &ifs, bool l_entered); 

	// Merge all files that are sorted
	void mergeFiles(); 
}; 


#endif /* __FILESGROUPER_H_ */