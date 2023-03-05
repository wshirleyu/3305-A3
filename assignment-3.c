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
long int *threadSums;
long int *count;

/**
 * method to count and add the prime numbers in a range 
 * 
 * @param arg 
 * @return void* 
 */
void* countPrimes(void* arg){


    // set ranges to divide work equally among each thread created
    long int threadID = *(int*)arg;      // pointer to ID to access each individual thread
    long int start = threadID * (maxNum / threadNum);
    long int end = (threadID+1) * (maxNum / threadNum);


    // create temporary variable storing sum of primes in each thread
    long int pSum = 0;
    long int pCount=0;


    // parse numbers and check if prime
    for (int i = start; i < end; i++) {
        // flag for primeness
        int isPrime = 1;
        for (int j = 2; j < i; j++) {
            if (i % j == 0) { // if number is divisible by j, number is prime
                isPrime = 0; // change flag to 0 if not prime
                break;
            }
        }
        // confirm primeness and exclude numbers 0 and 1
        if (isPrime==1 && i>1) {
            pSum += i ; // increment sum of primes
            pCount++; // increment count   
        }
    }

    threadSums[threadID] = pSum;    // populate sum of primes per each thread
    count[threadID] = pCount;       // populate count of primes per each thread

    printf("\nThread # %ld is finding primes from low = %ld to high %ld ", threadID, start, end);
    printf("\nSum of thread %ld is %ld, Count is %ld", threadID, pSum, pCount);

    pthread_exit(NULL);             // leave thread

}


// main method
int main(int argc, char *argv[]) {

    // verify command line parameter input
    if (argc != 3) {
        printf("Proper usage is ./assignment-3 <threadCount> <highestInt>\n");
        return 1;
    }
    // convert strings to integers
    threadNum = atoi(argv[1]);
    maxNum = atoi(argv[2]);

	//allocate memory to store threads and sums
    threadSums = (long int*)calloc(threadNum, sizeof(long int));
    count = (long int*)calloc(threadNum, sizeof(long int));

    //allocate memory to point to threads created
    pthread_t* threads = (pthread_t*)calloc(threadNum, sizeof(pthread_t));

    // pointer to threadIDs
    long int* threadIDs = (long int*)calloc(threadNum, sizeof(int));
    
    // create threads corresponding to number from command line input
    for (int i = 0; i < threadNum; i++) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, countPrimes, &threadIDs[i]);
    }

    // wait for each thread to complete
    for (int i = 0; i < threadNum; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // create variables to store GRAND SUM and COUNT outputs
    long int totalSum = 0;
    long int numPrimes = 0;

    // parse set of sums per thread
    for (int i = 0; i < threadNum; i++) {
        totalSum += threadSums[i];
    }

    // parse set of prime numbers counted per thread
    for (int i = 0; i < threadNum; i++) {
        numPrimes += count[i];
    }

    if (totalSum>0 && numPrimes>0){
            printf("\n \n \tGRAND SUM IS %ld, COUNT IS %ld \n\n", totalSum, numPrimes);
    }

    //free allocated memory
    free(count);
    free(threadSums);
    free(threads);
    free(threadIDs);
    return 0;


}