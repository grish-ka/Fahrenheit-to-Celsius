#include <stdio.h>
#include "Util.h" // Include the header for our DLL

/*
* Note: We do *not* define MYMATH_EXPORTS here.
* This ensures that the 'add' function is marked with
* __declspec(dllimport), telling the compiler we expect
* to find this function in an external DLL.
*/

int main(char **argv, int argc) {
    printf("--- Main Application Started ---\n");

    int x = 5;
    int y = 7;
    int sum = 0;

    // Call the 'add' function from our DLL
    printf("Calling 'add' function from Util.dll...\n");
    sum = add(x, y);

    printf("The result is: %d\n", sum);

    printf("Calling 'ftc' function from Util.dll...\n");
    int f = 32;
    int c = ftc(f);
    printf("Fahrenheit %d is Celsius %d\n", f, c);

    printf("--- Main Application Exiting ---\n");

    // Wait for user input to see the messages before console closes
    printf("Press Enter to exit...\n");
    getchar(); // Waits for the Enter key
    
    return 0;
}