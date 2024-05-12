# File-Processing-With-Threads

## Overview
This C program is designed to process files in a specified directory concurrently using multithreading. Each thread scans a file to determine how many numbers within are prime. The program utilizes POSIX threads (pthread) for threading and the Grand Central Dispatch (GCD) semaphore to control the number of concurrent threads.

## Features
- **Concurrency**: Uses pthreads to process multiple files simultaneously.
- **Prime Detection**: Includes a function to check if numbers in a file are prime.
- **Resource Management**: Properly manages memory and thread lifecycle.

## Prerequisites
To run this program, you need a system with:
- GCC (GNU Compiler Collection)
- pthread support
- Access to POSIX compliant libraries

## Compilation
Compile the program using the following GCC command:

```bash
gcc main.c -o main_executable
```


## Usage
Run the program by specifying the directory and the maximum number of concurrent threads:

```bash
time ./main_executable [directoryName] [threadNumber]
```

- `directoryName` - The path to the directory containing files to process.
- `threadNumber` - The maximum number of threads to run concurrently.

## Example
To process files in the `myDir` directory using up to 4 threads:
```bash
time ./main_executable myDir 4
```

## Implementation Details
- **Thread Management**: The program creates threads up to the specified limit. Each thread processes a single file.
- **Prime Checking**: Utilizes an efficient algorithm to check for prime numbers.
- **Semaphore**: Uses GCD's `dispatch_semaphore_t` to limit the number of concurrently running threads based on the user input.

## Error Handling
- The program handles typical errors such as file not opening or directory not being accessible, and exits gracefully.


