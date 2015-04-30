#include "gen.h"

generation::generation(){
	indiv = NULL;
	tmpindiv = NULL;
}
generation::~generation(){
}

void generation::loaddata(){
	data_load();
}

// Initialize 1st generation with POPULATION individuals
void generation::initial(){
	printf("Initialize trees");
	bestrawfitness = 0;
	avgfitness = 0;
	indiv = new tree[POPULATION];
	for(int i = 0; i < POPULATION; i++){
		if(i < 6)								// for 2 layers, only 9 operators
				indiv[i].depth = 2;
		else
			indiv[i].depth = i / 50 + 3;
		indiv[i].initial(indiv[i].depth, i%2);	// flip between grow/full
	
		// exam duplication
		if(i< 6 && i > 0){
			while(indiv[i-1].compare(&indiv[i]))
				indiv[i].initial(indiv[i].depth, i%2);  // flip between grow/full
		}
		else{
			while(examDup(i-1, &indiv[i])){
				indiv[i].initial(indiv[i].depth, i%2);	// flip between grow/full
			}
		}
	}
	
	printf("\t......finish.\n");
}

bool generation::examDup(int before, tree *p){
	bool conflict = false;
	for(int i = 0; i <= before; i++){
		if(p->output("123") == indiv[i].output("123")){
			conflict = true;
			break;
		}
	}	
	return conflict;
}

double generation::variety(){
	int count = 0;
	for(int i = 0; i < POPULATION; i++){
		bool same = false;
		for(int j = 0; j < i; j++){
			if(indiv[i].compare(&indiv[j])==true){
				same = true;
				break;	
			}
		}
		if(same==false)
			count++;
	}

	return (double)count/POPULATION;
}

/*
double generation::variety(){
	int count = 0;
	for(int i = 0; i < POPULATION; i++){
		bool same = false;
		for(int j = 0; j < i; j++){
			if(indiv[i].output("0.9") == indiv[j].output("0.9")){
				same = true;
				break;	
			}
		}
		if(same==false)
			count++;
	}

	return (double)count/POPULATION;
}
*/

// print trees in generation
void generation::print(){
	//indiv[bestindiv].print();
	indiv[bestindiv].simplify();
}

double generation::fitness(){
	double adjfitness[POPULATION] = {0};
	double sum = 0;
	avgfitness = 0;
	double bestadjfitness = 0;
	for(int i = 0; i < POPULATION; i++){
		indiv[i].rawfitness = indiv[i].raw();
		avgfitness += indiv[i].rawfitness;
		adjfitness[i] = 1 / ( 1 + indiv[i].rawfitness);
		if (bestadjfitness < adjfitness[i]){
			bestadjfitness = adjfitness[i];
			bestindiv = i; 
		}
		sum += adjfitness[i];
		bestrawfitness = indiv[bestindiv].rawfitness;
	}
	
	avgfitness = avgfitness/POPULATION;
	
	for(int i = 0; i < POPULATION; i++){
		indiv[i].fitness = adjfitness[i] / sum;	
	}
	
	return indiv[bestindiv].rawfitness;
}

/*
// propotional selection 
int generation::select(){
	double threshold = (double)rand() / RAND_MAX;
	double s = 0;
	for(int i =0; i < POPULATION; i++){
		double temp = s + indiv[i].fitness;
		if(threshold >= s && threshold < temp)
			return i;	
		else
			s = temp;
	}
	puts("Error: Cannot select by fitness.");
	exit(1);
}


//Ranking selection
int generation::select(){
	int sum = 0;
	int sum_last = 0;
	int randnum = rand() % 124750;
	int rank = -1;
	
	for(int i = 0; i < POPULATION; i++){
		sum += i;
		if (sum_last <= randnum && randnum <= sum){
			rank = i;
			break;
		}
		sum_last = sum;
	}
	double tmpfit[POPULATION] = {0};
	for(int i = 0; i < POPULATION; i++)
		tmpfit[i] = indiv[i].fitness;

	int index[POPULATION] = {0};
	for(int i = 0; i< POPULATION; i++){
		for(int j = 0; j < POPULATION; j++){
			if(index[i] < tmpfit[j]){
				index[i] = j;
			}
		}
		tmpfit[index[i]] = -1;
		if (i==rank)
			break;
	}
	
	if(rank < 0 || rank >= POPULATION){
		printf("Error rank");
		exit(1);
	}
		
	return index[rank];
}

//Tournament selection 1
int generation::select(){
	int indiv1, indiv2;
	int max, min;
	
	indiv1 = rand() % POPULATION;
	indiv2 = rand() % POPULATION;
	while (indiv2 == indiv1)
			indiv2 = rand() % POPULATION;
	
	if(indiv[indiv1].fitness > indiv[indiv2].fitness){
		max = indiv1;
		min = indiv2;
	}
	else{
		max = indiv2;
		min = indiv1;
	}
		
	double rand = (double)rand / RAND_MAX;
		
	if((double)rand / RAND_MAX < tourna_percent * POPULATION){	
				return max;
	}
	else{
				return min;
	}
}
*/
// Tourment selection 2: compare more than one pair
int generation::select(){
	int indiv1, indiv2;
	int max[POPULATION]={0};	
	int max_indiv = -1;
	double tmpfit[POPULATION] = {0};
	
	for(int i = 0; i < tourna_percent * POPULATION; i++){
		indiv1 = rand() % POPULATION;
		indiv2 = rand() % POPULATION;
		
		while (indiv2 == indiv1)
			indiv2 = rand() % POPULATION;
		double rand = (double)rand / RAND_MAX;
		
		if (indiv[indiv1].fitness > indiv[indiv2].fitness){
			if(rand < tourna_ther){	
				max[i] = indiv1;
				tmpfit[i] = indiv[indiv1].fitness;
			}
			else{
				max[i] = indiv2;
				tmpfit[i] = indiv[indiv2].fitness;
			}
		}
		else{
			if(rand < tourna_ther){
				max[i] = indiv2;
				tmpfit[i] = indiv[indiv2].fitness;
			}
			else{
				max[i] = indiv1;
				tmpfit[i] = indiv[indiv1].fitness;
			}
		}
	}

	double tmp = -1;
	for(int i = 0; i < tourna_percent*POPULATION; i++){
		if (tmpfit[i] > tmp){
			tmp = tmpfit[i];
			max_indiv = max[i];
		}
	}
	
	return max_indiv;
}

void generation::evolve(){
	tmpindiv =  new tree[POPULATION];
	
	for(int i = 0; i < (1-crossover_percent)*POPULATION; i++){
		int sel = select();
		tmpindiv[i].copy(&indiv[sel]);
	}
	
	for(int i = (1-crossover_percent)*POPULATION; i < POPULATION; i++){
		int c1 = select();
		//int c2 = select();
		int c2 = rand() % POPULATION;
		tmpindiv[i].copy(&indiv[c1]);
		tmpindiv[POPULATION - (int)(i-(1.0-crossover_percent)*POPULATION+1)].copy(&indiv[c2]);
		
		tmpindiv[POPULATION -(int)(i-(1-crossover_percent)*POPULATION+1)] = *(tmpindiv[i].crossover(&tmpindiv[POPULATION -(int)(i-(1-crossover_percent)*POPULATION+1)]));
	}
	
	for(int i = 0; i < POPULATION * mutate_percent; i++){
		int c = select();
		tmpindiv[c].mutate();
	}

	for(int i = 0; i < POPULATION; i++){
		indiv[i].copy(&tmpindiv[i]);
	}

}

double generation::bestoutput(char *s){
	return indiv[bestindiv].output(s);
}

void generation::parameters(){
	puts("\n*************************************");
	printf("Population: %d\n", POPULATION);
	printf("Percentage for selection: %f\n", tourna_percent);
	printf("Max depth of trees: %d\n", MAX_DEPTH);
	printf("Mutate rate: %f\n", mutate_percent);
	printf("Crossover rate: %f\n", crossover_percent);
	printf("Tournament threshold: %f\n", tourna_ther);
	puts("***************************************\n");
}

void generation::writebest(){
	indiv[bestindiv].write();
}

void generation::readbest(char *s){
	tree *indivbest = new tree[1];
	indivbest[0].read(s);
	indivbest[0].simplify();
	printf("\n");
}

