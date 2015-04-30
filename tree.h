#ifndef TREE_H
#define TREE_H

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include "param.h"

using namespace std;

void data_load();	// Loading data

class tree{
	private:
		struct node{   
			char *symbol;
			double value;
			int number;
			int layer;
			struct node *left;
			struct node *right;
		};
		node *root_node;
		int maxDep_tree(node *);
		node *create_tree(int, node *, bool);	// Create a tree according to layer and type
		void destroy_tree(node *);
		void print_tree(node *, int);
		bool cmp_tree(node *, node *);
		void input_tree(node *, char *);
		double output_tree(node *);
		int totlenode(node *, int);
		void copy_tree(node *, node *);
		void crossover_tree(node *, node *);
		bool paste(node *, node *, int);
		node *search(node *, int);
		int numbering(node *, int);
		int callayer(node *, int);
		void mutate_tree(node *);
		char *simplify_tree(node *, char *);
		void write_tree(node *, FILE *);
		node *read_tree(node *, FILE *);
	public:
		tree();
		~tree();
		
		int depth;
		double rawfitness, fitness;
		
		void data_load();
		void initial(int, bool);	// Initialize a tree
		double output(char *);		// Compute output for a given input
		void print();				// Print a tree according to root node
		bool compare(tree *);		// Compare two trees	
		double raw();				// Compute error 
		int nodenum();				
		void copy(tree *);			// Copy tree
		tree *crossover(tree *);	// Crossover
		void mutate();				// Mutate
		char *simplify();			// Simplify tree to equation
		void write();				// Write function to file
		void read(char *);			// Read funtion from file
};

#endif
