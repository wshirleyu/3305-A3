/**
 * @file assignment-3.c
 * @author Shirley Wu 251082034
 * @date 2023-03-02
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
// #include <math.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <errno.h>
#include <pthread.h>

int threadNum, maxNum;
int *threadSums;
int *count;


void* countPrimes(void* arg){
    // printf("\n\n Entered countPrimes \n");

    //divide work equally among each thread created
    int threadID = *(int*)arg; 
    printf("threadID: %d\n", threadID);
    int start = threadID * (maxNum / threadNum);
    printf("start value: %d\n", start);
    int end = (threadID+1) * (maxNum / threadNum);
    printf("end value: %d\n", end);

    // temp var storing sum of primes in each thread
    int pSum = 0;
    int primeCount=0;
    for (int i = start; i < end; i++) {
        int isPrime = 1;
        for (int j = 2; j < i; j++) {
            if (i % j == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            pSum += i;
            primeCount= primeCount+1;
            
        }
    }
    threadSums[threadID] = pSum;
    printf("\nsum value: %d\n", pSum);
    pthread_exit(NULL);

}



int main(int argc, char *argv[]) {


    // verify command line parameter input
    if (argc != 3) {
        printf("Error: Please enter exactly two integer command-line parameters.\n");
        return 1;
    }
    threadNum = atoi(argv[1]);
    maxNum = atoi(argv[2]);
    printf("\nNumber of threads to be created: %d \nMax number to count primes up to: %d\n", threadNum, maxNum);

  
	//allocate memory to store threads and sums
    threadSums = (int*)calloc(threadNum, sizeof(int));
    pthread_t* threads = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    int* threadIDs = (int*)calloc(threadNum, sizeof(int));
    
    for (int i = 0; i < threadNum; i++) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, countPrimes, &threadIDs[i]);
    }
    for (int i = 0; i < threadNum; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int totalSum = 0;
    int numPrimes = 0;
    for (int i = 0; i < threadNum; i++) {
        totalSum += threadSums[i];
        numPrimes += threadSums[i] > 0;
    }

    printf("Number of primes less than %d: %d\n", maxNum, numPrimes);
    printf("Sum of primes less than %d: %d\n", maxNum, totalSum);
    free(threadSums);
    free(threads);
    free(threadIDs);
    return 0;


}