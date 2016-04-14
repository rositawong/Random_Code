#include "FilesGrouper.h"

FilesGrouper::FilesGrouper(): max_vector_size(20000000), subfile_count(0), current_file("current_file.txt"), new_file("new_file.txt"), final_output(""), listed_files("listed_files.txt"), duplicate_original_file(true) {}; 


FilesGrouper::FilesGrouper(int gb_mem, bool duplicate){ 
	if (gb_mem <= 0) {
		cout << "Size of memory must be greater than 0\n"; 
		exit(1); 
	}

	max_vector_size = (gb_mem * pow(10,9))/20; 
	subfile_count = 0; 
	current_file = "current_file.txt";
	new_file = "new_file.txt";
	final_output = ""; 
	listed_files = "listed_files.txt"; 
	duplicate_original_file = duplicate; 
}


void FilesGrouper::mergeSortedFiles(string fs1, string fs2, string output_file) {
	ofstream merged_file(output_file);  
 	string s1, s2; 
 	ifstream ifs1(fs1);
 	ifstream ifs2(fs2);

 	getline(ifs1, s1); 
 	getline(ifs2, s2); 

 	while (ifs1.good() && ifs2.good()) {
 		if (s1 < s2) {
 			merged_file << s1; 
 			getline(ifs1, s1); 
 		} else {
 			merged_file << s2; 
 			getline(ifs2, s2);
 		}
 		merged_file << "\n"; 
 	}

 	// finish off the remaining of each file and appending the previously read file name 

 	if (ifs1.good()) {
 		merged_file << s1 << "\n"; 
 		while (getline(ifs1, s1))
 			merged_file << s1 << "\n";
 	} else {
 		merged_file << s2 << "\n"; 
 		while (getline(ifs2, s2))
 			merged_file << s2 << "\n"; 
 	}
 	merged_file.close(); 
}


// Alternate to read between two files that contain list of files to merge
void FilesGrouper::switchFstreams(ifstream *ifs, ofstream *ofs) {
	ifs->close(); 
	ofs->close(); 

	swap(current_file, new_file);

	ifs->open(current_file); 
	ofs->open(new_file); 
}


// Sorts the chunk of names and clears the data structure to be reused
void FilesGrouper::sortChunk(vector<string> *names_in_file, string output_file) {
	sort(names_in_file->begin(), names_in_file->end()); 

	// then output the sorted results into a new file 
	ofstream ofs(output_file); 

	// iterates through sorted names to write them into file
	for (auto it = names_in_file->begin(); it != names_in_file->end(); ++it)
		ofs << *it << "\n"; 

	ofs.close(); 
}


bool FilesGrouper::parseValidFile(string file, ifstream &ifs) {
	cout << "------ Sorting file ------ : " << file << endl; 
	vector<string> names_in_file; 			// if no duplicates desired, use a set vs vector
	string name, sub_file = "sub_file_"; 

	while (getline(ifs, name)){
		if (name != "") {
			names_in_file.push_back(name); 
		}

		if (names_in_file.size() == max_vector_size) {
			// write this vector to output file 
			sortChunk(&names_in_file, (sub_file + to_string(subfile_count))); 

			// clear contents in vector and reuse it
			names_in_file.clear(); 
			cout << "Splitting file " << file << " into: " << (sub_file + to_string(subfile_count)) << "\n";
			subfile_count += 1; 
		}
	}


	// Sorts last chunk of data
	ifs.close(); 
	if (!names_in_file.empty()) {

		string filename = file; 
		if (duplicate_original_file) {
			filename += "_copy"; 
		}


		sortChunk(&names_in_file, filename); 
		return true; 
	} else {
		return false; 
	}
}


// Removes all the intermediate files generated for chunks
void FilesGrouper::deleteGeneratedFiles(int output_file_count) {
	// Delete sub_files
	for (int i = 0; i < subfile_count; ++i) {
		remove( ("sub_file_" + to_string(i)).c_str());
	}

	// Delete intermediate output files (not final file)
	for (int i = 0; i < (output_file_count - 2); ++i) 
		remove(("output_file_" + to_string(i)).c_str());

	// Delete copy files 
	if (duplicate_original_file) {
		ifstream ifs(listed_files); 
		string copiedFiles; 
		while (getline(ifs, copiedFiles)) 
			remove(copiedFiles.c_str()); 
		ifs.close(); 
	}

	// delete generated files: 
	remove(current_file.c_str()); 
	remove(new_file.c_str()); 
	remove(listed_files.c_str()); 

}


// Takes in a single file, and sorts it, then places it back into the file 
// Function works under assumption that one file can be fit into memory 
bool FilesGrouper::sortFile(string file) {
	ifstream ifs(file); 
	if (ifs.good()) {
		return parseValidFile(file, ifs); 
	} else {
		cout << "File \"" << file << "\" is an invalid file. It will be ignored.\n"; 
		return false; 
	}
}


/* Iterates through two files (current_file, new_file) that contains filenames to be merged
	current_file to iterate through it's list of file names 
	new_file to append newly created output files containing the merged data 
	Continues alternating both files until only one file name is read from both streams
*/
void FilesGrouper::mergeFiles() {
	ifstream fs_ptr(current_file); 
	ofstream backup(new_file); // this is the file you're writing the sorted information into

	int count = 0; 
	string file1, file2, output_file = "output_file_"; 
	bool end = false; 
	while (!end) {
		if (!getline(fs_ptr, file1)) {	// no more file to merge in this file
			switchFstreams(&fs_ptr, &backup); 
			if (!getline(fs_ptr, file1)) {		
				end = true; 
				final_output = (output_file + to_string(count)); 
				cout << "No output file generated\n";
				break; 
			} 

			// If file2 fails to read, then we know file1 is out output 
			if (getline(fs_ptr, file2)) {
				cout << "Merging: " << file1 << ", " << file2 << " into " << (output_file + to_string(count)) << "\n";
				mergeSortedFiles(file1, file2, (output_file + to_string(count))); 	
				backup << (output_file + to_string(count)) << "\n"; 
			} else {
				end = true; 
				final_output = file1; 
			}
		} else {
			if (getline(fs_ptr, file2)) {			// both files to merge listed in current file 
				mergeSortedFiles(file1, file2, (output_file + to_string(count))); 
				backup << (output_file + to_string(count)) << "\n"; 
			} else {
				switchFstreams(&fs_ptr, &backup); 
				if (getline(fs_ptr, file2)) {		// second file to merge in next file 
					mergeSortedFiles(file1, file2, (output_file + to_string(count))); 
					backup << (output_file + to_string(count)) << "\n"; 
				} else {							// first extract file is final merged file 
					end = true; 
					// write everything into this output file?? 
					final_output = (output_file + to_string(count)); 
					// Since we could only read one file to be merged, this file is the merged output
					mergeSortedFiles(file1, "", final_output); 
				}
			}
			cout << "Merging: " << file1 << ", " << file2 << " into " << (output_file + to_string(count)) << "\n";
		}

		count += 1; 
	}
	cout << "You can now view final output file, \"" << final_output << "\"\n";
	// Upon completely merging all files, delete intermediately generated 
	deleteGeneratedFiles(count); 
}


void FilesGrouper::addFilesToSort(istream &ifs, bool l_entered) {
	if (l_entered)
		cout << "Enter all files to be merged into one file, when finished, enter 'q':\n"; 

	string filename; 
	ofstream ofs(new_file); 
	ofstream ofsEnteredFiles(listed_files); 
	while (getline(ifs, filename)) {
		if (l_entered && filename == "q")
			break; 

		if (filename != "") {
			ofs << filename << "\n"; 

			ofsEnteredFiles << (filename + "_copy") << "\n"; 
		}
	}
	ofs.close(); 
	ofsEnteredFiles.close(); 

	// Iterate through the file and parse
	ifstream ifs2(new_file); 
	ofs.open(current_file);		// purpose is not to overwrite main file containing list of files to merge
	while (getline(ifs2, filename)) {
		if (sortFile(filename)) {
			ofs << filename; 	

			if (duplicate_original_file) 
				ofs << "_copy"; 
			ofs << "\n"; 
		}
	}

	// Update all the subfiles generated presorted files thus no need to sort file 
	for (int i = 0; i < subfile_count; ++i) {
		ofs << ("sub_file_" + to_string(i)) << "\n"; 	
	}

	ofs.close(); 
}


