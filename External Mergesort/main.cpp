/*
Rosita Wong 
January 2016 

Given: file containing a list of files OR entering files in command line 
Merge and sort the files into one final output
*/

#include "FilesGrouper.h"
#include "getopt.h"
using namespace std; 


int main(int argc, char *argv[]) {
	// Parses command line arguments to ensure properly added 
	int opt = 0;
	bool f_entered = false, l_entered = false; 
	string filename; 
	while ((opt = getopt(argc, argv, "f:l")) != -1) {
		switch(opt) {
			case 'f': 
				if ((filename = (string) optarg) == "" || l_entered) {
					cout << "File entered: " << filename << endl;
					cerr << "Can't input empty filename or input both this filename and list file!\n"; 
					exit(1); 
				}
				f_entered = true; 
				break; 
			case 'l': 
				if (f_entered) {
					cout << "Both list and file were given or list of files entered is empty\n"; 
					exit(1); 
				}
				l_entered = true; 
				break;
			default: 
				cout << "Enter:\n-f (file containing all filenames) or \n-l to indicate you'll manually enter filenames\n";
				return 0; 
		}
	}
	if ((!f_entered && !l_entered)) {
		cerr << "Either list out files or entere a file that contains all the files you want\n"; 
		exit(1); 
	}


	// The code to perform merge/sort names
	FilesGrouper fg;

	// Open file user enters and begins extracting filenames
	// Sorts files in passed in file or via cin
	if (f_entered){
		ifstream fs(filename);
		if (!fs.good()) {
			cerr << "File name isn't a file or in this directory\n"; 
			exit(1);
		}

		// need to check that the input file isn't empty
		fg.addFilesToSort(fs, l_entered); 
		fs.close(); 
	} else {
		// Takes user input, and places that into a file consisting of files to merge
		fg.addFilesToSort(cin, l_entered); 
	}


	// Merges all files
	fg.mergeFiles();


	return 0; 
}