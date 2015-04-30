#ifndef GEN_H
#define GEN_H
#include "tree.h"

class generation{
	private:
		tree *indiv;				// Typically 500 indiv in each generation
		tree *tmpindiv;
		tree *indivbest;
		int select();
	public:
		generation();
		~generation();
		
		double bestrawfitness;		// Compute best fitness
		double avgfitness;			// Compute average fitness
		int bestindiv;				
		
		void loaddata();
		void initial();				// Generate 500 individuals			
		double fitness();			// Calculate fitness in each generation
		void print();				// Print all indiv in generation
		void evolve();				// Generation evolution
		bool examDup(int, tree *);	// Exam duplication for the initial gen
		double variety();			// Compute variety
		double bestoutput(char *);	
		void parameters();			// Print parameters
		void writebest();			// Write best function to file
		void readbest(char *);		// Read saved best function from file
};

#endif
