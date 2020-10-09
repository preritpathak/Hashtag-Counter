#include <iostream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <cmath>
#include <fstream>
#include "hashtagcounter.h"

#define hc hashtagcounter

using namespace std;

// We combine only those nodes who have the same degree, hence we maintain a map to keep track of these degrees
unordered_map<int, NodeStructure*> degreeMap;

hc::hashtagcounter() {
	hMax = NULL;		// Constructor to set the max pointer to NULL
}

NodeStructure* hc::InitializeHeap(int elem, string hHashTag) {
  
  NodeStructure *node = new NodeStructure;
  node->right = node;											// setting the initial right sibling to itself
  node->left = node; 											// setting the initial left sibling to itself
  node->hHashTag = hHashTag;
  node->element = elem;
 
  return node;  

}

// Insertion function - to insert a new node in the fibonacci heap
void hc::InsertNode(NodeStructure *newNode) {

	if(hMax == NULL) {  // if the heap is currently empty
	 
	    hMax = newNode;
	    hMax->parent = NULL;
	    hMax->left = hMax;
	    hMax->right = hMax;
	    hMax->childCut = false;		
	}
	else {
	   // if the heap already exists
	    hMax = InsertIntoRootList(hMax, newNode);		
	}

	NodeStructure *maxNodeTemp = hMax->left;                           // Making sure that the max pointer is always 
    	NodeStructure *currNode = hMax;									// pointing to the current max node
	do {
		if(currNode->right->element > hMax->element) {
					
			hMax = currNode->right;
			
		}
		currNode= currNode->right;
	}while(currNode != maxNodeTemp);

	return;
}

// IncreaseKey() : Increase the value of a node in the heap 
void hc::IncreaseKey(NodeStructure *node, int newValue) {

	node->element = node->element + newValue;		// Change the value at the node to the new larger value	
	NodeStructure *cacheParent = node->parent;		// The Parent of the node is stored
  			
	if(cacheParent != NULL && node->element > cacheParent->element) {
	    NodeCut(node, cacheParent);						// If the child node's frequency value is greater than its parent,
	    NodeCascade(cacheParent);					    // Perform Cut() & CascadeCut() : Cut the node from its parent node 
														//and cascade into root node
	}
	
	if(node->element > hMax->element)
		hMax = node;				// New node with larger count value becomes hMax		 

}


/** RemoveMax() : Remove the max node from the heap **/
NodeStructure* hc::RemoveMax() {

	NodeStructure *cacheMaxNode = hMax;			// Need to return this to calling function  

	if(hMax->right == hMax) {			// For A single node at root
	
		hMax = NULL;				// Set the max pointer to null & the heap becomes empty
		addChildrenToRoot(cacheMaxNode);		// Add the children (if any) of max node to root list
	
	}
	else {						// For more than one node at root

		// Max Nodes Siblings must be joined
		hMax->right->left = hMax->left;
		hMax->left->right = hMax->right;	

		NodeStructure *rightChild = cacheMaxNode->right;
		
		hMax = NULL;				// Set the max pointer to null, may be not required (CHECK later)
		hMax = rightChild;			
		
		addChildrenToRoot(cacheMaxNode);		// Insert the children of Max node into the root of the heap

		NodeStructure *currPointer = hMax;

		do { 	                                                     // Looping over the heap to find the new max node

			if(currPointer->right->element > hMax->element) {
				
				hMax = currPointer->right;
			}
			
			currPointer = currPointer->right;
		}
		while(currPointer != rightChild);
	
	}

	// This method will meld nodes with same degree until no two nodes in the root have same degree 
	Meld(hMax);
	
	degreeMap.clear();	
	cacheMaxNode->left = cacheMaxNode;
	cacheMaxNode->right = cacheMaxNode;
	cacheMaxNode->parent = NULL;
	cacheMaxNode->child = NULL;
	cacheMaxNode->degree = 0;

	// Return the max node to main function to write to the output file
	return cacheMaxNode;
}


/** This method adds the children of a removed node to root node list of the heap **/
void hc::addChildrenToRoot(NodeStructure *removedChild) {

	NodeStructure *tempNode = removedChild->child;
	
	if(removedChild->degree == 0) 		// Max node has no children		
		return;	
		
	else if(removedChild->degree == 1) 	// Max node has only one child		
		InsertNode(tempNode);		// Insert the only child to root of heap

	else {					// Max node has more than one child

		for(int i=0; i<removedChild->degree; i++) {

			NodeStructure *currChild = removedChild->child;		
			removedChild->child = currChild->right;
			currChild->right = currChild;
			currChild->left = currChild;
			InsertNode(currChild);					

		}			
	}

	removedChild->child = NULL;
	return;
}


// Meld two nodes which have the same degree
void hc::Meld(NodeStructure *pairNode1) {
					
	do {
		int degree = pairNode1->degree;		// Degree of the node
		
		if(degreeMap.find(degree) != degreeMap.end()) { // Same degree node is present in hash map
			
			if(pairNode1 != degreeMap[degree]) {

				NodeStructure *pairNode2 = degreeMap[degree];

				// Remove the entry of this degree from the degree map 
				degreeMap.erase(degree);				
					
				// Returns the parent node (Node with the larger frequency value)
				NodeStructure *parentNode = CombineThePairs(pairNode1, pairNode2);
				pairNode1 = parentNode;		
					
				Meld(pairNode1);
				return;
				
			}
						
		} 
		else
			degreeMap[degree] = pairNode1;		// to update the values of node and degree in the degree table	

		pairNode1 = pairNode1->right;
	}
	while(pairNode1 != hMax);
	return;
}


//Combines two node and makes one the parent of the other
NodeStructure* hc::CombineThePairs(NodeStructure *pairNode1, NodeStructure *pairNode2) {

	NodeStructure *parentNode, *childNode;
	
	// This is to handle similar count hashTags, 
	// need to make sure, the max node always stays at the root
	if(pairNode1 == hMax || pairNode2 == hMax) {

		if(pairNode1 == hMax) {
			
			pairNode2->left->right = pairNode2->right;
			pairNode2->right->left = pairNode2->left;								

			parentNode = pairNode1;
			childNode = pairNode2;			
		}
		else {
			
			pairNode1->left->right = pairNode1->right;
                        pairNode1->right->left = pairNode1->left;                                               

                        parentNode = pairNode2;
                        childNode = pairNode1;
        }
	}
	else if(pairNode1->element > pairNode2->element) {			// to remove pairNode2 from the root list

	        pairNode2->left->right = pairNode2->right;
        	pairNode2->right->left = pairNode2->left;
		
			parentNode = pairNode1;

	        childNode = pairNode2;
		
	}
	else {								// to remove pairNode1 from the root list
			
        	pairNode1->left->right = pairNode1->right;
		pairNode1->right->left = pairNode1->left;
        
	        parentNode = pairNode2;
        	childNode = pairNode1;		

	}
	
	if(parentNode->degree == 0) {					// If the parent has no previous children, Insert new child

		parentNode->child = childNode;
		childNode->parent = parentNode;

		childNode->right = childNode;
                childNode->left = childNode;
			
	}
	else {

		childNode->right = childNode;
                childNode->left = childNode;				
			
		NodeStructure *defaultChild = parentNode->child;		// Accessing the already present child node
		NodeStructure *valNext = defaultChild->right;		// Store this since we are going to overwrite it.
		defaultChild->right = childNode;
		defaultChild->right->left = defaultChild;
		childNode->right = valNext;
		childNode->right->left = childNode;
		//parentNode->child = childNode;		// May be not necessary!!! remove and see if it is OKAY
		childNode->parent = parentNode;
		
	}
	
	parentNode->degree++;				// Increase the degree of parent node as we just inserted a new child

	return parentNode;
}


/** Merge new node with the List of nodes already present in the heap **/
NodeStructure* hc::InsertIntoRootList(NodeStructure *oldNode, NodeStructure *newNode) {

	if(oldNode == NULL && newNode == NULL) {		// New Node & Old Node are NULL => Result is NULL
		return NULL;
	}
	
	else if(oldNode != NULL && newNode == NULL) {		// New Node is NULL and Old Node isn't NULL => Result is Old Node.
		return oldNode;
	}
	
	else if(oldNode == NULL && newNode != NULL) {		// Old Node is NULL and New Node isn't NULL => Result is New Node.
		newNode->parent = NULL;
		newNode->childCut = false;
		if(newNode->degree == 0)
			newNode->child = NULL;

		return newNode;
	}
	else {
		NodeStructure *valNext = oldNode->right;		// we store this since we are going to insert new node in between
		oldNode->right = newNode->right;
		oldNode->right->left = oldNode;
		newNode->right = valNext;
		newNode->right->left = newNode;
		newNode->parent = NULL;
		newNode->childCut = false;
		if(newNode->degree == 0) 
			newNode->child = NULL;

		/* Return a pointer to the Larger node */
		return oldNode->element >/*=*/ newNode->element? oldNode : newNode;
	}
}


/** Separates the Child node from its parent node, as the key of Child is larger than its Parent **/
void hc::NodeCut(NodeStructure *childNode, NodeStructure *parentNode) {

	childNode->parent = NULL;										// Set the parent of the node to removed to NULL

	// When the childNode has siblings, set right sibling as new child of parent
	if(parentNode->child == childNode && childNode->right != childNode) {

		parentNode->child = childNode->right;
		childNode->left->right = childNode->right;
		childNode->right->left = childNode->left;

	}
	else if(parentNode->child != childNode && childNode->right != childNode) {

		childNode->left->right = childNode->right;
                childNode->right->left = childNode->left;

	}
	else { 

		parentNode->child = NULL;
	}


	childNode->right = childNode->left = childNode;
	childNode->childCut = false;		// ChildCut is set to false 
	parentNode->degree-- ;			// Decrease the degree of the parent node
	InsertNode(childNode);			// Remove the node and insert into the root list	

	return;
	
}
 

/** Inserts the Separated Child into the root node List **/
void hc::NodeCascade(NodeStructure *parentNode) {

	NodeStructure *grandParentNode = parentNode->parent;	// Store the parent of parentNode
	
	if(grandParentNode != NULL) {

		if(parentNode->childCut == false) { 		// If the node has not lost a child before

			parentNode->childCut = true;		// Set its childCut to true

		}
		else {
			
			// Cut() & CascadeCut() : Cut the node from its parent node and cascade into root node
			NodeCut(parentNode, grandParentNode);
			NodeCascade(grandParentNode);			

		}
			
	}

	return;
}