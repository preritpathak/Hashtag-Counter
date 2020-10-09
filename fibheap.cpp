 /* Student : Prerit Pathak
 * UFID   : 8805 7930
 */

#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <cmath>
#include <fstream>
#include "hashtagcounter.h"

using namespace std;
int number_of_nodes = 0; // counter variable to count the number of the nodes in the heap

// Maintaining a hash table (map in cpp) to store the hashtag as the key and the node ptr of its frequency as key
unordered_map<string, NodeStructure*> hashTable;

int main(int argc, char *argv[]) {   // argc = number of arguments, argv = array containing the arguments

if(argc < 2) {

cout << "Specify the input file containing hashtags" << endl;
cout << "Usage : ./hashtagcounter <filename>" << endl;
return 0;

}


// The Input File containing hash tags and their frequencies
ifstream inputFile;
inputFile.open(argv[1]); 

if(!inputFile.is_open()) { 
	cout << "Check whether the inpue file exists or not" << endl;
	return 0;
}

//The Output File containing top most n hash tags

ofstream outputFile;

if(argc == 3){
	outputFile.open(argv[2]);    // open the output file only if it's provided
}

hashtagcounter heap;  // Initialize hashtagcounter class 

string readHashTag;
int frequency;

while(getline(inputFile, readHashTag)){  // iterating over the entire input file
	char firstChar = readHashTag.at(0);       
	size_t hash = readHashTag.find("#");  // if the line contains a '#', this implies that is has a hashtag
	if(hash != string::npos){
	string::size_type pos = readHashTag.find(' ');
	if(readHashTag.npos != pos){
		frequency = stoi(readHashTag.substr(pos + 1));     
		readHashTag = readHashTag.substr(0, pos);
	
		if (hashTable.find(readHashTag) == hashTable.end()) {     // If the hashtag is not present in the heap, we add it to the heap
			
			NodeStructure *node;
			node = heap.InitializeHeap(frequency, readHashTag);
			hashTable[readHashTag] = node;
			heap.InsertNode(node);
			number_of_nodes += 1;
		
		}
		else {									// If the hashtag already exists, we perform an increase key operation	
				heap.IncreaseKey(hashTable[readHashTag],frequency);					
		}   
	}
	}
	else if (firstChar == 's' || firstChar == 'S') {			// Stop query encountered 
		inputFile.close();
		if(argc ==3 ){
			outputFile.close();   
		}
		exit(0);  									// exit the program with code 0
		}

   else {							// These are removeMax queries in the form of a single integer

	int removeMaxQuery = stoi(readHashTag);	   
		
	NodeStructure *node[removeMaxQuery];
	string key[removeMaxQuery];
	int value[removeMaxQuery];
if (removeMaxQuery <= number_of_nodes){   // if the query is acceptable
// Remove Max and Pairwise Combine operations
if(argc == 3){
	for (int i=0; i<removeMaxQuery; i++) {

	    node[i] = heap.RemoveMax();
	    key[i] = node[i]->hHashTag;		
            value[i] = node[i]->element;

	    // Write to output file
	    outputFile << key[i];
	    if(i < removeMaxQuery-1)
	        outputFile << ",";
		
	}
}

else{
int i = 0;
while(i != removeMaxQuery){

	    node[i] = heap.RemoveMax();
	    key[i] = node[i]->hHashTag;		
            value[i] = node[i]->element;

	    // Write to console file
	    cout << key[i];
	    if(i < removeMaxQuery-1)
	        cout << ",";
		i++;
	}

}
	// Insert the above removed nodes back into the root of Fibonacci heap
	int j =0;
	while(j != removeMaxQuery){
	    node[j] = heap.InitializeHeap(value[j], key[j]);
	    hashTable[key[j]] = node[j]; 
	    heap.InsertNode(node[j]);
		number_of_nodes += 1;
		j++;
  	}
	if(argc == 3){

	outputFile << endl;   
	}
}
else{    // if the range query contains a value which is greater than the nodes in the heap
	cout << "ERROR : The input line contains a query which is greater than the total number input hashtags.";
}
   }  
      
  }

  inputFile.close();  // closing the input file

if(argc == 3){
  outputFile.close();   // if output file was opened, close it
}
  return 0;
}