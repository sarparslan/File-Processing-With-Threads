#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>

#define MAX_FILES 30

// Define global variables
char* directory_name;
int num_threads;
sem_t semaphore;


// Function to check if a number is prime
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

void* thFun(void* arg){
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int NUM_OF_THREADS = atoi(argv[2]);
    directory_name = argv[1];
    
    sem_init(&semaphore, 0, num_threads);

    pthread_t threads[NUM_OF_THREADS];
    
    for(int i = 0;i<NUM_OF_THREADS;i++){
        pthread_create(&threads[i],NULL,thFun,NULL);
    }
    for(int i = 0;i<NUM_OF_THREADS;i++){
        pthread_join(threads[i],NULL);
    }
    
    sem_destroy(&semaphore);
    return 0;
}
