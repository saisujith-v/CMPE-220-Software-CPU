/*
 * Factorial Recursive Function - C Reference Implementation
 * 
 * This demonstrates a simple recursive function that will be
 * translated to assembly for the Software CPU.
 */

#include <stdio.h>

/**
 * Calculate factorial recursively
 * factorial(n) = n * factorial(n-1)
 * Base case: factorial(1) = 1
 */
int factorial(int n) {
    // Base case
    if (n <= 1) {
        return 1;
    }
    
    // Recursive case: n * factorial(n-1)
    return n * factorial(n - 1);
}

/**
 * Main program - driver
 */
int main() {
    int n = 5;
    int result;
    
    printf("Computing factorial(%d)...\n", n);
    result = factorial(n);
    printf("Result: %d\n", result);
    
    // Expected output: 120
    // 5! = 5 * 4 * 3 * 2 * 1 = 120
    
    return 0;
}
