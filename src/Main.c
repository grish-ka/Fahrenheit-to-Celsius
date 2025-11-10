#include <stdio.h>
#include <stdlib.h>
#include "Util.h" // Include the header for our DLL

/*
* Note: We do *not* define MYMATH_EXPORTS here.
* This ensures that the 'add' function is marked with
* __declspec(dllimport), telling the compiler we expect
* to find this function in an external DLL.
*/

int main(int argc, char **argv) {
    printf("--- Main Application Started ---\n");

    // int x = 5;
    // int y = 7;
    // int sum = 0;

    // // Call the 'add' function from our DLL
    // printf("Calling 'add' function from Util.dll...\n");
    // sum = add(x, y);

    // printf("The result is: %d\n", sum);

    printf("Calling 'ftc' function from Util.dll...\n");
    if (argc < 2) {
        printf("Usage: %s <Fahrenheit temperature>\n", argv[0]);
        return 1;
    }
    // FIXED: Use double for variables
    double f; 
    // sscanf(str, "%lf", &d);
    sscanf(argv[1], "%lf", &f);

    double c = ftc(f);
    // FIXED: Use %.1f format specifier for doubles
    printf("Fahrenheit %.1f is Celsius %.1f\n", f, c);

    printf("--- Main Application Exiting ---\n");

    // Wait for user input to see the messages before console closes
    printf("Press Enter to exit...\n");
    getchar(); // Waits for the Enter key
    
    return 0;
}