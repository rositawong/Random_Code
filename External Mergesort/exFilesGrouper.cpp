#include "FilesGrouper.h"
using namespace std; 

int main() {
	ifstream ifs("files.txt");
	// FilesGrouper fg; 				// default chunks, no duplicating files to merge
	FilesGrouper fg(2, true);		// chunks based on memory size, duplicate files to merge
	// FilesGrouper fg(2, false);	 		// chunks based on input, no duplicating files to merge

	// fg.addFilesToSort(cin, true); 	// user promped to enter/list files
	fg.addFilesToSort(ifs, false); 		// files to merge will be taken from file from ifs

	fg.mergeFiles(); 					// merge sorted files

	return 0; 
}