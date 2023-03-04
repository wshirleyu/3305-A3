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

void* countPrimes(void* arg){
    printf("\n\n Entered countPrimes \n");


    int threadID = *(int*)arg;
    int start = threadID * (maxNum / threadNum);
    int end = (threadID + 1) * (maxNum / threadNum);

    int sum = 0;
    for (int i = start; i < end; i++) {
        int isPrime = 1;
        for (int j = 2; j < i; j++) {
            if (i % j == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            sum += i;
        }
    }
    threadSums[threadID] = sum;
    pthread_exit(NULL);

}



int main(int argc, char *argv[]) {


    // verify command line parameter input
    // int threadNum, maxNum;
    if (argc != 3) {
        printf("Error: Please enter exactly two integer command-line parameters.\n");
        return 1;
    }
    threadNum = atoi(argv[1]);
    maxNum = atoi(argv[2]);
    printf("The number of threads to be created is %d \n and the max number to count primes up to is is %d\n", threadNum, maxNum);


    // int *sum;

    // pthread_t pSum; // holds information fo thread created
    // pthread_create(&pSum, NULL, &routine, NULL);
    // pthread_join(pSum, &sum)

    // check if there's more than one thread
	// if (threadNum == 1){
        // create thread
        // pthread_t pSum; 

        // count primes
        
        // add primes
        // return sum
        
    // }

    
    // else if (threadNum>0){
    //     // create array to store threads
    //     int count[threadNum] = {0};

    //     // create array to store sums
    //     int sum[threadNum]={0};

        

    // }
		



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