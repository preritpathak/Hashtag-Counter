#ifndef HASHTAGCOUNTER_H
#define HASHTAGCOUNTER_H

#include <iostream>
#include <string>

using namespace std;

//This stucture holds the node structure for the Fibonacci heap

struct NodeStructure {
  
  int degree=0;			// Number of children in the node
  string hHashTag;		// Holds the hash tag
  NodeStructure *left;		// Left Sibling element 
  NodeStructure *right;		// Right Sibling element
  NodeStructure *parent=NULL;	// Parent in the fibonacci heap (if any)
  NodeStructure *child=NULL;	// Child node (if any)
  bool childCut = false;	// Whether a node has lost a child or not
  int element;			// Element stored in the node
  
};


class hashtagcounter {
  
    private:
	//Max pointer which points to the max node in the heap
	NodeStructure *hMax;
     
    public:
	hashtagcounter();	// Constructor	
	NodeStructure* InitializeHeap(int element, string hHashTag);	
	void InsertNode(NodeStructure *newNode);		
	void IncreaseKey(NodeStructure *node, int newVal);	
	NodeStructure *RemoveMax();				
	void addChildrenToRoot(NodeStructure *removedChild); 	
	void Meld(NodeStructure *pairNode1);	
	NodeStructure *CombineThePairs(NodeStructure *pairNode1, NodeStructure *pairNode2);	
    NodeStructure *InsertIntoRootList(NodeStructure *oldNode, NodeStructure *newNode);
	void NodeCut(NodeStructure *childNode, NodeStructure *parentNode);
	void NodeCascade(NodeStructure *parentNode);
    
};

#endif