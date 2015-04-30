#include <time.h>
#include "gen.h"

using namespace std;

int main(){
	
	srand(clock());		// randam number seed
	generation gen;

	while(true){
		char str[10];
		printf("Select mode: train, test, or exit:\t");
		scanf("%s", str);
		
		if(!strcmp(str, "exit"))
				return 0;
		
		if(!strcmp(str, "train")){
			printf("\n>>>>>> Start training\n");
			gen.parameters();
			gen.loaddata();	
			gen.initial();			// initialize the first generation
			
			int i = 0;
			double f = 0;
			int step = 0;
			
			printf("\t\t\tBest_fitness\tAverage_fitness\t\tVariety\n", i);
			printf("Epoch %d\t\t", i);
			printf("%f\t", gen.fitness());
			printf("%f\t\t", gen.avgfitness);
			printf("%f\n", gen.variety() );

			while(true){

				gen.evolve();
				gen.fitness();
			
				printf("Epoch %d\t\t", ++i);
				printf("%f\t", gen.bestrawfitness);
				printf("%f\t\t", gen.avgfitness);
				printf("%f\n", gen.variety() );
				
				//gen.print();	
				
				if(gen.bestrawfitness < 0.25)
					break;
				
				// if trapped in 0.5 for a long time, stop training.
				if(gen.bestrawfitness < 0.8){
					if(step > 30)
						break;
					step ++;
				}
			}
		
			puts("\n\n>>>>>> Finish traning. Best function:");
			gen.print();
			printf("Best fitness: %f\n", gen.bestrawfitness);
			gen.parameters();
			//gen.writebest();
		}
		
		if(!strcmp(str, "test")){
			char s[10];
			
			generation genbest;
			while(true){
				printf("\t type input value or exit:\t");
				scanf("%s", s);
				if(!strcmp(s, "exit"))
						return 0;
				if(atof(s) > 7 || atof(s) < -3)
					puts("\tPlease input number in the range of [-3.0, 7.0]. ");
				else{
					genbest.readbest(s);
				}
			}
		}
		
	}
	return 0;
}

