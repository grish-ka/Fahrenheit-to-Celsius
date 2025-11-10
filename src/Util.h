#ifndef UTIL_H
#define UTIL_H

/*
* This is a common pattern for creating DLLs in Windows
* using Microsoft's C/C++ compilers (MSVC) or MinGW (gcc).
* We define a macro (MYMATH_API) that will change based on
* whether we are *building* the DLL or *using* the DLL.
*/

#ifdef UTIL_EXPORTS
    #define UTIL_API __declspec(dllexport)
#else
    #define UTIL_API __declspec(dllimport)
#endif

/**
 * @brief Adds two integers.
 * @param a The first integer.
 * @param b The second integer.
 * @return The sum of a and b.
 */
UTIL_API int add(int a, int b);

/**  
 * @brief Converts Fahrenheit to Celsius
 * @param f 
 * @return Celsius temperature
 */
UTIL_API double ftc(double f);
// When you add more functions, just add their prototypes here:
// UTIL_API int subtract(int a, int b);


#endif // UTIL_H