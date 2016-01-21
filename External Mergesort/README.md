##External Mergesort
###Problem: 
Given a file containing a list of files/command line prompt to enter a list of files that contain names, merge and sort the list of names 

###To run: 
	$ make 
	$ ./main -f (file with list of filenames to merge) OR ./main -l

Test to illustrate the merge and sort if you execute the following: 
	$ ./main -f files.txt

###Essential files: 
main.cpp - Program that extracts command line arguments to merge/sort files
FilesGrouper.cpp/h - FilesGrouper library, algorithm to merge/sort files
exFilesGrouper.cpp - How to use FilesGrouper as a library

###Main functions of merging and sorting:
	sortFile()
	parseValidFile()
	sortChunk()
	margeFiles()
	mergeSortedFiles()

cout statements included to better follow the program. Your output file will be output_file_*, dependent on number of merges completed


###Notes: 
f1.txt is a large file which I didn't show in the tests cases for simplicity purposes. If you'd like to see what happens with a large file, include "f1.txt" in files.txt

###Design: 
- Reason for using fstreams vs data structures is to limit memory use to hold this information (i.e. filenames of files to be merged). 
- Thus, we can allocate more data to sort larger chunks of data

###Assumptions: 
1. Each line of a file contains one name
2. Each name is a maximum of 20 characters
3. For the file with a list of files to merge, each line is one file name
4. User can pass file names in command line arguments or pass a file containing the list of files to merge


###Algorithm: 
1. Sort all the files. 

	parseValidFile()
	- Breaks files into sortable chunks (sub_file_*) if whole file is too large to sort
	- Writes all sub_file_* into list of files to be sorted 

2. Merge sorted files and into one file 

	mergeFiles(): 
	- Reads list of files from two files (current_file and new_file) until only one filename remains in both files 
	- Calls mergeSortedFiles(file1, file2, output_file) on two files read in to generate an output_file (sorted and merged f1, f2)

3. Deletes intermediate output_file_*, and sub_file_*, etc generated
	
Two files will hold list of files to merge



###About FilesGrouper: 
FilesGrouper can be used as a library. The following order of function calls are needed to merge and sort files: 
addFilesToSort()
mergeFiles()


Rosita Wong
January 2016
