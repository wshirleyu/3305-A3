/**
 * @file assignment-3.c
 * @author Shirley Wu 251082034
 * @date 2023-03-02
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int threadNum, maxNum;
int *threadSums;
int *count;


void* countPrimes(void* arg){


    //divide work equally among each thread created
    int threadID = *(int*)arg; 
    // printf("threadID: %d\n", threadID);
    int start = threadID * (maxNum / threadNum);
    // printf("start value: %d\n", start);
    int end = (threadID+1) * (maxNum / threadNum);
    // printf("end value: %d\n", end);        
    

    // temp var storing sum of primes in each thread
    int pSum = 0;
    int pCount=0;


    for (int i = start; i < end; i++) {
        int isPrime = 1;
        for (int j = 2; j < i; j++) {
            // printf("\n i value: %d \n j value: %d \n", i, j);
            if (i % j == 0) {
                isPrime = 0;
                break;
            }
        }
        // confirm primeness excluding 0 and 1
        if (isPrime==1 && i>1) {
            pSum += i ;
            pCount++;
            // printf("\n pSum: %d \n pCount: %d", pSum, pCount);
            
        }
    }
    threadSums[threadID] = pSum;
    count[threadID] = pCount;
    printf("for thread # %d . . . start = %d and end = %d . . . sum value: %d and prime count: %d\n", threadID, start, end, pSum, pCount);
    // printf("\nprime count: %d\n\n", pCount);

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
    printf("\nNumber of threads to be created: %d \nMax number to count primes up to: %d\n\n", threadNum, maxNum);

  
	//allocate memory to store threads and sums
    threadSums = (int*)calloc(threadNum, sizeof(int));
    count = (int*)calloc(threadNum, sizeof(int));

    //pointer to threads created
    pthread_t* threads = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    // pointer to threadIDs
    int* threadIDs = (int*)calloc(threadNum, sizeof(int));
    
    for (int i = 0; i < threadNum; i++) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, countPrimes, &threadIDs[i]);
    }

    // wait for threads
    for (int i = 0; i < threadNum; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int totalSum = 0;
    int numPrimes = 0;


    // parse set of sums per thread
    for (int i = 0; i < threadNum; i++) {
        printf("\nthreadsums[i]: %d", threadSums[i]);
        totalSum += threadSums[i];
    }
    

    // parse set of prime numbers counted per thread
    for (int i = 0; i < threadNum; i++) {
        // numPrimes += count[i]>=0;
        numPrimes += count[i];
        // printf("\n numPrimes: %d ", numPrimes);
    }





    printf("\n\nNumber of primes less than %d: %d\n", maxNum, numPrimes);
    printf("\nSum of primes less than %d: %d\n", maxNum, totalSum);
    free(count);
    free(threadSums);
    free(threads);
    free(threadIDs);
    return 0;


}