#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"
#include <pthread.h>
#include <errno.h>
double L;
double U;
int  K;
int N;
double* sums;

void* calc(void *param){
	int i = *((int*)param); 
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
		sums[i] = sum;
		return 0;
}

int  main(int argc, char const *argv[])
{
	 L = atof(argv[1]);
	 U = atof(argv[2]);
	 K = atoi(argv[3]);
	 N = atoi(argv[4]);

	pthread_t * threads=(pthread_t *)malloc(N*sizeof(pthread_t));
	sums = (double*)(malloc(sizeof(double)*N));
	int i=0;
	
	while(i<N){
		int *x = (int*)(malloc(sizeof(int)));
		*x = i;
		if(0==pthread_create(&threads[i], NULL, calc, (void*)x))
		i++;
		else pthread_join(threads[i-1], NULL);
	}
	
	i = 0; 
	while(i<N)
		pthread_join(threads[i++], NULL);
		double sum = 0;
		i = 0;
		while(i < N)
			sum+=sums[i++];
			
		if(L>U)
			sum=-1*sum;
		printf("%f\n", sum);
	


	return 0;
}



