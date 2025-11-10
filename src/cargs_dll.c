// 1. Define this to enable __declspec(dllexport) in cargs.h
#define CARGS_EXPORTS

// 2. Define this to include the C code implementation from cargs.h
#define CARGS_IMPLEMENTATION

// 3. Include the header. This will now build as a DLL.
#include "cargs.h"