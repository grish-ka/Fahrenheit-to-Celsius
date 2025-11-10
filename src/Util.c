// We must define MYMATH_EXPORTS before including the header
// so that the functions are marked for *export*.
#define UTIL_EXPORTS

#include "Util.h"
#include <stdio.h>

// Windows-specific header for DLL entry point
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

/**
 * @brief DllMain is the entry point for a Windows DLL.
 * It's called by the system when the DLL is loaded or unloaded.
 */
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Called when the DLL is loaded into a process's address space.
        // We can do initialization here.
        printf("Util.dll loaded!\n");
        break;
    case DLL_THREAD_ATTACH:
        // Called when a new thread is created in the process.
        break;
    case DLL_THREAD_DETACH:
        // Called when a thread exits.
        break;
    case DLL_PROCESS_DETACH:
        // Called when the DLL is unloaded.
        // We can do cleanup here.
        printf("Util.dll unloaded.\n");
        break;
    }
    return TRUE; // Successful
}
#endif


// --- Implementation of our exported function ---

/**
 * @brief Implementation of the add function.
 * This is the actual code that will live inside the DLL.
 */
UTIL_API int add(int a, int b) {
    return a + b;
}

// Implementation for subtract (if you added it to the .h)
// int subtract(int a, int b) {
//     return a - b;
// }

// FIXED: Signature changed to double
UTIL_API double ftc(double f) {
    // FIXED: Use floating-point numbers (5.0 / 9.0)
    // C integer math makes 5 / 9 = 0, which is the bug.
    double c = (f - 32.0) * (5.0 / 9.0);

    return c;
}