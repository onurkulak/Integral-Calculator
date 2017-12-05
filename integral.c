#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"

#define READ_END	0
#define WRITE_END	1



int  main(int argc, char const *argv[])
{
	double L = atof(argv[1]);
	double U = atof(argv[2]);
	int K = atoi(argv[3]);
	int N = atoi(argv[4]);

	
	int** pipes = (int**)(malloc(sizeof(int*)*N));
	int i=0;
	pid_t pid = 0;
	while(i<N){
		int* tpipe = (int*)(malloc(sizeof(int)*2));
		if (pipe(tpipe) == -1) { fprintf(stderr,"Pipe failed"); return 1;}
		pipes[i] = tpipe;
		pid = fork();
		if(pid==0)
			break;
		i++;
	}
	
	if(pid==0){
		double intervalSize = (U-L)/K/N;
		double lb = L+i*(U-L)/N;
		double ub = lb+(U-L)/N;
		double sum = (compute_f(ub)+compute_f(lb))/2;
		int j = 1;
		
		while(j<K){
			sum+=compute_f(lb+j*intervalSize);
			j++;
		}
		sum*=(U-L)/N/K;

	//to improve efficiency function calls to the compute_f with same parameters are omitted
		close(pipes[i][READ_END]); //close read end (index 0) - it is unused here
		write(pipes[i][WRITE_END], &sum, sizeof(sum)); 
		close(pipes[i][WRITE_END]);
	}

	else{
		wait(NULL);
		double sum = 0;
		i = 0;
		while(i < N)
		{
			double tf;
			close(pipes[i][WRITE_END]); //close write end (index 1) - it is unused here
			read(pipes[i][READ_END], &tf, sizeof(tf));
			close(pipes[i][READ_END]);
			free(pipes[i]);
			sum+=tf;
			i++;
		}
		free(pipes);
		if(L>U)
			sum=-1*sum;
		printf("%f\n", sum);
	}


	return 0;
}



