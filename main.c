#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <dispatch/dispatch.h>

#define MAX_PATH 256  // Maximum length for paths used in the program

// Structure to hold thread arguments
typedef struct {
    char *filename;   // File path to be processed
    int thread_id;    // Identifier for the thread for easier tracking
} ThreadArg;

dispatch_semaphore_t semaphore; // Semaphore for limiting concurrent thread execution

// Function to check if a number is prime
int is_prime(int n) {
    if (n <= 1){
        return 0;
    }
    if (n <= 3){
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0){
        return 0;
    }
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0){
            return 0;
        }
    }
    return 1;  // Number is prime if no divisors found
}

// Function to process each file
void* process_file(void* arg) {
    ThreadArg *thread_arg = (ThreadArg*)arg;  // Casting arg back to ThreadArg structure
    FILE* file = fopen(thread_arg->filename, "r");  // Opening the file for reading
    if (file == NULL) {
        perror("Error opening file");
        free(thread_arg->filename);  // Free allocated memory for filename
        free(thread_arg);  // Free allocated memory for the structure
        pthread_exit(NULL);  // Exit the thread
    }

    int prime_count = 0;
    int num;
    
    // fscanf() reads integers from the file one by one
    // If a number is successfully read, it checks whether the number is prime using the is_prime() function
    while (fscanf(file, "%d", &num) == 1) {
        if (is_prime(num)) {
            prime_count++; // If the number is prime, incrementing the prime_count
        }
    }
    fclose(file);
    
    // Finds the last occurrence of '/' in the file path
    char* short_filename = strrchr(thread_arg->filename, '/') + 1;  // Extract only the filename
    
    // Prints the thread ID, the number of primes found, and the filename to standard output
    printf("Thread %d has found %d primes in %s\n", thread_arg->thread_id, prime_count, short_filename);
    
    free(thread_arg->filename);  // Free the filename memory
    free(thread_arg);  // Free the structure memory
    dispatch_semaphore_signal(semaphore);  // Signal the semaphore
    pthread_exit(NULL);  // Exit the thread
}

// Function to traverse the specified directory and launch threads
void traverse_directory(const char* directory_name) {
    DIR* dir = opendir(directory_name);  // Opening the directory
    if (dir == NULL) {
        perror("Error opening directory");  // Printing error if directory cannot be opened
        exit(EXIT_FAILURE);  // Exiting the program with a failure status
    }

    struct dirent* entry;  // Directory entry structure
    // Entry will point to each directory entry within the given directory

    pthread_t threads[MAX_PATH];  // Array to store thread identifiers
    int t_count = 0;  // Counter for threads
    
    // readdir() reads the next entry in the directory,
    // The loop continues until there are no more entries or the maximum limit of MAX_PATH threads is reached
    while ((entry = readdir(dir)) != NULL && t_count < MAX_PATH) {  // Reading directory entries
        if (entry->d_type == DT_REG) {  // Checking if the entry is a regular file
            ThreadArg *thread_arg = malloc(sizeof(ThreadArg));  // Allocating memory for thread arguments
            thread_arg->filename = malloc(MAX_PATH);  // Allocating memory for the filename
            snprintf(thread_arg->filename, MAX_PATH, "%s/%s", directory_name, entry->d_name);  // Constructing full path of the file
            thread_arg->thread_id = t_count + 1;  // Assign thread ID
            
            // Waits for the semaphore, which limits the number of concurrent threads as per the user input
            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
            
            // Creates a new thread that starts execution by calling process_file function with thread_arg as its argument
            pthread_create(&threads[t_count], NULL, process_file, (void*)thread_arg);  // Create a new thread
            t_count++;  // Increment thread counter
        }
    }

    for (int i = 0; i < t_count; i++) {
        pthread_join(threads[i], NULL);  // Wait for all threads to finish
    }
    closedir(dir);  // Close the directory
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s directoryName threadNumber\n", argv[0]);  // Print usage if arguments are incorrect
        exit(EXIT_FAILURE);
    }

    const char* directory_name = argv[1];  // Get the directory name from arguments
    int num_threads = atoi(argv[2]);  // Get the number of threads from arguments
    semaphore = dispatch_semaphore_create(num_threads);  // Initialize the semaphore

    traverse_directory(directory_name);  // Call function to process the directory

    return 0;  // Exit program successfully
}

